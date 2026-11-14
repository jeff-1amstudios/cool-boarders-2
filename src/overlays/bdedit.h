#ifndef CB2_BDEDIT_H
#define CB2_BDEDIT_H

#include "main.h"

int func_8005AB6C(FileLoadState *state);

unsigned char *func_8005BE94(unsigned long *ot, unsigned char *packet,
                          int selection, unsigned char **planes,
                          CVECTOR *palette, short *cursor,
                          int mode, unsigned char *mask, int value9, int flags);

void func_8005C020(unsigned char **planes);

typedef struct BdEditPixelMasks {
    unsigned short values[4];
} BdEditPixelMasks;

void func_8005C0A8(unsigned char **planes, unsigned char *packed);

typedef struct BdEditTextureChoices {
    SpriteTexture entries[3];
} BdEditTextureChoices;

void func_8005D860(int *cursor);

typedef struct BdEditMenuTextures {
    SpriteTexture entries[9][2];
} BdEditMenuTextures;

void func_8005C204(unsigned char **planes, unsigned char *packed);

typedef struct BdEditHitRegions {
    int entries[5][5];
} BdEditHitRegions;

typedef struct BdEditFrameTextures {
    SpriteTexture entries[5];
} BdEditFrameTextures;

typedef struct BdEditFramePositions {
    int entries[5][2];
} BdEditFramePositions;

typedef struct BdEditCapPositions {
    int entries[2][2];
} BdEditCapPositions;

typedef struct BdEditMaskPositions {
    int entries[3][2];
} BdEditMaskPositions;

void func_8005FB54(unsigned char *mask);

typedef union BdEditPrimitiveLink {
    PrimitiveTag tag;
    unsigned long word;
} BdEditPrimitiveLink;

unsigned char *func_8006207C(unsigned long *ot, unsigned char *packet, void *unused,
                            unsigned int buttons, unsigned int pressed, int *state,
                            unsigned char **planes, CVECTOR *palette,
                            unsigned short *packed);

typedef struct BdEditMenuColors {
    CVECTOR entries[3];
} BdEditMenuColors;

typedef struct BdEditBoardPositions {
    int entries[4][2];
} BdEditBoardPositions;

typedef struct BdEditBoardLabels {
    unsigned char *entries[4];
} BdEditBoardLabels;

#endif
