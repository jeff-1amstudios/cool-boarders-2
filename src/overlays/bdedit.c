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

extern BdEditTextureChoices D_80062E28;
extern SpriteTexture D_80062E48;
extern SPRT *func_80012950(unsigned long *ot, SPRT *sprite, short x, short y,
                          SpriteTexture *texture, unsigned char shade);
extern DR_TPAGE *func_80012B6C(unsigned long *ot, DR_TPAGE *packet, unsigned int tpage);

extern short D_800631E8;
extern short D_800631EC;

extern int D_800631F8;
extern void func_8005F634(unsigned short *packed, CVECTOR *palette, int count);
extern void func_8005D8C4(unsigned short *packed);

extern CVECTOR D_80062E6C;
extern unsigned char *func_8005EDD8(unsigned long *ot, unsigned char *packet,
                                  int x, int y, int w, int h, CVECTOR *color);
extern unsigned char *func_8005F77C(unsigned long *ot, unsigned char *packet,
                                  int x, int y, int value, int digits, int flags);
extern unsigned char *func_8005EEF8(unsigned long *ot, unsigned char *packet,
                                  int x, int y, int w, int h, CVECTOR *color, int flags);
extern unsigned char *func_8005F238(unsigned long *ot, unsigned char *packet,
                                  int x, int y, int w, int h,
                                  CVECTOR *border, CVECTOR *fill, int flags);

extern int func_80015B70(int sample, int volume);
extern void func_8005DB4C(int *cursor, CVECTOR *palette);
extern int D_800631F0;
extern int D_800631F4;

extern BdEditMenuTextures D_80062E70;

extern BdEditPixelMasks D_80062E20;
extern void func_8005F6A4(CVECTOR *palette, unsigned short *packed, int count);
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

// FUNCTION: BDEDIT.BIN:0x8005C7CC
unsigned char *func_8005C7CC(unsigned long *ot, unsigned char *packet,
                          int active, int value, int flags)
{
    LINE_F3 *lines[2];
    unsigned long *prim = (unsigned long *)packet;
    BdEditTextureChoices choices = D_80062E28;
    SpriteTexture background = D_80062E48;
    SpriteTexture texture;
    int brightness;
    int index;
    int x;

    lines[0] = (LINE_F3 *)prim;
    prim += sizeof(LINE_F3) / sizeof(*prim);
    lines[1] = (LINE_F3 *)prim;
    ((PrimitiveTag *)lines[0])->length = 5;
    lines[0]->code = 0x48;
    lines[0]->pad = 0x55555555;
    ((PrimitiveTag *)lines[1])->length = 5;
    lines[1]->code = 0x48;
    lines[1]->pad = 0x55555555;
    lines[0]->x0 = lines[1]->x0 = lines[1]->x1 = D_800631FC + 31;
    prim += sizeof(LINE_F3) / sizeof(*prim);
    lines[0]->x1 = lines[0]->x2 = lines[1]->x2 = lines[0]->x0 + 41;
    lines[0]->y0 = lines[0]->y1 = lines[1]->y0 = 23;
    lines[0]->y2 = lines[1]->y1 = lines[1]->y2 = 56;
    if (active != 0) {
        lines[0]->r0 = lines[0]->g0 = lines[0]->b0 =
            lines[1]->r0 = lines[1]->g0 = lines[1]->b0 = abs((D_80047DC0 & 31) - 16) * 8 + 96;
    } else {
        lines[0]->r0 = lines[0]->g0 = lines[0]->b0 =
            lines[1]->r0 = lines[1]->g0 = lines[1]->b0 = 192;
    }
    ((PrimitiveTag *)lines[0])->address = ((PrimitiveTag *)ot)->address;
    ((PrimitiveTag *)ot)->address = (unsigned long)lines[0];
    brightness = abs((D_80047DC0 & 63) - 32) + 28;
    ((PrimitiveTag *)lines[1])->address = ((PrimitiveTag *)ot)->address;
    ((PrimitiveTag *)ot)->address = (unsigned long)lines[1];
    index = 0;
    do {
        x = index * 128 + 32;
        if ((flags & 2) == 0) {
            texture = choices.entries[value];
            if (index != 0) texture.v += 32;
            prim = (unsigned long *)((SPRT *(*)(unsigned long *, SPRT *, int, int, SpriteTexture *, unsigned char))func_80012950)(ot, (SPRT *)prim, x, 24, &texture, brightness);
            prim = (unsigned long *)func_80012B6C(ot, (DR_TPAGE *)prim, choices.entries[value].tpage);
        }
        background.v = (index * 32) % 256;
        prim = (unsigned long *)((SPRT *(*)(unsigned long *, SPRT *, int, int, SpriteTexture *, unsigned char))func_80012950)(ot, (SPRT *)prim, x, 24, &background, 128);
        prim = (unsigned long *)func_80012B6C(ot, (DR_TPAGE *)prim, background.tpage);
        index++;
    } while (index < 2);
    return (unsigned char *)prim;
}

// FUNCTION: BDEDIT.BIN:0x8005CB9C
void func_8005CB9C(int *cursor, unsigned int buttons)
{
    int column;
    int row;
    int value;

    if ((D_800631EC < 2 || (buttons & 8) != 0) && (buttons & 0xF000) != 0) {
        D_800631E8 = 0;
        if (D_800631EC != 0) {
            D_800631EC = 4;
        } else {
            D_800631EC = 11;
        }
        row = (((short *)cursor)[3] - 63) / 4;
        if (buttons & 0x1000) row--;
        if (buttons & 0x4000) row++;
        row = row < 0 ? 0 : row >= 32 ? 31 : row;
        column = (((short *)cursor)[1] - 106) / 4;
        if (buttons & 0x8000) {
            if (column > 0) {
                column--;
            } else if (D_800631FC > 0) {
                D_800631FC--;
                column = 0;
            }
        }
        if (buttons & 0x2000) {
            if (column < 39) {
                column++;
            } else if (D_800631FC < 216) {
                D_800631FC++;
                column = 39;
            }
        }
        value = column < 0 ? 0 : column >= 40 ? 39 : column;
        cursor[0] = (value * 4 + 109) << 16;
        cursor[1] = (row * 4 + 66) << 16;
    }
}

// FUNCTION: BDEDIT.BIN:0x8005CF38
void func_8005CF38(short *cursor, unsigned int buttons)
{
    int position;

    if ((buttons & 0x400000) != 0) {
        position = cursor[1] - 32;
        if (position < 20) {
            D_800631FC = 0;
        } else {
            D_800631FC = position > 236 ? 216 : cursor[1] - 52;
        }
    }
}

// FUNCTION: BDEDIT.BIN:0x8005CF8C
void func_8005CF8C(CVECTOR *palette, unsigned short *packed)
{
    int index;
    CVECTOR *color;

    D_800631F8 = 7;
    for (index = 0; index < 64; index++) {
        color = &palette[index];
        if ((index & 4) > 0) {
            color->g = (8 - index / 8) * 255 / 8;
        } else {
            color->g = 0;
        }
        if ((index & 2) > 0) {
            color->r = (8 - index / 8) * 255 / 8;
        } else {
            color->r = 0;
        }
        if ((index & 1) > 0) {
            color->b = (8 - index / 8) * 255 / 8;
        } else {
            color->b = 0;
        }
        color->cd = 1;
    }
    for (index = 64; index < 256; index++) {
        color = &palette[index];
        color->r = color->g = color->b = color->cd = 0;
    }
    func_8005F634(packed, palette, 256);
    func_8005D8C4(packed);
}

// FUNCTION: BDEDIT.BIN:0x8005D194
void func_8005D194(unsigned short *packed)
{
    unsigned short *palette = (unsigned short *)0x801F1E00;
    int index;

    for (index = 0; index < 64; index++) {
        palette[index] = packed[index] | 0x8000;
    }
    for (index = 64; index < 255; index++) {
        palette[index] = packed[index] & 0x7F;
    }
    palette[255] = 0;
}

// FUNCTION: BDEDIT.BIN:0x8005D200
unsigned char *func_8005D200(unsigned long *ot, unsigned char *packet,
                          int x, int y, CVECTOR *palette, int selection)
{
    CVECTOR fill = D_80062E6C;
    CVECTOR border = D_80062E14;
    CVECTOR highlight;
    int row, column;
    int dx, dy;
    TILE *tile;
    LINE_F2 *line;
    POLY_G4 *bar;
    CVECTOR *color;

    if (selection == 1) {
        highlight.r = highlight.g = highlight.b = abs((D_80047DC0 & 31) - 16) * 8 + 96;
    } else {
        highlight.r = highlight.g = highlight.b = 192;
    }
    packet = func_8005EDD8(ot, packet, (D_800631F8 % 8) * 8 + 24,
                          (D_800631F8 / 8) * 8 + 68, 7, 7, &highlight);
    row = 0;
    do {
        dy = (row + 1) * 8 - 4;
        column = 0;
        do {
            dx = column * 8 + 8;
            color = (CVECTOR *)((row * 8 + column) * 4 + (int)palette);
            tile = (TILE *)packet;
            packet += sizeof(TILE);
            ((PrimitiveTag *)tile)->length = 3;
            tile->code = 0x60;
            tile->w = 8;
            tile->h = 8;
            tile->x0 = x + dx;
            tile->y0 = y + dy;
            tile->r0 = color->r;
            tile->g0 = color->g;
            tile->b0 = color->b;
            ((PrimitiveTag *)tile)->address = ((PrimitiveTag *)ot)->address;
            ((PrimitiveTag *)ot)->address = (unsigned long)tile;
            column++;
        } while (column < 8);
        row++;
    } while (row < 8);
    row = 0;
    do {
        line = (LINE_F2 *)packet;
        packet += sizeof(LINE_F2);
        ((PrimitiveTag *)line)->length = 3;
        line->code = 0x40;
        line->x0 = 30 + row * 20;
        line->x1 = 41 + row * 20;
        if (row + 2 == selection) {
            line->r0 = line->g0 = line->b0 = abs((D_80047DC0 & 31) - 16) * 8 + 96;
        } else {
            line->r0 = line->g0 = line->b0 = 255;
        }
        bar = (POLY_G4 *)packet;
        packet += sizeof(POLY_G4);
        ((PrimitiveTag *)bar)->length = 8;
        bar->code = 0x38;
        bar->x0 = bar->x2 = 32 + row * 20;
        bar->x1 = bar->x3 = 40 + row * 20;
        bar->y0 = bar->y1 = 136;
        bar->y2 = bar->y3 = 168;
        bar->r0 = bar->r1 = bar->g0 = bar->g1 = bar->b0 = bar->b1 =
        bar->r2 = bar->r3 = bar->g2 = bar->g3 = bar->b2 = bar->b3 = 0;
        switch (row) {
        case 0:
            bar->b0 = bar->b1 = 255;
            line->y0 = line->y1 = 167 - (palette[D_800631F8].b >> 3);
            break;
        case 1:
            bar->r0 = bar->r1 = 255;
            line->y0 = line->y1 = 167 - (palette[D_800631F8].r >> 3);
            break;
        case 2:
            bar->g0 = bar->g1 = 255;
            line->y0 = line->y1 = 167 - (palette[D_800631F8].g >> 3);
            break;
        }
        ((PrimitiveTag *)line)->address = ((PrimitiveTag *)ot)->address;
        ((PrimitiveTag *)ot)->address = (unsigned long)line;
        ((PrimitiveTag *)bar)->address = ((PrimitiveTag *)ot)->address;
        ((PrimitiveTag *)ot)->address = (unsigned long)bar;
        row++;
    } while (row < 3);
    packet = func_8005F77C(ot, packet, 28, 170, palette[D_800631F8].b >> 3, 2, 0);
    packet = func_8005F77C(ot, packet, 48, 170, palette[D_800631F8].r >> 3, 2, 0);
    packet = func_8005F77C(ot, packet, 68, 170, palette[D_800631F8].g >> 3, 2, 0);
    packet = func_8005EEF8(ot, packet, 24, 180, 64, 8, &palette[D_800631F8], 0);
    return func_8005F238(ot, packet, x, y, 80, 127, &border, &fill, 0);
}

// FUNCTION: BDEDIT.BIN:0x8005D764
void func_8005D764(int *cursor, unsigned int buttons, unsigned int pressed,
                   unsigned int repeated, CVECTOR *palette)
{
    int column;
    int row;

    if (pressed & 0x400000) {
        func_80015B70(7, 128);
        column = (((short *)cursor)[1] - 24) / 8;
        row = (((short *)cursor)[3] - 68) / 8;
        D_800631F8 = row * 8 + column;
    } else if ((repeated & 0x400040) == 0x40 && (buttons & 0xF000) == 0) {
        func_80015B70(7, 128);
        D_800631E8 = 0;
        cursor[0] = D_800631F0;
        cursor[1] = D_800631F4;
    } else if ((repeated & 0x10) != 0 && (buttons & 0xF000) == 0) {
        func_80015B70(2, 128);
        func_8005DB4C(cursor, palette);
    }
}

// FUNCTION: BDEDIT.BIN:0x8005D860
void func_8005D860(int *cursor)
{
    cursor[0] = (D_800631F8 % 8 * 8 + 28) << 16;
    cursor[1] = (D_800631F8 / 8 * 8 + 72) << 16;
}

// FUNCTION: BDEDIT.BIN:0x8005D8C4
void func_8005D8C4(unsigned short *packed)
{
    RECT rect;

    rect.x = 0x300;
    rect.y = 0x1F7;
    rect.w = 0x100;
    rect.h = 1;
    LoadImage(&rect, (unsigned long *)packed);
}

// FUNCTION: BDEDIT.BIN:0x8005D908
void func_8005D908(int *cursor, unsigned int buttons, void *unused,
                   CVECTOR *palette, unsigned short *packed)
{
    int channel;
    int level;
    RECT rect;

    if (D_800631EC < 2 && (buttons & 0x5000) != 0) {
        D_800631E8 = 0;
        if (D_800631EC == 0) {
            D_800631EC = 11;
        } else {
            D_800631EC = 4;
        }
        channel = (((short *)cursor)[1] - 26) / 20;
        switch (channel) {
        case 0:
            level = palette[D_800631F8].b >> 3;
            break;
        case 1:
            level = palette[D_800631F8].r >> 3;
            break;
        case 2:
        default:
            level = palette[D_800631F8].g >> 3;
            break;
        }
        if (buttons & 0x1000) {
            level++;
        }
        if (buttons & 0x4000) {
            level--;
        }
        level = level < 0 ? 0 : level > 31 ? 31 : level;
        cursor[1] = (167 - level) << 16;
        level = level * 255 / 31;
        switch (channel) {
        case 0:
            palette[D_800631F8].b = level;
            break;
        case 1:
            palette[D_800631F8].r = level;
            break;
        case 2:
            palette[D_800631F8].g = level;
            break;
        }
        func_8005F634(packed + D_800631F8, palette + D_800631F8, 1);
        rect.x = 0x300;
        rect.y = 0x1F7;
        rect.w = 0x100;
        rect.h = 1;
        LoadImage(&rect, (unsigned long *)packed);
    }
}

// FUNCTION: BDEDIT.BIN:0x8005DB4C
void func_8005DB4C(int *cursor, CVECTOR *palette)
{
    cursor[0] = 36 << 16;
    cursor[1] = (167 - (palette[D_800631F8].b >> 3)) << 16;
}

// FUNCTION: BDEDIT.BIN:0x8005DB84
void func_8005DB84(int *cursor, unsigned int buttons, unsigned int pressed,
                   unsigned int repeated, CVECTOR *palette, unsigned short *packed)
{
    int channel;
    int level;
    RECT rect;

    if (buttons & 0x400000) {
        channel = (((short *)cursor)[1] - 26) / 20;
        level = 167 - ((short *)cursor)[3];
        level = level * 255 / 31;
        switch (channel) {
        case 0:
            palette[D_800631F8].b = level;
            break;
        case 1:
            palette[D_800631F8].r = level;
            break;
        case 2:
            palette[D_800631F8].g = level;
            break;
        }
        func_8005F634(packed + D_800631F8, palette + D_800631F8, 1);
        rect.x = 0x300;
        rect.y = 0x1F7;
        rect.w = 0x100;
        rect.h = 1;
        LoadImage(&rect, (unsigned long *)packed);
    } else {
        if (buttons & 0xF000) goto move_channel;
        if ((repeated & 0x400040) == 0x40) {
            func_80015B70(7, 128);
            D_800631E8 = 0;
            cursor[0] = D_800631F0;
            cursor[1] = D_800631F4;
        }
    }
    if ((buttons & 0xF000) == 0 && (repeated & 0x10) != 0) {
        func_80015B70(2, 128);
        cursor[0] = (D_800631F8 % 8 * 8 + 28) << 16;
        cursor[1] = (D_800631F8 / 8 * 8 + 72) << 16;
    }
move_channel:
    if ((buttons & 0x5000) == 0 && (pressed & 0xA000) != 0) {
        channel = (((short *)cursor)[1] - 26) / 20;
        if (pressed & 0x8000) channel--;
        if (pressed & 0x2000) channel++;
        if (channel < 0) {
            channel = 0;
        } else if (channel >= 3) {
            channel = 2;
        } else {
            func_80015B70(0, 128);
        }
        cursor[0] = (channel * 20 + 36) << 16;
    }
}

// FUNCTION: BDEDIT.BIN:0x8005DE88
unsigned char *func_8005DE88(unsigned long *ot, unsigned char *packet, int selection)
{
    BdEditMenuTextures textures = D_80062E70;
    int item;

    item = 0;
    do {
        packet = (unsigned char *)((SPRT *(*)(unsigned long *, SPRT *, int, int,
                      SpriteTexture *, unsigned char))func_80012950)(ot, (SPRT *)packet, 278, item * 15 + 63,
                      &textures.entries[item][item == selection], 128);
        packet = (unsigned char *)func_80012B6C(ot, (DR_TPAGE *)packet,
                      textures.entries[item][item == selection].tpage);
        item++;
    } while (item < 9);
    return packet;
}

// FUNCTION: BDEDIT.BIN:0x8005E224
void func_8005E224(int selection, int *cursor)
{
    cursor[0] = 290 << 16;
    cursor[1] = (selection * 15 + 70) << 16;
}

// FUNCTION: BDEDIT.BIN:0x8005E244
void func_8005E244(int *cursor)
{
    D_800631E8 = 0;
    D_800631EC = 0;
    cursor[0] = 107 << 16;
    D_800631F0 = 107 << 16;
    cursor[1] = 64 << 16;
    D_800631F4 = 64 << 16;
    D_800631FC = 0;
}

// FUNCTION: BDEDIT.BIN:0x8005E6A4
unsigned char *func_8005E6A4(unsigned long *ot, unsigned char *packet, int x, int y)
{
    POLY_G4 *poly;

    if (D_800631E8 != 0) {
        poly = (POLY_G4 *)packet;
        packet += sizeof(POLY_G4);
        ((PrimitiveTag *)poly)->length = 8;
        poly->code = 0x38;
        poly->x0 = x + 16;
        poly->y0 = y + 10;
        poly->x1 = x;
        poly->y1 = y;
        poly->x2 = x + 10;
        poly->y2 = y + 10;
        poly->x3 = x + 10;
        poly->y3 = y + 16;
        poly->r0 = poly->g0 = poly->b0 = 64;
        poly->r1 = poly->g1 = poly->b1 = 255;
        poly->r2 = poly->g2 = poly->b2 = 192;
        poly->r3 = poly->g3 = poly->b3 = 64;
        ((PrimitiveTag *)poly)->address = ((PrimitiveTag *)ot)->address;
        ((PrimitiveTag *)ot)->address = (unsigned long)poly;
    }
    return packet;
}

// FUNCTION: BDEDIT.BIN:0x8005E784
unsigned char *func_8005E784(unsigned long *ot, unsigned char *packet, int *cursor)
{
    int x;
    int y;
    CVECTOR color;

    D_800631F0 = *(volatile int *)&cursor[0];
    D_800631F4 = *(volatile int *)&cursor[1];
    x = ((cursor[0] >> 16) - 106) / 4 * 4 + 106;
    y = ((cursor[1] >> 16) - 63) / 4 * 4 + 63;
    color.r = color.g = color.b = abs((D_80047DC0 & 31) - 16) * 8 + 96;
    return func_8005EDD8(ot, packet, x, y, 4, 4, &color);
}

// FUNCTION: BDEDIT.BIN:0x8005E840
unsigned char *func_8005E840(unsigned long *ot, unsigned char *packet, int *cursor)
{
    int x;
    int y;
    CVECTOR color;

    x = ((cursor[0] >> 16) - 278) / 24 * 24 + 277;
    y = ((cursor[1] >> 16) - 63) / 15 * 15 + 62;
    color.r = color.g = color.b = abs((D_80047DC0 & 31) - 16) * 8 + 96;
    return func_8005EDD8(ot, packet, x, y, 25, 16, &color);
}

// FUNCTION: BDEDIT.BIN:0x8005E9EC
void func_8005E9EC(unsigned char **planes)
{
    int row;
    int column;
    int opposite;
    unsigned short *pixel;
    unsigned short saved;

    row = 0;
    do {
        column = 0;
        do {
            pixel = (unsigned short *)((row * 64 + column) * 2 + (unsigned int)planes[0]);
            opposite = ((31 - row) * 64 + column) * 2;
            saved = *pixel;
            *pixel = *(unsigned short *)(opposite + (unsigned int)planes[1]);
            column++;
            *(unsigned short *)(opposite + (unsigned int)planes[1]) = saved;
        } while (column < 64);
        row++;
    } while (row < 32);
}

// FUNCTION: BDEDIT.BIN:0x8005EA68
void func_8005EA68(unsigned char **planes, short *cursor)
{
    unsigned char *pixels;
    unsigned char *row;
    unsigned char saved;
    int offset;
    int column;

    if (D_800631FC + (cursor[1] - 106) / 4 >= 128) {
        planes++;
    }
    pixels = *planes;
    offset = 0;
    row = pixels;
    do {
        saved = *row;
        column = 0;
        do {
            pixels[offset + column] = pixels[offset + column + 1];
            column++;
        } while (column < 127);
        row[127] = saved;
        offset += 128;
        row += 128;
    } while (offset < 4096);
}

// FUNCTION: BDEDIT.BIN:0x8005EAF0
void func_8005EAF0(unsigned char **planes, short *cursor)
{
    unsigned char *pixels;
    unsigned char *row;
    unsigned char saved;
    int offset;
    int column;

    if (D_800631FC + (cursor[1] - 106) / 4 >= 128) {
        planes++;
    }
    pixels = *planes;
    offset = 0;
    row = pixels;
    do {
        saved = row[127];
        column = 127;
        do {
            pixels[offset + column] = pixels[offset + column - 1];
            column--;
        } while (column > 0);
        *row = saved;
        offset += 128;
        row += 128;
    } while (offset < 4096);
}

// FUNCTION: BDEDIT.BIN:0x8005EB70
void func_8005EB70(unsigned char **planes, short *cursor)
{
    unsigned char *pixels;
    unsigned char *column;
    unsigned char *pixel;
    unsigned char saved;
    int x;
    int row;
    int offset;

    if (D_800631FC + (cursor[1] - 106) / 4 >= 128) {
        planes++;
    }
    pixels = *planes;
    x = 0;
    column = pixels;
    do {
        saved = *column;
        row = 0;
        do {
            offset = x + row * 128;
            pixel = pixels + offset;
            *pixel = pixel[128];
            row++;
        } while (row < 31);
        column[3968] = saved;
        x++;
        column++;
    } while (x < 128);
}

// FUNCTION: BDEDIT.BIN:0x8005EBF8
void func_8005EBF8(unsigned char **planes, short *cursor)
{
    unsigned char *pixels;
    unsigned char *column;
    unsigned char *pixel;
    unsigned char saved;
    int x;
    int row;
    int offset;

    if (D_800631FC + (cursor[1] - 106) / 4 >= 128) {
        planes++;
    }
    pixels = *planes;
    x = 0;
    column = pixels;
    do {
        saved = column[3968];
        row = 31;
        offset = x + 3968;
        do {
            pixel = pixels + offset;
            offset -= 128;
            row--;
            *pixel = pixel[-128];
        } while (row > 0);
        *column = saved;
        x++;
        column++;
    } while (x < 128);
}

// FUNCTION: BDEDIT.BIN:0x8005EC7C
void func_8005EC7C(unsigned short **planes, unsigned short *palette)
{
    int i;

    i = 0;
    do {
        planes[0][i] = 0;
        planes[1][i] = 0;
        i++;
    } while (i < 0x1000);

    i = 255;
    palette += 255;
    do {
        *palette = 0;
        i--;
        palette--;
    } while (i >= 0);
}

// FUNCTION: BDEDIT.BIN:0x8005C204
void func_8005C204(unsigned char **planes, unsigned char *packed)
{
    BdEditPixelMasks masks = D_80062E20;
    unsigned short *destination;
    unsigned short value;
    int plane;
    int pixel;
    unsigned char **current;

    destination = (unsigned short *)0x801F2000;
    plane = 0;
    current = planes;
    do {
        pixel = 0;
        do {
            *destination++ = ((unsigned short *)*current)[pixel] |
                             masks.values[*packed & 3];
            *destination++ = ((unsigned short *)*current)[pixel + 1] |
                             masks.values[(*packed >> 2) & 3];
            *destination++ = ((unsigned short *)*current)[pixel + 2] |
                             masks.values[(*packed >> 4) & 3];
            value = ((unsigned short *)*current)[pixel + 3];
            value |= masks.values[*packed >> 6];
            packed++;
            *destination++ = value;
            pixel += 4;
        } while (pixel < 0x800);
        plane++;
        current++;
    } while (plane < 2);
}

// FUNCTION: BDEDIT.BIN:0x8005D0F0
void func_8005D0F0(CVECTOR *palette, unsigned short *packed)
{
    unsigned short *base = (unsigned short *)0x801F1E00;
    int index;

    D_800631F8 = 0;
    for (index = 0; index < 64; index++) {
        packed[index] = base[index] | 0x8000;
    }
    for (index = 64; index < 256; index++) {
        packed[index] = base[index] & 0x7F;
    }
    func_8005F6A4(palette, packed, 256);
    func_8005D8C4(packed);
}
