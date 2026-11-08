/* The caller supplies storage for sixteen 32-bit words. */
void func_8001C0D4(unsigned int *words) {
    int i;
    for (i = 15; i >= 0; i--) {
        words[i] = 0;
    }
}
