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

/* Queue commands: 0/1 select automatic voices, larger values are voice masks. */
typedef struct SoundCommand {
    unsigned long command;
    unsigned char sample_index;
    unsigned char volume;
    unsigned short pitch;
} SoundCommand;

typedef struct CardResultTable {
    int entries[5];
} CardResultTable;

typedef struct CardLoadResultTable {
    int entries[4];
} CardLoadResultTable;

typedef struct CardMessageColors {
    CVECTOR normal;
    CVECTOR error;
} CardMessageColors;

/* Serialized save header; data_offset points beyond this 28-byte prefix. */
typedef struct CardSaveHeader {
    unsigned char signature[8];
    unsigned int checksum;
    unsigned int data_offset;
    unsigned char unknown_10[12];
} CardSaveHeader;

/* Word view of the serialized tail retained as bytes in CardSaveHeader. */
typedef struct CardSaveHeaderTail {
    unsigned int unknown_10;
    unsigned int data_checksum;
    unsigned int unknown_18;
} CardSaveHeaderTail;

typedef struct CardSaveBlock {
    unsigned char card_header[512];
    CardSaveHeader header;
    unsigned char payload[7652];
} CardSaveBlock;

/* Byte-aligned assets copied into the memory-card header workspace. */
typedef struct CardSaveTitle {
    char text[65];
} CardSaveTitle;

typedef struct CardSavePalette {
    unsigned char bytes[32];
} CardSavePalette;

typedef struct CardSaveFrames {
    unsigned char bytes[384];
} CardSaveFrames;

/* Serialized settings retain halfword alignment and their observed copy extents. */
typedef struct CardGameSettings {
    short values[13];
} CardGameSettings;

typedef struct CardAudioSettings {
    short cd_volume;
    short effect_volume;
    short music_volume;
    short saved_cd_volume;
    unsigned short pitch;
} CardAudioSettings;

typedef struct CardGameRecord {
    unsigned int words[48];
} CardGameRecord;

typedef struct CardGameResult {
    unsigned int words[26];
} CardGameResult;

typedef struct CardGameProgress {
    unsigned int words[27];
} CardGameProgress;

typedef struct CardGameEntry {
    unsigned int words[4];
} CardGameEntry;

typedef struct CardGamePayload {
    unsigned int size;
    CardGameSettings settings;
    CardAudioSettings audio;
    CardGameSettings options;
    CardGameRecord records[10];
    CardGameResult results[3];
    CardGameProgress progress;
    CardGameEntry entries[3];
} CardGamePayload;

/* Read window after the 512-byte memory-card header. */
typedef struct CardLoadBlock {
    CardSaveHeader header;
    CardGamePayload payload;
    unsigned char trailing[204];
} CardLoadBlock;

/* Secondary save payload validated and imported by func_800195F8. */
typedef struct CardExtraPayload {
    unsigned int size;
    short selected_record;
    unsigned short settings[7];
    unsigned short record_values[8];
    unsigned short record_data[8][10];
} CardExtraPayload;

/* The secondary payload occupies the existing load window's trailing bytes. */
typedef struct CardExtraLoadBlock {
    CardSaveHeader header;
    CardGamePayload payload;
    CardExtraPayload extra;
    unsigned char trailing[8];
} CardExtraLoadBlock;

/* Byte-aligned filename template copied before substituting the ghost slot. */
typedef struct CardGhostFilename {
    char text[24];
} CardGhostFilename;

/* Observed prefix of the variable-length ghost payload. */
typedef struct CardGhostPayload {
    unsigned char unknown_00[10];
    short title_index;
} CardGhostPayload;

typedef struct CardGhostSaveBlock {
    unsigned char card_header[256];
    unsigned char signature[8];
    unsigned int checksum;
    unsigned int data_offset;
    unsigned int data_size;
    unsigned int data_checksum;
    unsigned char payload[16104];
} CardGhostSaveBlock;

/* One 16x16 four-bit memory-card icon. */
typedef struct CardGhostFrame {
    unsigned char bytes[128];
} CardGhostFrame;

typedef struct CardGhostAssetPointers {
    unsigned char *entries[7];
} CardGhostAssetPointers;

/* Serialized ghost header embedded at byte 256 of CardGhostSaveBlock. */
typedef struct CardGhostHeader {
    unsigned char signature[8];
    unsigned int checksum;
    unsigned int data_offset;
    unsigned int data_size;
    unsigned int data_checksum;
} CardGhostHeader;

/* Initial ghost-file read: 24-byte header plus the remaining read window. */
typedef struct CardGhostReadWindow {
    CardGhostHeader header;
    unsigned char remaining[104];
} CardGhostReadWindow;

/* Four payload words returned while inspecting a ghost slot. */
typedef struct CardGhostSummary {
    unsigned int words[4];
} CardGhostSummary;

/* Board save record at file offset 512, followed by its checksum region. */
typedef struct CardBoardRecord {
    unsigned char signature[8];
    int selected_record;
    unsigned int checksum;
    unsigned short values[256];
    unsigned char data[8192];
} CardBoardRecord;

typedef struct CardBoardSaveBlock {
    unsigned char card_header[512];
    CardBoardRecord record;
    unsigned char remaining[7152];
} CardBoardSaveBlock;

/* Sixteen-word state cleared here; asset fields receive CD completion pointers. */
typedef struct FileLoadState {
    int active;
    int unknown_04;
    char *file_5c;
    char *file_13;
    char *file_14;
    int mode;
    char *file_17;
    char *file_18;
    unsigned int unknown_20[8];
} FileLoadState;

/* Eight signed file indices copied as one halfword-aligned table. */
typedef struct AssetFileIndices {
    short entries[8];
} AssetFileIndices;

/* Completion destinations for the seven assets queued by func_8001C3C4. */
typedef struct AssetLoadState {
    int active;
    int unknown_04;
    char *assets[7];
    unsigned int unknown_24[7];
} AssetLoadState;

#endif
