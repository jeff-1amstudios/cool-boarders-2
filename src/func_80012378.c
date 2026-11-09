extern int func_80040814(int value);
extern int func_8004082C(int value);
extern int func_800296F0(int value);
extern void func_8003F3F4(int mode, unsigned int mask);
extern void func_8003F094(void);
extern void func_8001BA78(void);
extern void func_80029328(int mode);
extern void func_80040734(void);
extern void func_8003CD34(void);
extern void func_800178AC(void);
extern void func_8003BD40(void);
extern void func_8003C038(void);

void func_80012378(void) {
    func_80040814(0);
    func_8004082C(0);
    func_800296F0(0);
    func_8003F3F4(0, 0xFFFFFF);
    func_8003F094();
    func_8001BA78();
    func_80029328(3);
    func_80040734();
    func_8003CD34();
    func_800178AC();
    func_8003BD40();
    func_8003C038();
}
