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

#endif
