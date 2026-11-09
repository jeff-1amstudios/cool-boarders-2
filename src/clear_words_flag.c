/* Clear sixteen words, then initialize the first three fields. */
int func_8001C0F4(int *words, int flag) {
    int i;
    for (i = 15; i >= 0; i--) words[i] = 0;
    words[0] = 0;
    words[1] = 0;
    if (flag) words[2] = -1;
    else words[2] = 0;
    return 3;
}
