/* Shift/XOR byte fold. Both shifts are left shifts, as observed. */
unsigned int func_800181D4(unsigned char *bytes, unsigned int count) {
    unsigned int result = 0x6B383658;
    unsigned int i;
    for (i = 0; i < count; i++) {
        unsigned int value = *bytes++;
        result ^= (value << (i & 31)) | (value << ((32 - i) & 31));
    }
    return result;
}
