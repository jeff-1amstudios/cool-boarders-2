/* Four-byte entry stride is observed; entry contents remain provisional. */
extern unsigned int D_80050570[];
extern void func_8004097C(int command, void *argument);

void func_80017044(int index) {
    func_8004097C(22, &D_80050570[index]);
}
