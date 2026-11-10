#ifndef CB2_MAIN_H
#define CB2_MAIN_H

#include "record_search.h"

/* Reconstructed layouts remain provisional; preserve their observed field sizes. */
typedef struct {
    int location;
    int size;
    char name[16];
} BootFile;

typedef struct {
    short x;
    short y;
    short width;
    short height;
} ScreenRect;

typedef struct {
    int entries[14];
} OverlayTable;

typedef struct {
    unsigned int words[23];
} StateRecord;

typedef struct {
    unsigned char bytes[20];
} DisplayRecord;

typedef struct {
    unsigned char bytes[8];
} RectangleRecord;

typedef struct {
    unsigned char settings[24];
    unsigned char clear_enabled;
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char commands[64];
} DrawingEnvironment;

typedef struct {
    short x;
    short y;
    short width;
    short height;
    short screen_x;
    short screen_y;
    short screen_width;
    short screen_height;
    unsigned char flags[4];
} DisplayEnvironment;

typedef struct {
    DrawingEnvironment drawing;
    DisplayEnvironment display;
    unsigned int unknown;
    void *packets;
    unsigned int remaining;
} GraphicsBuffer;

typedef struct {
    GraphicsBuffer first;
    GraphicsBuffer second;
} GraphicsBuffers;

typedef struct {
    unsigned int address : 24;
    unsigned int length : 8;
    unsigned char r, g, b, code;
    short x, y;
    unsigned char u, v;
    unsigned short clut;
    unsigned short width, height;
} SpritePacket;

typedef struct {
    unsigned char u, v;
    unsigned short width, height, clut;
} SpriteTexture;

typedef struct {
    unsigned char r, g, b;
} SpriteColor;

typedef struct {
    unsigned int address : 24;
    unsigned int length : 8;
    unsigned char r, g, b, code;
    short x, y;
    unsigned char u, v;
    unsigned short clut;
} Sprite16Packet;

typedef struct {
    unsigned char u, v;
    unsigned short clut;
} Sprite16Texture;

typedef struct {
    unsigned int address : 24;
    unsigned int length : 8;
    unsigned int command;
} DrawModePacket;

typedef struct {
    unsigned int address : 24;
    unsigned int length : 8;
    unsigned char r, g, b, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad2;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad3;
} RotatedQuadPacket;

typedef struct {
    unsigned char u, v;
    unsigned short width, height, clut, tpage;
} RotatedQuadTexture;

typedef struct {
    unsigned char r, g, b, semitransparent;
} RotatedQuadColor;

typedef struct { unsigned int word0; unsigned int word1; } WordPair;

/* Only the observed 16-byte prefix and two payload extents are modeled. */
struct Payload8001ADC8 {
    unsigned char unknown[16];
    unsigned short halfwords[256];
    unsigned char bytes[8192];
};

#endif
