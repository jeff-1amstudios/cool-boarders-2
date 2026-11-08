/* State word and command meanings remain provisional. */
extern int D_8005AB5C;
extern int func_8003D714(int, int, int);

void func_8001BF08(void) {
    D_8005AB5C = 0;
    func_8003D714(2, 0, 0);
    func_8003D714(1, 1, 3);
}
