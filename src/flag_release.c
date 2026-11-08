/* Paired halfword flag access; no atomicity or subsystem role is inferred. */
extern unsigned short D_800478D0;

int func_80015F98(void) {
    int result;
    if (D_800478D0 != 0) {
        D_800478D0 = 0;
        result = 0;
    } else {
        result = -1;
    }
    return result;
}
