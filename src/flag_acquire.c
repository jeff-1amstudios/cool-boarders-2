/* Halfword flag access; ownership and subsystem meaning remain provisional. */
extern unsigned short D_800478D0;

int func_80015F68(void) {
    int result;
    if (D_800478D0 == 0) {
        D_800478D0 = 1;
        result = 0;
    } else {
        result = -1;
    }
    return result;
}
