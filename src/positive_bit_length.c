/* Count arithmetic right shifts until a positive signed input reaches zero. */
int func_8002FCEC(int value) {
    int count = 0;
    while (value > 0) {
        value >>= 1;
        count++;
    }
    return count;
}
