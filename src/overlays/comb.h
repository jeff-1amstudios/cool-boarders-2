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

typedef struct CombRaceStateView {
    unsigned char unknown_00[0x5C];
    short value_5c;
} CombRaceStateView;

typedef struct CombPlayerRaceHudView {
    CombPlayerHudView player;
    CombRaceStateView *race;
} CombPlayerRaceHudView;

typedef struct CombPlayerRenderStateView {
    unsigned char unknown_000[0x1E0];
    int result;
    unsigned char unknown_1e4[0x4F4];
    short *surface;
} CombPlayerRenderStateView;

void func_8005FB48(int mode, unsigned int mask);

int func_8005FC1C(int course, int value, int difference, int penalty);

typedef struct CombSetupValues {
    unsigned int word_00, word_04, word_08;
} CombSetupValues;

typedef struct CombSetupLink {
    unsigned char unknown_00[0x1C];
    int value;
    unsigned char unknown_20[0x28];
    int *progress;
} CombSetupLink;

typedef struct CombSetupSource {
    unsigned char unknown_0000[0x70];
    CombSetupValues values;
    unsigned int word_0c;
    unsigned char unknown_0080[0x1200];
    CombSetupLink *link;
    short value;
} CombSetupSource;

typedef struct CombSetupState {
    CombSetupValues values;
    unsigned int word_0c;
    unsigned char unknown_0010[0x4DC];
    short value;
    unsigned char unknown_04ee[2];
    CombSetupLink *link;
} CombSetupState;

void func_8006009C(CombSetupState *state, CombSetupSource *source, int *progress);

typedef struct CombPlayerCheckpointView {
    unsigned char unknown_0000[0x7E0];
    int checkpoint_reset;
    int comparison_state;
    int time_difference;
    unsigned char unknown_07ec[0x4CA];
    short checkpoint;
    short previous_checkpoint;
    unsigned char unknown_0cba[2];
    int checkpoint_value;
    unsigned char unknown_0cc0[0x12];
    short finished_value;
    unsigned char unknown_0cd4[0x13C];
    int checkpoint_times[4];
    unsigned char unknown_0e20[0x3AFC];
    short side;
} CombPlayerCheckpointView;

unsigned int func_800600E8(unsigned int value, int time);

int func_800605A8(void);

int func_80060660(CombPlayerCheckpointView *player);

void func_800606F0(void);

void func_80060728(void);

typedef struct CombMotionSample {
    int x, y, z;
    SVECTOR rotation;
    int active;
} CombMotionSample;

typedef struct CombMotionState {
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
    CombMotionSample samples[4];
    int selected;
    int word_190;
    int word_194;
    int word_198;
    int word_19c;
    int word_1a0;
} CombMotionState;

void func_800607B4(CombMotionState *state, short mode, int value, int x, int y, int z);

void func_80060BD8(CombMotionState *state, int *position);

int func_80060C24(CombMotionState *state, int sample, int target);

typedef struct CombMotionTransform {
    unsigned char unknown_000[0x2C];
    SVECTOR rotation;
    GsCOORDINATE2 coordinate;
} CombMotionTransform;

typedef struct CombMotionPlayer {
    unsigned char unknown_000[0x2C];
    int position[3];
    unsigned char unknown_038[4];
    short height;
    short airborne;
    unsigned char unknown_040[0x19A];
    short heading;
    unsigned char unknown_1dc[0x4FC];
    CombMotionState *motion;
    unsigned char unknown_6dc[0x28];
    int progress;
    unsigned char unknown_708[0x544];
    short reverse;
    unsigned char unknown_c4e[0x3C96];
    int vertical_speed;
} CombMotionPlayer;

void func_80061A64(CombMotionPlayer *player, int *position);

void func_80061AB0(CombMotionPlayer *player, int *position);

#endif
