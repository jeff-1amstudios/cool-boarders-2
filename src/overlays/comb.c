#include "comb.h"

extern int D_800570E0;
extern int func_8001F378(void *allocation);

// FUNCTION: COMB.BIN:0x8005AEA4
void func_8005AEA4(FileLoadState *state)
{
    void *allocation;
    void **entry;
    int index;

    if (D_800570E0 < 2 && (index = 2, D_800570E0 >= 0)) {
        entry = (void **)&state->file_5c;
        do {
            allocation = *entry;
            if (allocation != 0 && (unsigned int)allocation < (unsigned int)state->unknown_04) {
                func_8001F378(allocation);
            }
            index++;
            entry++;
        } while (index < 16);
    }
}
