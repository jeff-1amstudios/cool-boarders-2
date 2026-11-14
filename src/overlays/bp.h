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

typedef struct BpSetupValues {
    unsigned int word_00, word_04, word_08;
} BpSetupValues;

typedef struct BpSetupLink {
    unsigned char unknown_00[0x1C];
    int value;
    unsigned char unknown_20[0x28];
    int *progress;
} BpSetupLink;

typedef struct BpSetupSource {
    unsigned char unknown_0000[0x70];
    BpSetupValues values;
    unsigned int word_0c;
    unsigned char unknown_0080[0x1200];
    BpSetupLink *link;
    short value;
} BpSetupSource;

typedef struct BpSetupState {
    BpSetupValues values;
    unsigned int word_0c;
    unsigned char unknown_0010[0x4DC];
    short value;
    unsigned char unknown_04ee[2];
    BpSetupLink *link;
} BpSetupState;

typedef struct BpMotionSample {
    int x, y, z;
    SVECTOR rotation;
    int active;
} BpMotionSample;

typedef struct BpMotionState {
    unsigned char unknown_000[0x84];
    short height;
    short unknown_086;
    int history_a[16];
    int history_b[16];
    short angles[4];
    short *angle_buffer;
    short mode;
    short unknown_116;
    int value;
    int negative_height;
    int timer;
    int word_124;
    int word_128;
    BpMotionSample samples[4];
    int selected;
    int word_190;
    int word_194;
    int word_198;
    int word_19c;
    int word_1a0;
} BpMotionState;

#endif
