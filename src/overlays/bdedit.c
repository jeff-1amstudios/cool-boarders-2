#include "bdedit.h"

extern void func_80015948(unsigned char *first_data, int first_bank,
                          unsigned char *second_data, int second_bank);
extern void func_8005AC10(void);
extern unsigned int func_8001B320(int port, unsigned char *state);
extern int func_8001C130(FileLoadState *state, short mode);
extern int D_80045370;
extern int D_8004536C;

extern CVECTOR D_80062E10;
extern CVECTOR D_80062E14;
extern unsigned char *func_8005DE88(unsigned long *ot, unsigned char *packet, int selection);
extern unsigned char *func_8005C374(unsigned long *ot, unsigned char *packet,
                                  int selection, short *cursor, unsigned char **planes,
                                  CVECTOR *palette, int mode, unsigned char *mask, int value9, int flags);
extern unsigned char *func_8005D200(unsigned long *ot, unsigned char *packet,
                                  int x, int y, CVECTOR *palette, int selection);
extern unsigned char *func_8005F4AC(unsigned long *ot, unsigned char *packet,
                                  int x, int y, int w, int h,
                                  CVECTOR *border, CVECTOR *fill, int flags);

extern void func_8005C328(unsigned char **planes, int plane);
extern int D_800631FC;

extern BdEditPixelMasks D_80062E18;

extern unsigned short D_80047DC0;
extern unsigned char *func_80060824(unsigned long *ot, unsigned char *packet,
                                  int offset, int x, int y, int brightness);
extern unsigned char *func_8006135C(unsigned long *ot, unsigned char *packet,
                                  int offset, int x, int y, int brightness);
extern unsigned char *func_8005C7CC(unsigned long *ot, unsigned char *packet,
                                  int active, int value, int flags);

// FUNCTION: BDEDIT.BIN:0x8005AB6C
int func_8005AB6C(FileLoadState *state)
{
    func_80015948((unsigned char *)state->file_5c, 0x41, 0, -1);
    func_8005AC10();
    if (D_80045370 != 0 && (func_8001B320(0, 0) & 0x401F) == 0x401F) {
        D_8004536C = rand() % 2 + 10;
    }
    func_8001C130(state, 1);
    return 3;
}

// FUNCTION: BDEDIT.BIN:0x8005BE94
unsigned char *func_8005BE94(unsigned long *ot, unsigned char *packet,
                          int selection, unsigned char **planes, CVECTOR *palette, short *cursor,
                          int mode, unsigned char *mask, int value9, int flags)
{
    CVECTOR fill = D_80062E10;
    CVECTOR border = D_80062E14;
    int item;

    packet = func_8005DE88(ot, packet, selection);
    packet = func_8005C374(ot, packet, selection, cursor, planes, palette,
                           mode, mask, value9, flags);
    if (mode != 2) {
        if (mode == 3) {
            item = (cursor[1] - 26) / 20 + 2;
        } else {
            item = 0;
        }
    } else {
        item = 1;
    }
    packet = func_8005D200(ot, packet, 16, 64, palette, item);
    return func_8005F4AC(ot, packet, 16, 201, 285, 16, &border, &fill, 0);
}

// FUNCTION: BDEDIT.BIN:0x8005C020
void func_8005C020(unsigned char **planes)
{
    int plane;
    int pixel;
    short fill;
    unsigned char **current;

    plane = 0;
    fill = 0x101;
    current = planes;
    do {
        pixel = 0;
        do {
            ((unsigned short *)*current)[pixel] = fill;
            pixel++;
        } while (pixel < 0x800);
        fill += 0x101;
        plane++;
        current++;
    } while (plane < 2);
    func_8005C328(planes, 0);
    func_8005C328(planes, 1);
    D_800631FC = 0;
}

// FUNCTION: BDEDIT.BIN:0x8005C0A8
void func_8005C0A8(unsigned char **planes, unsigned char *packed)
{
    BdEditPixelMasks masks = D_80062E18;
    unsigned short *source;
    int plane;
    int pixel;
    unsigned short value;
    unsigned short *maskValues;
    unsigned char **current;

    source = (unsigned short *)0x801F2000;
    plane = 0;
    maskValues = masks.values;
    current = planes;
    do {
        pixel = 0;
        do {
            value = *source++;
            value &= maskValues[*packed & 3];
            ((unsigned short *)*current)[pixel] = value;
            value = *source++;
            value &= maskValues[(*packed >> 2) & 3];
            ((unsigned short *)*current)[pixel + 1] = value;
            value = *source++;
            value &= maskValues[(*packed >> 4) & 3];
            ((unsigned short *)*current)[pixel + 2] = value;
            value = *source++;
            value &= maskValues[*packed++ >> 6];
            ((unsigned short *)*current)[pixel + 3] = value;
            pixel += 4;
        } while (pixel < 0x800);
        plane++;
        current++;
    } while (plane < 2);
    func_8005C328(planes, 0);
    func_8005C328(planes, 1);
    D_800631FC = 0;
}

// FUNCTION: BDEDIT.BIN:0x8005C328
void func_8005C328(unsigned char **planes, int plane)
{
    RECT rect;

    rect.x = 0x300;
    rect.y = (short)(plane << 5);
    rect.w = 0x40;
    rect.h = 0x20;
    LoadImage(&rect, (unsigned long *)planes[plane]);
}

// FUNCTION: BDEDIT.BIN:0x8005C374
unsigned char *func_8005C374(unsigned long *ot, unsigned char *packet,
                          int selection, short *cursor, unsigned char **planes,
                          CVECTOR *palette, int mode, unsigned char *mask, int value9, int flags)
{
    unsigned long *prim = (unsigned long *)packet;
    unsigned char *maskPixel;
    int column, row;
    int x, y;
    int pixel, plane, offset;
    int brightness;
    unsigned char *localPlanes[2];
    unsigned char index;
    CVECTOR *color;
    LINE_F2 *line;
    TILE *tile;

    if ((flags & 1) == 0) {
        column = 0;
        do {
            line = (LINE_F2 *)prim;
            ((PrimitiveTag *)line)->length = 3;
            line->code = 0x40;
            line->x0 = line->x1 = 106 + column * 4;
            line->y0 = 63;
            line->y1 = 191;
            line->r0 = line->g0 = line->b0 = 64;
            ((PrimitiveTag *)line)->address = ((PrimitiveTag *)ot)->address;
            ((PrimitiveTag *)ot)->address = (unsigned long)line;
            prim += sizeof(LINE_F2) / sizeof(*prim);
            column++;
        } while (column < 41);
        column = 0;
        do {
            line = (LINE_F2 *)prim;
            ((PrimitiveTag *)line)->length = 3;
            line->code = 0x40;
            line->x0 = 106;
            line->x1 = 266;
            line->y0 = line->y1 = 63 + column * 4;
            line->r0 = line->g0 = line->b0 = 64;
            ((PrimitiveTag *)line)->address = ((PrimitiveTag *)ot)->address;
            ((PrimitiveTag *)ot)->address = (unsigned long)line;
            prim += sizeof(LINE_F2) / sizeof(*prim);
            column++;
        } while (column < 33);
    }
    if (mode == 1) {
        x = (cursor[1] - 106) / 4;
        y = (cursor[3] - 63) / 4;
        if (selection == 1) {
            prim = (unsigned long *)func_80060824(ot, (unsigned char *)prim, D_800631FC, x, y, abs((D_80047DC0 & 31) - 16) * 8 + 96);
        } else if ((0 < selection) && (selection < mode + 3)) {
            prim = (unsigned long *)func_8006135C(ot, (unsigned char *)prim, D_800631FC, x, y, abs((D_80047DC0 & 31) - 16) * 8 + 96);
        }
    }
    x = 106;
    column = 0;
    localPlanes[0] = planes[0];
    localPlanes[1] = planes[1];
    do {
        pixel = D_800631FC + column;
        y = 63;
        if (pixel >= 128) {
            pixel -= 128;
            plane = 1;
        } else {
            plane = 0;
        }
        offset = pixel;
        if (plane != 0) maskPixel = mask + (offset / 8 + 512);
        else maskPixel = mask + offset / 8;
        row = 0;
        do {
            tile = (TILE *)prim;
            prim += sizeof(TILE) / sizeof(*prim);
            ((PrimitiveTag *)tile)->length = 3;
            tile->code = 0x60;
            tile->w = tile->h = 4;
            tile->x0 = x;
            tile->y0 = y;
            index = localPlanes[plane][offset];
            if ((flags & 2) == 0 && ((*maskPixel >> (pixel % 8)) & 1)) {
                brightness = (D_80047DC0 & 63) - 32;
                if (brightness < 0) brightness = -brightness;
                tile->r0 = tile->g0 = tile->b0 = brightness * 2 + 48;
            } else {
                color = (CVECTOR *)(index * sizeof(CVECTOR) + (unsigned long)palette);
                tile->r0 = color->r;
                tile->g0 = color->g;
                tile->b0 = color->b;
            }
            ((PrimitiveTag *)tile)->address = ((PrimitiveTag *)ot)->address;
            ((PrimitiveTag *)ot)->address = (unsigned long)tile;
            offset += 128;
            y += 4;
            row++;
            maskPixel += 16;
        } while (row < 32);
        column++;
        x += 4;
    } while (column < 40);
    return func_8005C7CC(ot, (unsigned char *)prim, mode == 4, value9, flags);
}
