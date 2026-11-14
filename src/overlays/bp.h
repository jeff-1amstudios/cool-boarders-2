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

void func_8005AEA8(FileLoadState *state);
void func_8005B720(void);

typedef struct BpViewState {
    short state;
    short unknown_02;
    short previous_state;
    short unknown_06;
    short timer;
    unsigned char unknown_0a[0x19A];
    void *data;
} BpViewState;

typedef struct BpMotion {
    int unknown_00;
    int x, y, z;
    unsigned char unknown_10[4];
    short mode;
    unsigned char unknown_16[0x42];
    unsigned int buttons;
    int camera_x, camera_y, camera_z;
    unsigned char unknown_68[0x48];
    unsigned int previous_buttons;
    int previous_x, previous_y, previous_z;
} BpMotion;

typedef struct BpRenderEntry {
    unsigned char unknown_00[12];
    short related[3];
    unsigned char unknown_12[38];
} BpRenderEntry;

typedef struct BpRenderView {
    unsigned char unknown_000[0x188];
    GsCOORDINATE2 coordinate;
    unsigned char unknown_1d8[12];
    GsDOBJ3 *objects;
    int object_count;
    unsigned char unknown_1ec[0xA60];
    short forward_order;
} BpRenderView;

#endif
