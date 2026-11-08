/* Signed result, preserved across a call, followed by a conditional loop. */
extern int func_80013C70(void);
extern void func_8001B320(int, int);
extern int func_800449F8(void);
extern int func_8003BD60(int);

void func_80014388(void) {
    int pending;
    for (;;) {
        pending = func_80013C70() - 1;
        func_8001B320(0, 0);
        if (pending < 0) break;
        func_800449F8();
        func_8003BD60(0);
    }
}
