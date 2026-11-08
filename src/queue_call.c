/* Initial matching reconstruction; symbol and layout types remain provisional. */
typedef struct { unsigned int word0; unsigned int word1; } WordPair;
extern WordPair D_800580A8[];
extern void func_8004097C(int command, void *argument);

void func_80013B30(int index) {
    func_8004097C(21, &D_800580A8[index]);
}
