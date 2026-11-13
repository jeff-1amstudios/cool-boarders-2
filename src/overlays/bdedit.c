#include "bdedit.h"

extern void func_80015948(unsigned char *first_data, int first_bank,
                          unsigned char *second_data, int second_bank);
extern void func_8005AC10(void);
extern unsigned int func_8001B320(int port, unsigned char *state);
extern int func_8001C130(FileLoadState *state, short mode);
extern int D_80045370;
extern int D_8004536C;

// FUNCTION: BDEDIT.BIN:0x8005AB6C
int func_8005AB6C(FileLoadState *state)
{
    func_80015948((unsigned char *)state->file_5c, 0x41, 0, -1);
    func_8005AC10();
    if (D_80045370 != 0 && (func_8001B320(0, 0) & 0x401F) == 0x401F) {
        D_8004536C = rand() % 2 + 10;
    }
    func_8001C130(state, 1);
    return 3;
}
