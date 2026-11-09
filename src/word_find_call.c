/* Return the index of the first nonzero callee result among four words. */
extern int func_8003CACC(unsigned int);
int func_80017AC0(unsigned int *words) {
    int i;
    for (i = 0; i < 4; i++) {
        if (func_8003CACC(words[i])) return i;
    }
    return -1;
}
