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

#endif
