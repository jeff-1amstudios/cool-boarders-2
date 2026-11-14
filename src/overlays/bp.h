#ifndef CB2_BP_H
#define CB2_BP_H

#include "main.h"

typedef struct BpPlayer {
    unsigned char unknown_0000[0x1F4];
    void *buffer_1f4;
    void *buffer_1f8;
    void *buffer_1fc;
    unsigned char unknown_0200[0x470C];
    void *effects;
    unsigned char unknown_4910[12];
} BpPlayer;

int func_8005B4A4(void);

#endif
