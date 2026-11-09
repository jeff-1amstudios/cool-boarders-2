/* Visit all four words; callee results are discarded. */
extern int func_8003CACC(unsigned int);
void func_80017B18(unsigned int *words) {
    int i;
    for (i = 0; i < 4; i++) func_8003CACC(words[i]);
}
