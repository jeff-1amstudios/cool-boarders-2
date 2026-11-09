/* Copy at most 256 bytes, including a terminator if encountered. */
extern unsigned char D_80058D10[];
extern int D_80058C40;
extern int D_8005AB54;
void func_8001E560(unsigned char *source, int value) {
    int i;
    for (i = 0; i < 256; i++) {
        D_80058D10[i] = *source;
        if (*source == 0) break;
        source++;
    }
    D_80058C40 = value;
    D_8005AB54 = 0;
}
