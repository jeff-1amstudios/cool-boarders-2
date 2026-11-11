#ifndef CB2_MAIN_H
#define CB2_MAIN_H

/* Add reviewed layouts as fresh Ghidra-based reconstructions are accepted. */

typedef struct CdlLOC {
    unsigned char minute;
    unsigned char second;
    unsigned char sector;
    unsigned char track;
} CdlLOC;

typedef struct CdlFILE {
    CdlLOC pos;
    unsigned long size;
    char name[16];
} CdlFILE;

typedef void (*CdlCB)(unsigned char status, unsigned char *result);

typedef struct OverlayFileIndices {
    int entries[14];
} OverlayFileIndices;

typedef struct OverlayHeapStarts {
    char *entries[14];
} OverlayHeapStarts;

typedef struct RECT {
    short x, y, w, h;
} RECT;

typedef struct DR_ENV {
    unsigned long tag;
    unsigned long code[15];
} DR_ENV;

typedef struct DRAWENV {
    RECT clip;
    short ofs[2];
    RECT tw;
    unsigned short tpage;
    unsigned char dtd, dfe;
    unsigned char isbg;
    unsigned char r0, g0, b0;
    DR_ENV dr_env;
} DRAWENV;

typedef struct DISPENV {
    RECT disp;
    RECT screen;
    unsigned char isinter;
    unsigned char isrgb24;
    unsigned char pad0, pad1;
} DISPENV;

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

typedef struct SPRT {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short w, h;
} SPRT;

typedef struct SpriteTexture {
    unsigned char u, v;
    short w, h;
    unsigned short clut;
    unsigned short tpage;
} SpriteTexture;

typedef struct CVECTOR {
    unsigned char r, g, b, cd;
} CVECTOR;

typedef struct SPRT_16 {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
} SPRT_16;

typedef struct Sprite16Texture {
    unsigned char u, v;
    unsigned short clut;
} Sprite16Texture;

typedef struct DR_TPAGE {
    unsigned long tag;
    unsigned long code[1];
} DR_TPAGE;

#endif
