/* Transition states 1/2 to 3, preserving a signed halfword as a word. */
extern int D_80050680;
extern int D_80050688;
extern int D_8005068C;
extern short D_80057FB8;
void func_8001742C(int value) {
    if (D_80050680 == 1 || D_80050680 == 2) {
        D_80050688 = value;
        D_80050680 = 3;
        D_8005068C = D_80057FB8;
    }
}
