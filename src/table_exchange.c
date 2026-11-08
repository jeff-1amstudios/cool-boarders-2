extern int D_80047D18[];

int func_8001B5B4(int index, int value) {
    int previous = D_80047D18[index];
    D_80047D18[index] = value;
    return previous;
}
