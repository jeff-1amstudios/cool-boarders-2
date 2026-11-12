#ifndef CB2_CARD_FORMAT_H
#define CB2_CARD_FORMAT_H

static __inline__ void card_format_open_events(long *events)
{
    long event;
    int i;
    long *cursor;

    func_8003CB2C();
    events[0] = OpenEvent(0xF4000001, 4, 0x2000, 0);
    events[1] = OpenEvent(0xF4000001, 0x8000, 0x2000, 0);
    events[2] = OpenEvent(0xF4000001, 0x100, 0x2000, 0);
    events[3] = OpenEvent(0xF4000001, 0x2000, 0x2000, 0);
    events[4] = OpenEvent(0xF0000011, 4, 0x2000, 0);
    events[5] = OpenEvent(0xF0000011, 0x8000, 0x2000, 0);
    events[6] = OpenEvent(0xF0000011, 0x100, 0x2000, 0);
    events[7] = OpenEvent(0xF0000011, 0x2000, 0x2000, 0);
    func_8003CB3C();
    i = 0;
    cursor = events;
    do {
        event = *cursor++;
        i++;
        EnableEvent(event);
    } while (i < 8);
}

static __inline__ void card_format_close_events(long *events)
{
    long event;
    int i;
    long *cursor;

    func_8003CB2C();
    i = 0;
    cursor = events;
    do {
        event = *cursor++;
        i++;
        CloseEvent(event);
    } while (i < 8);
    func_8003CB3C();
}

static __inline__ int card_format_load_result(long *events)
{
    CardLoadResultTable results;
    long event;
    int i;
    long *cursor;
    int index;
    int retries;
    int result;
    long status;
    int result_offset;
    long *event_base;

    results = card_load_result_table;
    i = 0;
    event_base = events;
    cursor = event_base;
    do {
        event = *cursor++;
        i++;
        TestEvent(event);
    } while (i < 4);
    retries = 0;
    do {
        _card_load(0);
        do {
            index = 0;
            do {
                status = TestEvent(event_base[index]);
                result_offset = index * sizeof(int);
                if (status != 0) {
                    goto load_event;
                }
                index++;
            } while (index < 4);
        } while (1);
load_event:
        if (index == 0) {
            break;
        }
        retries++;
    } while (retries < 16);
    /* Keep byte-offset address addition in the original operand order. */
    result = *(int *)(result_offset + (unsigned long)&results);
    return result;
}

#endif
