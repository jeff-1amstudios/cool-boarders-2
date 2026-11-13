#ifndef CB2_CARD_GHOST_READ_H
#define CB2_CARD_GHOST_READ_H

static __inline__ int card_ghost_read_validate(CardGhostHeader *data)
{
    CardGhostHeader *header = data;
    unsigned char *signature = card_ghost_read_signature;
    unsigned char *expected = signature;
    CardGhostHeader *input = data;
    unsigned char *payload = (unsigned char *)header + header->data_offset;

    do {
        if (*(unsigned char *)input != *expected++) {
            return -1;
        }
        input = (CardGhostHeader *)((unsigned char *)input + 1);
    } while ((int)expected < (int)(signature + 8));
    if (func_800181D4((unsigned char *)&header->data_offset, 12) != header->checksum) {
        return -2;
    }
    return -(func_800181D4(payload, header->data_size) != header->data_checksum);
}

#endif
