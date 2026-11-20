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

typedef struct BpMotionTransform {
    unsigned char unknown_000[0x2C];
    SVECTOR rotation;
    GsCOORDINATE2 coordinate;
} BpMotionTransform;

typedef struct BpMotionPlayer {
    unsigned char unknown_000[0x2C];
    int position[3];
    unsigned char unknown_038[4];
    short height;
    short airborne;
    unsigned char unknown_040[0x19A];
    short heading;
    unsigned char unknown_1dc[0x4FC];
    BpMotionState *motion;
    unsigned char unknown_6dc[0x28];
    int progress;
    unsigned char unknown_708[0x544];
    short reverse;
    unsigned char unknown_c4e[0x3C96];
    int vertical_speed;
} BpMotionPlayer;

typedef struct BpMotionExtended {
    BpMotionState motion;
    int saved_height;
} BpMotionExtended;

typedef struct BpCameraState {
    unsigned char unknown_000[0xC];
    GsRVIEW2 view;
    unsigned char unknown_02c[0x17C];
    int position[3];
} BpCameraState;

typedef struct BpPoseInput {
    unsigned char unknown_00[0x64];
    short amount;
} BpPoseInput;

typedef struct BpPosePlayer {
    BpMotionPlayer player;
    unsigned char unknown_48e8[0x30];
    BpPoseInput *pose;
} BpPosePlayer;

typedef struct BpPoseState {
    BpMotionTransform transform;
    short height;
    unsigned char unknown_086[0x11E];
    int saved_height;
} BpPoseState;

typedef struct BpSelectionView {
    unsigned char unknown_00[20];
    short course;
    short record;
    int mode;
} BpSelectionView;

typedef struct BpSurfaceState {
    int kind;
    int x, y, z;
    int word_10;
    short surface;
    short mode;
    int word_18;
    SVECTOR offset;
    unsigned char unknown_24[0x18];
    int normal[3];
    int word_48;
    int height;
    int word_50;
    int word_54;
} BpSurfaceState;

typedef struct BpModelHeader {
    unsigned int unknown_00;
    unsigned int unknown_04;
    unsigned int object_count;
} BpModelHeader;

typedef struct BpCoordinateHeader {
    unsigned int unknown_00;
    unsigned short coordinate_count;
} BpCoordinateHeader;

typedef struct BpLightColor {
    unsigned char r, g, b;
    unsigned char ambient;
} BpLightColor;

typedef struct BpLightingTable {
    BpLightColor colors[16][3];
} BpLightingTable;

#endif
