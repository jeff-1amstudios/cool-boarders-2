/* Clear two words and fill the 64-byte slot table. */
extern int D_80058B10;
extern int D_8005A7B8;
extern unsigned char D_80058CC8[];

void func_800166A4(void) {
    int i;
    unsigned char *slots;
    unsigned char value;
    D_80058B10 = 0;
    D_8005A7B8 = 0;
    value = 255;
    slots = D_80058CC8;
    for (i = 63; i >= 0; i--) {
        slots[i] = value;
    }
}
