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

#endif
