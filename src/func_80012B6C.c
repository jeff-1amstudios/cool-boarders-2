/* Build a draw-mode packet and prepend it to the ordering table. */
typedef struct {
    unsigned int address : 24;
    unsigned int length : 8;
    unsigned int command;
} DrawModePacket;

extern int func_800296D0(void);

unsigned char *func_80012B6C(unsigned int *ordering_table, unsigned char *buffer,
                            unsigned int mode) {
    DrawModePacket *packet = (DrawModePacket *)buffer;
    buffer += sizeof(DrawModePacket);
    packet->length = 1;
    packet->command = 0xE1000000 |
        ((func_800296D0() == 1 || func_800296D0() == 2)
            ? mode & 0x27FF : mode & 0x09FF);
    packet->address = *ordering_table & 0x00FFFFFF;
    *ordering_table = (*ordering_table & 0xFF000000) |
                      ((unsigned int)packet & 0x00FFFFFF);
    return buffer;
}
