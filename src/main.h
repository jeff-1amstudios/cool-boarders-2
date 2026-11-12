#ifndef CB2_MAIN_H
#define CB2_MAIN_H

#include "sdk.h"

/* Add reviewed layouts as fresh Ghidra-based reconstructions are accepted. */

typedef struct OverlayFileIndices {
    int entries[14];
} OverlayFileIndices;

typedef struct OverlayHeapStarts {
    char *entries[14];
} OverlayHeapStarts;

typedef struct RenderBuffer {
    DRAWENV draw;
    DISPENV disp;
    unsigned char unknown_70[4];
    void *allocation;
    unsigned char unknown_78[4];
} RenderBuffer;

typedef struct PrimitiveTag {
    unsigned int address : 24;
    unsigned int length : 8;
} PrimitiveTag;

typedef struct SpriteTexture {
    unsigned char u, v;
    short w, h;
    unsigned short clut;
    unsigned short tpage;
} SpriteTexture;

typedef struct Sprite16Texture {
    unsigned char u, v;
    unsigned short clut;
} Sprite16Texture;

typedef struct DiscFileRecord {
    CdlLOC pos;
    unsigned long size;
} DiscFileRecord;

typedef struct FileReadRequest {
    char *cursor;
    char *destination;
    char **completion;
    unsigned int flags;
    int file_index;
    unsigned long size;
    int sector;
    unsigned long remaining;
} FileReadRequest;

/* Six word fields read by func_800143E0; semantic names remain uncertain. */
typedef struct ModeDisplayData {
    int index;
    int variant;
    int value_08;
    int value_0c;
    int value_10;
    int value_14;
} ModeDisplayData;

typedef struct SoundBankDescriptor {
    unsigned long *offsets;
    int sample_count;
    unsigned long pitch;
    int unknown_0c;
} SoundBankDescriptor;

typedef struct SoundDiscTransfer {
    short bank_index;
    unsigned char voice;
    unsigned char transfer_pending;
    CdlLOC positions[2];
    unsigned long sizes[2];
    int sector;
    unsigned long remaining;
    unsigned char data[2048];
} SoundDiscTransfer;

typedef struct SoundTransferBank {
    short bank_index;
    unsigned char unknown_02[2];
    unsigned long transfer_start;
    unsigned long size;
    unsigned long *offsets;
    int sample_count;
} SoundTransferBank;

/* File-table entries overlap the descriptor view starting at the next word. */
typedef struct SoundBankFileRecord {
    int file_index;
    unsigned long *offsets;
    int sample_count;
    unsigned long pitch;
} SoundBankFileRecord;

#endif
