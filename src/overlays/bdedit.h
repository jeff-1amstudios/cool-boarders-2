#ifndef CB2_BDEDIT_H
#define CB2_BDEDIT_H

#include "main.h"

int func_8005AB6C(FileLoadState *state);

unsigned char *func_8005BE94(unsigned long *ot, unsigned char *packet,
                          int selection, unsigned char **planes,
                          CVECTOR *palette, short *cursor,
                          int mode, unsigned char *mask, int value9, int flags);

void func_8005C020(unsigned char **planes);

#endif
