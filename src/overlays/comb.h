#ifndef CB2_COMB_H
#define CB2_COMB_H

#include "main.h"

void func_8005AEA4(FileLoadState *state);

typedef struct CombPlayer {
    unsigned char unknown_0000[0x1F4];
    void *buffer_1f4;
    void *buffer_1f8;
    void *buffer_1fc;
    unsigned char unknown_0200[0x4714];
    void *effects;
    unsigned char unknown_4918[12];
} CombPlayer;

typedef struct CombSettings {
    short character;
    unsigned char unknown_02[8];
    short second_character;
    unsigned char unknown_0c[8];
    short course;
    unsigned char unknown_16[6];
    short option;
} CombSettings;

int func_8005B568(void);

void func_8005B8D0(void);

void func_8005BB00(GsOT *ot, unsigned char shade);

void func_8005D774(int first, int second, int mode);

int func_8005DEE4(int mode, int value);

typedef struct CombSceneEntry {
    unsigned char unknown_00[12];
    short linked_entries[3];
    unsigned char unknown_12[38];
} CombSceneEntry;

void func_8005DF88(GsOT *ot, void *view, void *position, int mode, int entry);

typedef struct CombRenderView {
    unsigned char unknown_000[0x188];
    GsCOORDINATE2 coordinate;
    unsigned char unknown_1d8[12];
    GsDOBJ3 *objects;
    int object_count;
    unsigned char unknown_1ec[0xA68];
    short forward_order;
} CombRenderView;

void func_8005E7A4(GsOT *ot, CombPlayer *player, int index);

void func_8005E9E0(GsOT *ot, int shift, int kind, int entry);

typedef struct CombPlayerEffectView {
    unsigned char unknown_00[0x2C];
    VECTOR position;
    short entry;
} CombPlayerEffectView;

void func_8005EB18(GsOT *ot, CombPlayer *player);

int func_8005EC58(VECTOR *origin, MATRIX *matrix, VECTOR *normal);

typedef struct CombPlayerHeadingView {
    unsigned char unknown_0000[0x96];
    short state_96;
    unsigned char unknown_0098[0xB0];
    int state_148;
    unsigned char unknown_014c[0x8E];
    short heading;
    unsigned char unknown_01dc[0x4FC];
    unsigned short *surface;
    unsigned char unknown_06dc[0x48];
    int value_724;
    unsigned char unknown_0728[6];
    short mode;
    unsigned char unknown_0730[0x41B8];
    long motion_x;
    unsigned char unknown_48ec[4];
    long motion_z;
} CombPlayerHeadingView;

void func_8005EDC4(CombPlayerHeadingView *view, int value);

typedef struct CombHudEntry {
    unsigned char unknown[0x34];
} CombHudEntry;

typedef struct CombHudState {
    int unknown_00;
    int value_04;
    int value_08;
    int value_0c;
    int value_10;
    int value_14;
    int unknown_18;
    int value_1c;
    int value_20;
    unsigned char unknown_24[0x18];
    int value_3c;
    int unknown_40;
    int value_44;
    int value_48;
    int value_4c;
    int value_50;
    int countdown;
    int value_58;
    int value_5c;
} CombHudState;

typedef struct CombMovementHudView {
    unsigned char unknown_00[0x1C];
    int angular_speed;
    unsigned char unknown_20[0x18];
    unsigned short speed;
} CombMovementHudView;

typedef struct CombPlayerHudView {
    unsigned char unknown_0000[0x3C];
    short state;
    unsigned char unknown_003e[0x69A];
    short *surface;
    unsigned char unknown_06dc[0x28];
    int progress;
    unsigned char unknown_0708[0x2C];
    CombMovementHudView movement;
    unsigned char unknown_0770[0x4A];
    short display_position;
    unsigned char unknown_07bc[4];
    short display_side;
    unsigned char unknown_07c2[6];
    CombHudState hud;
    unsigned char unknown_0828[0x28];
    CombHudEntry *entries;
    unsigned char unknown_0854[0x36];
    short entry_count;
    unsigned char unknown_088c[0x42A];
    short value_cb6;
    unsigned char unknown_0cb8[0x1E];
    short value_cd6;
    unsigned char unknown_0cd8[0x138];
    unsigned char display_e10[0x3AE8];
    int previous_progress;
    unsigned char unknown_48fc[0x20];
    short value_491c;
} CombPlayerHudView;

void func_8005EEE0(GsOT *ot, CombPlayerHudView *player, int first, int second);

#endif
