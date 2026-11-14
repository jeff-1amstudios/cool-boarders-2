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
extern BdEditHitRegions D_80062FC4;
extern unsigned int func_8001AE14(short *results);
extern void *func_8001F1A0(unsigned int size);
extern int func_8001A900(unsigned int slot, short selected_record,
                         unsigned short *values, unsigned char *data);
extern int func_8001F378(void *allocation);
extern int func_80017DE8(void);
extern char D_80062C44[16];
extern char D_80062C54[30];
extern char D_80062C74[19];
extern char D_80062C88[33];
extern char D_80062CAC[28];
extern char D_80062CC8[14];
extern char D_80062CD8[17];
extern char D_80062CEC[34];
extern char D_80062D10[13];
extern char D_80062D20[8];
extern char D_80062D28[30];
extern char D_80062D48[18];
extern char D_80062D5C[16];
extern char D_80062D6C[14];
extern int func_8001AFD0(int slot, unsigned short *values, unsigned char *data);
extern char bdedit_now_loading[15];
extern char bdedit_load_successful[17];
extern char bdedit_no_data[9];
extern void func_8005E224(int tool, int *cursor);
extern int func_8005FF24(int x, int y, unsigned int buttons, unsigned int previous,
                       unsigned char **planes, int *color, unsigned char *mask);
extern int func_80060108(int x, int y, unsigned int buttons, unsigned int previous,
                       unsigned char **planes, int *color, unsigned char *mask);
extern int func_80060E6C(int x, int y, unsigned int buttons, unsigned int previous,
                       unsigned char **planes, int *color, unsigned char *mask);
extern int func_8006177C(int x, int y, unsigned int buttons, unsigned int previous,
                       unsigned char **planes, int *color, unsigned char *mask);
extern int func_80061AC0(int x, int y, unsigned int buttons, unsigned int previous,
                       unsigned char **planes, int *color, unsigned char *mask);
extern void func_8005FF08(void);
extern char D_80062F24[22];
extern char D_80062F3C[28];
extern char D_80062F58[27];
extern char D_80062F74[17];
extern void func_8005CB9C(int *cursor, unsigned int buttons);
extern void func_8005D908(int *cursor, unsigned int buttons, void *unused,
                          CVECTOR *palette, unsigned short *packed);
extern BdEditFrameTextures D_80063030;
extern BackgroundTextures D_80063064;
extern BdEditFramePositions D_80063078;
extern BdEditCapPositions D_800630A0;

extern int D_80063204;
extern int D_80063200;

extern void func_80060318(unsigned char **planes, int x0, int y0,
                          int x1, int y1, int color, unsigned char *mask);

extern void func_800603DC(unsigned char **planes, int x, int y,
                          int dx, int dy, int color, unsigned char *mask);
extern void func_800604F4(unsigned char **planes, int x, int y,
                          int dx, int dy, int color, unsigned char *mask);
extern void func_80060604(unsigned char **planes, int x, int y,
                          int dx, int dy, int color, unsigned char *mask);
extern void func_80060714(unsigned char **planes, int x, int y,
                          int dx, int dy, int color, unsigned char *mask);

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

// FUNCTION: BDEDIT.BIN:0x8005E90C
int func_8005E90C(int *cursor)
{
    int result = -1;
    BdEditHitRegions regions = D_80062FC4;
    int i;
    int x = ((short *)cursor)[1];
    int y = ((short *)cursor)[3];
    int *region;

    for (i = 0; i < 5; i++) {
        region = regions.entries[i];
        if (x >= region[0] && x < region[1] &&
            y >= region[2] && y < region[3]) {
            result = region[4];
            break;
        }
    }
    return result;
}

// FUNCTION: BDEDIT.BIN:0x8005B76C
int func_8005B76C(int *state, char **message, int *timer, unsigned int buttons,
                  unsigned short **boards, unsigned short *values,
                  int unused, int *selection)
{
    int result = 0;
    int status;
    unsigned short *data;
    int board;
    int sample;
    short card_results[8];

    switch (*state) {
    case 8:
        status = func_8001AE14(card_results);
        switch (status) {
        case -1:
            *message = D_80062C44;
            *timer = 240;
            *state = 4;
            break;
        case -2:
            *message = D_80062C54;
            *timer = 240;
            *state = 5;
            break;
        case -6:
            *message = D_80062C74;
            *timer = 240;
            *state = 4;
            break;
        default:
            if (status & 1) {
                *message = D_80062C88;
            } else {
                *message = D_80062CAC;
            }
            *state = 0;
            break;
        }
        break;
    case 0:
        if (buttons & 0x40) {
            *message = D_80062CC8;
            func_80015B70(1, 128);
            result = 2;
            *state = 1;
        } else if (buttons & 0x20) {
            *message = 0;
            func_80015B70(3, 128);
            *state = -1;
        }
        break;
    case 1:
        data = func_8001F1A0(0x2000);
        for (board = 0; board < 2; board++) {
            for (sample = 0; sample < 0x800; sample++) {
                data[board * 0x800 + sample] = boards[board][sample];
            }
        }
        values[255] = 0;
        status = func_8001A900(0, (short)(selection[1] * 4 + selection[2]),
                              values, (unsigned char *)data);
        func_8001F378(data);
        switch (status) {
        case 0:
            *message = D_80062CD8;
            *timer = 120;
            *state = 4;
            break;
        case -1:
            *message = D_80062C44;
            *timer = 240;
            *state = 4;
            break;
        case -2:
            *message = D_80062C54;
            *timer = 240;
            *state = 5;
            break;
        case -3:
            *message = D_80062CEC;
            *timer = 240;
            *state = 4;
            break;
        case -4:
            *message = D_80062D10;
            *timer = 240;
            *state = 4;
            break;
        case -6:
            *message = D_80062D20;
            *timer = 240;
            *state = 4;
            break;
        }
        break;
    case 4:
        if (*timer > 0) {
            --*timer;
        }
        if (buttons & 0x70) {
            func_80015B70(3, 128);
            *timer = 0;
        }
        if (*timer == 0) {
            *message = 0;
            *state = -1;
        }
        break;
    case 5:
        if (*timer > 0) {
            --*timer;
        }
        if (buttons & 0x70) {
            func_80015B70(3, 128);
            *timer = 0;
        }
        if (*timer == 0) {
            *message = D_80062D28;
            *state = 6;
        }
        break;
    case 6:
        if (buttons & 0x40) {
            *message = D_80062D48;
            func_80015B70(1, 128);
            result = 2;
            *state = 7;
        } else if (buttons & 0x20) {
            func_80015B70(3, 128);
            *message = D_80062D5C;
            *timer = 120;
            *state = 4;
        }
        break;
    case 7:
        if (func_80017DE8() < 0) {
            *message = D_80062D6C;
            *timer = 240;
            *state = 4;
        } else {
            result = 2;
            *message = D_80062CC8;
            *state = 1;
        }
        break;
    }
    return result;
}

// FUNCTION: BDEDIT.BIN:0x8005BBAC
int func_8005BBAC(int *state, char **message, int *timer, unsigned int buttons,
                  unsigned short **pixels, unsigned short *palette,
                  int unused, int *selection)
{
    unsigned char *data;
    unsigned short *values;
    int result = 0;
    int status;
    int i;
    int bank;

    switch (*state) {
    case 0:
        if (buttons & 0x40) {
            *message = bdedit_now_loading;
            func_80015B70(1, 0x80);
            *state = 2;
            result = 2;
        } else if (buttons & 0x20) {
            *message = 0;
            func_80015B70(3, 0x80);
            *state = -1;
        }
        break;
    case 2:
        data = func_8001F1A0(0x2000);
        values = func_8001F1A0(0x200);
        status = func_8001AFD0(0, values, data);
        if (status >= 0) {
            for (bank = 0; bank < 2; bank++) {
                for (i = 0; i < 0x800; i++) {
                    pixels[bank][i] = ((unsigned short *)data)[bank * 0x800 + i];
                }
            }
            for (bank = 0; bank < 0x40; bank++) {
                palette[bank] = values[bank] | 0x8000;
            }
            for (bank = 0x40; bank < 0xff; bank++) {
                palette[bank] = values[bank];
            }
            palette[0xff] = 0;
            selection[1] = status / 4;
            selection[2] = status % 4;
            *message = bdedit_load_successful;
            *timer = 0x78;
            *state = -1;
            result = -1;
        } else {
            switch (status) {
            case -1:
                *message = D_80062C44;
                break;
            case -5:
            case -2:
                *message = bdedit_no_data;
                break;
            case -4:
                *message = D_80062D10;
                break;
            case -6:
            case -3:
            default:
                *message = D_80062D20;
                break;
            }
            *timer = 0xf0;
            *state = 4;
            func_8001F378(data);
            func_8001F378(values);
        }
        break;
    case 4:
        if (*timer > 0) {
            --*timer;
        }
        if (buttons & 0x70) {
            func_80015B70(3, 0x80);
            *timer = 0;
        }
        if (*timer == 0) {
            *message = 0;
            *state = -1;
        }
        break;
    }
    return result;

}

// FUNCTION: BDEDIT.BIN:0x8005CD14
void func_8005CD14(int tool, short *cursor, unsigned int buttons,
                   unsigned int previous, unsigned int pressed,
                   unsigned char **planes, unsigned char *mask)
{
    int x;
    int y;
    int changed;
    RECT rect;

    if ((buttons & 0xF000) == 0) {
        if (pressed & 0x10) {
            func_80015B70(2, 0x80);
            func_8005D860((int *)cursor);
        } else if (pressed & 0x80) {
            func_80015B70(2, 0x80);
            func_8005E224(tool, (int *)cursor);
        }
    }
    x = (cursor[1] - 106) / 4 + D_800631FC;
    y = (cursor[3] - 63) / 4;
    switch (tool) {
    case 0:
        changed = func_8005FF24(x, y, buttons, previous, planes, &D_800631F8, mask);
        break;
    case 1:
        changed = func_80060108(x, y, buttons, previous, planes, &D_800631F8, mask);
        break;
    case 2:
        changed = func_80060E6C(x, y, buttons, previous, planes, &D_800631F8, mask);
        break;
    case 3:
        changed = func_8006177C(x, y, buttons, previous, planes, &D_800631F8, mask);
        break;
    case 4:
        changed = func_80061AC0(x, y, buttons, previous, planes, &D_800631F8, mask);
        break;
    default:
        changed = 0;
    }
    if (changed) {
        rect.x = 0x300;
        rect.y = 0;
        rect.w = 0x40;
        rect.h = 0x20;
        LoadImage(&rect, (unsigned long *)planes[0]);
        rect.x = 0x300;
        rect.y = 0x20;
        rect.w = 0x40;
        rect.h = 0x20;
        LoadImage(&rect, (unsigned long *)planes[1]);
    }
}

// FUNCTION: BDEDIT.BIN:0x8005E010
int func_8005E010(int selection, int *cursor, unsigned int *flags,
                  char **message, int *message_state, unsigned int buttons,
                  unsigned int pressed, unsigned int repeated)
{
    int item = (((short *)cursor)[3] - 63) / 15;
    char *text;
    int action = -1;

    if ((pressed & 0x400000) != 0 && item < 5) {
        action = item;
    } else if ((repeated & 0x400040) == 0x40 && (buttons & 0xF000) == 0) {
        D_800631E8 = 0;
        cursor[0] = D_800631F0;
        cursor[1] = D_800631F4;
        action = item;
    } else if ((repeated & 0x200020) == 0x20 && (buttons & 0xF000) == 0) {
        func_80015B70(3, 128);
        D_800631E8 = 0;
        cursor[0] = D_800631F0;
        cursor[1] = D_800631F4;
    }
    if (action >= 0) {
        selection = action;
        switch (selection) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            func_80015B70(7, 128);
            func_8005FF08();
            *message = 0;
            break;
        case 6:
            func_80015B70(5, 128);
            text = D_80062F24;
            selection |= 0x80;
            *message = text;
            *message_state = -1;
            *flags &= ~2;
            break;
        case 5:
            func_80015B70(5, 128);
            text = D_80062F3C;
            selection |= 0x80;
            *message = text;
            *message_state = -1;
            *flags &= ~2;
            break;
        case 7:
            func_80015B70(5, 128);
            text = D_80062F58;
            selection |= 0x80;
            *message = text;
            *message_state = -1;
            *flags &= ~2;
            break;
        case 8:
            func_80015B70(5, 128);
            selection |= 0x80;
            text = D_80062F74;
            *message = text;
            *message_state = -1;
            *flags &= ~2;
            break;
        }
    }
    return selection;
}

// FUNCTION: BDEDIT.BIN:0x8005E284
unsigned int func_8005E284(int *cursor, int mode, CVECTOR *palette, unsigned short *packed)
{
    unsigned char state[8];
    unsigned int original;
    unsigned int buttons;
    unsigned int mouseButtons;
    int column;
    int row;
    int value;

    original = func_8001B320(0, 0);
    buttons = original & 0xFFFF;
    func_8001B320(1, state);
    mouseButtons = state[3];
    if ((original & 0xF000) == 0) {
        D_800631EC = 0;
    }
    if (state[4] == 0x12) {
        if (mouseButtons & 8) buttons |= 0x400040;
        if (mouseButtons & 4) buttons |= 0x200020;
        if (state[0] != 0 || state[1] != 0) {
            D_800631E8 = 1;
            cursor[0] += (signed char)state[0] << 16;
            cursor[1] += (signed char)state[1] << 16;
            if (cursor[0] < 0) cursor[0] = 0;
            if (cursor[0] > 0x13FFFFF) cursor[0] = 0x13F0000;
            if (cursor[1] < 0x100000) cursor[1] = 0x100000;
            if (cursor[1] > 0xDFFFFF) cursor[1] = 0xDF0000;
            D_800631EC = 0;
            goto done;
        }
    }
    switch (mode) {
    case 1:
        if ((buttons & 4) == 0) {
            ((void (*)(int *, unsigned int, void *))func_8005CB9C)(cursor, buttons, state);
        }
        break;
    case 2:
        if (D_800631EC < 2 && (buttons & 0xF000) != 0) {
            D_800631E8 = 0;
            if (D_800631EC != 0) {
                D_800631EC = 4;
            } else {
                D_800631EC = 11;
            }
            column = D_800631F8 % 8;
            row = D_800631F8 / 8;
            if (buttons & 0x8000) column--;
            if (buttons & 0x2000) column++;
            if (buttons & 0x1000) row--;
            if (buttons & 0x4000) row++;
            column = column < 0 ? 0 : column >= 8 ? 7 : column;
            row = row < 0 ? 0 : row >= 8 ? 7 : row;
            cursor[0] = (column * 8 + 29) << 16;
            cursor[1] = (row * 8 + 73) << 16;
            D_800631F8 = row * 8 + column;
        }
        break;
    case 3:
        func_8005D908(cursor, buttons, state, palette, packed);
        break;
    case 5:
        if (D_800631EC < 2 && (buttons & 0x5000) != 0) {
            D_800631E8 = 0;
            if (D_800631EC == 0) {
                D_800631EC = 11;
            } else {
                D_800631EC = 4;
            }
            value = (((short *)cursor)[3] - 63) / 15;
            if (buttons & 0x1000) value--;
            if (buttons & 0x4000) value++;
            if (value < 0) {
                value = 0;
            } else if (value > 8) {
                value = 8;
            } else {
                func_80015B70(0, 128);
            }
            cursor[0] = 290 << 16;
            cursor[1] = (value * 15 + 70) << 16;
        }
        break;
    case 4:
    default:
        if (buttons & 0xF000) {
            D_800631E8 = 0;
            cursor[0] = D_800631F0;
            cursor[1] = D_800631F4;
        }
        break;
    }
done:
    if (D_800631EC > 0) D_800631EC--;
    return buttons;
}

// FUNCTION: BDEDIT.BIN:0x8005ECC8
void func_8005ECC8(unsigned short **source_planes, unsigned short **destination_planes,
                   unsigned short *source_palette, unsigned short *destination_palette)
{
    int i;

    i = 0;
    do {
        destination_planes[0][i] = source_planes[0][i];
        destination_planes[1][i] = source_planes[1][i];
        i++;
    } while (i < 0x1000);

    i = 0;
    do {
        *destination_palette++ = *source_palette++;
        i++;
    } while (i < 0x100);
}

// FUNCTION: BDEDIT.BIN:0x8005ED3C
void func_8005ED3C(unsigned short **first_planes, unsigned short **second_planes,
                   unsigned short *first_palette, unsigned short *second_palette)
{
    unsigned short temporary;
    int i;

    i = 0;
plane_loop:
        temporary = second_planes[0][i];
        second_planes[0][i] = first_planes[0][i];
        first_planes[0][i] = temporary;
        temporary = second_planes[1][i];
        second_planes[1][i] = first_planes[1][i];
        first_planes[1][i] = temporary;
        i++;
    if (i < 0x1000) goto plane_loop;

    i = 0;
    do {
        temporary = *second_palette;
        *second_palette = *first_palette;
        *first_palette = temporary;
        first_palette++;
        second_palette++;
        i++;
    } while (i < 0x100);
}

// FUNCTION: BDEDIT.BIN:0x8005EDD8
unsigned char *func_8005EDD8(unsigned long *ot, unsigned char *packet,
                          int x, int y, int w, int h, CVECTOR *color)
{
    LINE_F3 *line;
    short right, bottom;

    line = (LINE_F3 *)packet;

    packet = (unsigned char *)(line + 1);
    ((PrimitiveTag *)line)->length = 5;
    line->code = 0x48;
    line->pad = 0x55555555;
    line->x0 = x;
    line->y0 = y;
    line->y1 = y;
    right = x + w;
    bottom = y + h;
    line->x1 = right;
    line->x2 = right;
    line->y2 = bottom;
    line->r0 = color->r;
    line->g0 = color->g;
    line->b0 = color->b;
    ((PrimitiveTag *)line)->address = ((PrimitiveTag *)ot)->address;
    ((PrimitiveTag *)ot)->address = (unsigned long)line;

    line = (LINE_F3 *)packet;
    packet = (unsigned char *)(line + 1);
    ((PrimitiveTag *)line)->length = 5;
    line->code = 0x48;
    line->pad = 0x55555555;
    line->x0 = x;
    line->y0 = y;
    line->x1 = x;
    line->y1 = bottom;
    line->x2 = right;
    line->y2 = bottom;
    line->r0 = color->r;
    line->g0 = color->g;
    line->b0 = color->b;
    ((PrimitiveTag *)line)->address = ((PrimitiveTag *)ot)->address;
    ((PrimitiveTag *)ot)->address = (unsigned long)line;
    return packet;
}

// FUNCTION: BDEDIT.BIN:0x8005EEF8
unsigned char *func_8005EEF8(unsigned long *ot, unsigned char *packet,
                          int x, int y, int w, int h, CVECTOR *color, int flags)
{
    TILE *tile;
    DR_TPAGE *mode;

    tile = (TILE *)packet;
    packet += sizeof(TILE);
    ((PrimitiveTag *)tile)->length = 3;
    tile->code = 0x60;
    tile->x0 = x;
    tile->y0 = y;
    tile->w = w;
    tile->h = h;
    tile->r0 = color->r;
    tile->g0 = color->g;
    tile->b0 = color->b;
    if (flags & 0x80) {
        tile->code |= 2;
    }
    ((PrimitiveTag *)tile)->address = ((PrimitiveTag *)ot)->address;
    ((PrimitiveTag *)ot)->address = (unsigned long)tile;

    if (flags & 0x80) {
        mode = (DR_TPAGE *)packet;
        packet += sizeof(DR_TPAGE);
        ((PrimitiveTag *)mode)->length = 1;
        mode->code[0] = 0xE1000000 |
            ((GetGraphType() == 1 || GetGraphType() == 2) ?
             (((flags & 3) << 5) & 0x27FF) : (((flags & 3) << 5) & 0x09FF));
        ((PrimitiveTag *)mode)->address = ((PrimitiveTag *)ot)->address;
        ((PrimitiveTag *)ot)->address = (unsigned long)mode;
    }
    return packet;
}

// FUNCTION: BDEDIT.BIN:0x8005F058
unsigned char *func_8005F058(unsigned long *ot, unsigned char *packet,
                          int x, int y, int w, int h, CVECTOR *color, int flags)
{
    POLY_G4 *poly;
    DR_TPAGE *mode;

    poly = (POLY_G4 *)packet;
    packet += sizeof(POLY_G4);
    ((PrimitiveTag *)poly)->length = 8;
    poly->code = 0x38;
    poly->x0 = x;
    poly->y0 = y;
    poly->y1 = y;
    poly->x2 = x;
    poly->x1 = x + w;
    poly->y2 = y + h;
    poly->x3 = x + w;
    poly->y3 = y + h;
    poly->r0 = poly->r3 = color->r;
    poly->g0 = poly->g3 = color->g;
    poly->b0 = poly->b3 = color->b;
    poly->r1 = poly->r2 = color->r * 2 / 3;
    poly->g1 = poly->g2 = color->g * 2 / 3;
    poly->b1 = poly->b2 = color->b * 2 / 3;
    if (flags & 0x80) {
        poly->code |= 2;
    }
    ((PrimitiveTag *)poly)->address = ((PrimitiveTag *)ot)->address;
    ((PrimitiveTag *)ot)->address = (unsigned long)poly;

    if (flags & 0x80) {
        mode = (DR_TPAGE *)packet;
        packet += sizeof(DR_TPAGE);
        ((PrimitiveTag *)mode)->length = 1;
        mode->code[0] = 0xE1000000 |
            ((GetGraphType() == 1 || GetGraphType() == 2) ?
             (((flags & 3) << 5) & 0x27FF) : (((flags & 3) << 5) & 0x09FF));
        ((PrimitiveTag *)mode)->address = ((PrimitiveTag *)ot)->address;
        ((PrimitiveTag *)ot)->address = (unsigned long)mode;
    }
    return packet;
}

// FUNCTION: BDEDIT.BIN:0x8005F634
void func_8005F634(unsigned short *packed, CVECTOR *palette, int count)
{
    unsigned int blue;
    int green;
    unsigned int red;
    int index;

    for (index = 0; index < count; index++) {
        blue = (palette[index].b << 7) & 0x7C00;
        green = (palette[index].g << 2) & 0x3E0;
        red = palette[index].r >> 3;
        *packed = (blue | (unsigned short)(palette[index].cd ? green | 0x8000 : green)) | red;
        packed++;
    }
}

// FUNCTION: BDEDIT.BIN:0x8005F6A4
void func_8005F6A4(CVECTOR *palette, unsigned short *packed, int count)
{
    int index;

    for (index = 0; index < count; index++) {
        palette[index].b = ((*packed >> 10 & 31) * 255) / 31;
        palette[index].g = ((*packed >> 5 & 31) * 255) / 31;
        palette[index].r = ((*packed & 31) * 255) / 31;
        palette[index].cd = *packed >> 15;
        packed++;
    }
}

// FUNCTION: BDEDIT.BIN:0x8005F7E8
unsigned char *func_8005F7E8(unsigned long *ot, unsigned char *packet)
{
    BdEditFrameTextures textures = D_80063030;
    BackgroundTextures caps = D_80063064;
    BdEditFramePositions positions = D_80063078;
    BdEditCapPositions cap_positions = D_800630A0;
    int index;
    POLY_FT4 *poly;

    index = 0;
    do {
        packet = (unsigned char *)((SPRT *(*)(unsigned long *, SPRT *, int, int,
                    SpriteTexture *, unsigned char))func_80012950)(ot, (SPRT *)packet,
                    positions.entries[index][0], positions.entries[index][1],
                    &textures.entries[index], 128);
        packet = (unsigned char *)func_80012B6C(ot, (DR_TPAGE *)packet,
                    textures.entries[index].tpage);
        index++;
    } while (index < 5);

    index = 0;
    do {
        poly = (POLY_FT4 *)packet;
        packet += sizeof(POLY_FT4);
        ((unsigned char *)poly)[3] = 9;
        poly->code = 0x2C;
        poly->x0 = poly->x1 = cap_positions.entries[index][0];
        poly->x2 = poly->x3 = cap_positions.entries[index][0] + caps.entries[index].h;
        poly->y0 = poly->y2 = cap_positions.entries[index][1];
        poly->y1 = poly->y3 = cap_positions.entries[index][1] - caps.entries[index].w;
        poly->u0 = poly->u2 = caps.entries[index].u - 1;
        poly->u1 = poly->u3 = caps.entries[index].u + caps.entries[index].w - 1;
        poly->v0 = poly->v1 = caps.entries[index].v;
        poly->v2 = poly->v3 = caps.entries[index].v + caps.entries[index].h;
        poly->tpage = caps.entries[index].tpage;
        poly->clut = caps.entries[index].clut;
        poly->r0 = poly->g0 = poly->b0 = 128;
        ((PrimitiveTag *)poly)->address = ((PrimitiveTag *)ot)->address;
        ((PrimitiveTag *)ot)->address = (unsigned long)poly;

        index++;
    } while (index < 2);
    return packet;
}

// FUNCTION: BDEDIT.BIN:0x8005FCFC
unsigned char *func_8005FCFC(unsigned long *ot, unsigned char *packet,
                           short x, short y, unsigned char *text, CVECTOR *color)
{
    unsigned char glyph;
    unsigned int index;
    SPRT *sprite;
    unsigned char *cursor;

    if (text == 0) {
        return packet;
    }
    cursor = text;
    while (*cursor != 0) {
        glyph = *cursor++;
        sprite = (SPRT *)packet;
        packet += sizeof(SPRT);
        ((PrimitiveTag *)sprite)->length = 4;
        sprite->code = 0x64;
        sprite->x0 = x;
        sprite->y0 = y;
        if (glyph >= 0x20) {
            sprite->w = 8;
            sprite->h = 12;
            sprite->clut = 0x7334;
            if (color != 0) {
                sprite->r0 = color->r;
                sprite->g0 = color->g;
                sprite->b0 = color->b;
            } else {
                sprite->r0 = sprite->g0 = sprite->b0 = 0x80;
            }
            glyph -= 0x20;
            x += 8;
            index = glyph;
            sprite->u0 = (index & 15) << 3;
            sprite->v0 = (index >> 4) * 12 - 0x50;
        } else {
            sprite->y0 = y - 2;
            sprite->w = 16;
            sprite->h = 16;
            sprite->r0 = sprite->g0 = sprite->b0 = 0x80;
            if (glyph == 2) {
                sprite->clut = 0x7335;
                sprite->u0 = 0x30;
                sprite->v0 = 0xA0;
            } else {
                sprite->clut = 0x7336;
                sprite->u0 = 0x40;
                sprite->v0 = 0xA0;
            }
            x += 16;
        }
        ((PrimitiveTag *)sprite)->address = ((PrimitiveTag *)ot)->address;
        ((PrimitiveTag *)ot)->address = (unsigned long)sprite;
    }
    return (unsigned char *)func_80012B6C(ot, (DR_TPAGE *)packet, 5);
}

// FUNCTION: BDEDIT.BIN:0x8005FF08
void func_8005FF08(void)
{
    D_80063204 = -1;
    D_80063200 = -1;
}

// FUNCTION: BDEDIT.BIN:0x8005FF24
int func_8005FF24(int x, int y, unsigned int buttons, unsigned int previous,
                   unsigned char **planes, int *color, unsigned char *mask)
{
    int changed = 0;
    int mask_x;
    int mask_y;
    int allowed;
    int plane;
    int pixel_x;
    unsigned char *pixels;

    if ((buttons & 0x60) == 0x40) {
        mask_x = x;
        mask_y = y;
        allowed = 0;
        if ((unsigned int)x < 256 && (unsigned int)y < 32) {
            if (x >= 128) {
                mask_x = x - 128;
                mask_y = y + 32;
            }
            if (((mask[mask_y * 16 + mask_x / 8] >> (mask_x % 8)) & 1) == 0) {
                allowed = 1;
            }
        }
        if (allowed) {
            if (x < 128) {
                plane = 0;
                pixel_x = x;
            } else {
                plane = 1;
                pixel_x = x - 128;
            }
            changed = 1;
            pixels = planes[plane];
            pixels[y * 128 + pixel_x] = *color;
        }
    }
    if ((buttons & 0x40) == 0 && (previous & 0x20) != 0) {
        mask_x = x;
        mask_y = y;
        allowed = 0;
        if ((unsigned int)x < 256 && (unsigned int)y < 32) {
            if (x >= 128) {
                mask_x = x - 128;
                mask_y = y + 32;
            }
            if (((mask[mask_y * 16 + mask_x / 8] >> (mask_x % 8)) & 1) == 0) {
                allowed = 1;
            }
        }
        if (allowed) {
            func_80015B70(4, 128);
            if (x < 128) {
                plane = 0;
                pixel_x = x;
            } else {
                plane = 1;
                pixel_x = x - 128;
            }
            pixels = planes[plane];
            *color = pixels[y * 128 + pixel_x];
        }
    }
    return changed;
}

// FUNCTION: BDEDIT.BIN:0x80060108
int func_80060108(int x, int y, unsigned int buttons, unsigned int previous,
                   unsigned char **planes, int *color, unsigned char *mask)
{
    int changed = 0;
    int mask_x;
    int mask_y;
    int allowed;
    int plane;
    int pixel_x;

    if ((buttons & 0x20) == 0 && (previous & 0x40) != 0) {
        if (D_80063200 < 0 || D_80063204 < 0) {
            D_80063200 = x;
            D_80063204 = y;
        } else {
            changed = 1;
            func_80060318(planes, D_80063200, D_80063204, x, y, *color, mask);
            D_80063204 = -1;
            D_80063200 = -1;
        }
    }
    if ((buttons & 0x40) == 0 && (previous & 0x20) != 0) {
        if (D_80063200 < 0 || D_80063204 < 0) {
            mask_x = x;
            mask_y = y;
            allowed = 0;
            if ((unsigned int)x < 256 && (unsigned int)y < 32) {
                if (x >= 128) {
                    mask_x = x - 128;
                    mask_y = y + 32;
                }
                if (((mask[mask_y * 16 + mask_x / 8] >> (mask_x % 8)) & 1) == 0) {
                    allowed = 1;
                }
            }
            if (allowed) {
                func_80015B70(4, 128);
                if (x < 128) {
                    plane = 0;
                    pixel_x = x;
                } else {
                    plane = 1;
                    pixel_x = x - 128;
                }
                *color = planes[plane][y * 128 + pixel_x];
            } else {
                D_80063204 = -1;
                D_80063200 = -1;
            }
        } else {
            D_80063204 = -1;
            D_80063200 = -1;
        }
    }
    return changed;
}

// FUNCTION: BDEDIT.BIN:0x80060318
void func_80060318(unsigned char **planes, int x0, int y0,
                   int x1, int y1, int color, unsigned char *mask)
{
    int temp;

    if (x1 < x0) {
        temp = x1;
        x1 = x0;
        x0 = temp;
        temp = y1;
        y1 = y0;
        y0 = temp;
    }
    x1 -= x0;
    y1 -= y0;
    if (x1 < -y1) {
        func_800603DC(planes, x0, y0, x1, y1, color, mask);
    } else if (x1 < y1) {
        func_800604F4(planes, x0, y0, x1, y1, color, mask);
    } else if (y1 <= 0) {
        func_80060604(planes, x0, y0, x1, y1, color, mask);
    } else {
        func_80060714(planes, x0, y0, x1, y1, color, mask);
    }
}
