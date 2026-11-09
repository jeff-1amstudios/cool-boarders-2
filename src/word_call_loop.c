/* Eight indexed words are passed between two calls; API names are provisional. */
extern void func_8003CB2C(void);
extern void func_8003CABC(unsigned int word);
extern void func_8003CB3C(void);

void func_80017A00(unsigned int *words) {
    int i;
    func_8003CB2C();
    for (i = 0; i < 8; i++) {
        func_8003CABC(words[i]);
    }
    func_8003CB3C();
}
