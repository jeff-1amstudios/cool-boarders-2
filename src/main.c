#include "main.h"

extern void boot_noop(void);
extern void func_8001228C(void);
extern void func_80015814(void);
extern void func_80016EE4(void);
extern void func_8001B9A4(void);
extern CdlFILE *CdSearchFile(CdlFILE *file, char *name);
extern int func_8001C0F4(int *state, int enabled);
extern int func_800123F8(int overlay, int size);
extern void func_800142F0(int mode);
extern void func_80012508(void);
extern void func_8001B59C(void);
extern int func_8005AB6C(int *state);
extern void func_8001B64C(void);
extern void func_8001263C(void);
extern void func_80044938(void); /* SDK exit */
extern char init_filename[];
extern unsigned int D_800580A8;
extern unsigned long D_800580AC;

extern CdlCB CdSyncCallback(CdlCB callback);
extern CdlCB CdReadyCallback(CdlCB callback);
extern unsigned long DrawSyncCallback(void (*callback)(void));
extern void SpuSetKey(long on_off, unsigned long voice_bits);
extern void func_8003F094(void);
extern void func_8001BA78(void);
extern int ResetGraph(int mode);
extern void CdFlush(void);
extern void StopPAD(void);
extern void func_800178AC(void);
extern void PadStop(void);
extern int StopCallback(void);

extern OverlayFileIndices overlay_file_indices;
extern OverlayHeapStarts overlay_heap_starts;
extern char *overlay_load_addresses[15];
extern int func_80013B60(int file_index, char *destination, int offset, int flags);
extern void func_8001F150(char *heap_start, char *heap_end);

extern DRAWENV D_80058BA0;
extern DRAWENV D_80058E68;
extern DISPENV D_80058C00;
extern DISPENV D_80058EC8;
extern RECT D_80058F70;
extern RECT D_8005AA90;
extern short D_80058030;
extern short D_80058032;
extern short D_80058034;
extern short D_80058036;
extern short D_80058ADC;
extern short D_80058ADE;
extern short D_80058B00;
extern short D_80058B02;

extern void GsInit3D(void);
extern int D_80058E10;
extern int D_80058E18;

extern DRAWENV *SetDefDrawEnv(DRAWENV *env, int x, int y, int w, int h);
extern DISPENV *SetDefDispEnv(DISPENV *env, int x, int y, int w, int h);
extern void *func_8001F1A0(unsigned int size);

extern int GetGraphType(void);

extern int func_8003CB2C(void);
extern int func_8003CA24(int mode);
extern int ResetCallback(void);
extern void func_8001B228(void);
extern int ChangeClearPAD(int mode);
extern void func_8001F824(void);
extern void func_8001787C(void);
extern int ClearImage(RECT *rect, unsigned char r, unsigned char g, unsigned char b);
extern int DrawSync(int mode);
extern void GsInitGraph(unsigned short x, unsigned short y, unsigned short intmode,
                        unsigned short dith, unsigned short varmmode);
extern void func_8002ECB0(short x0, short y0, short x1, short y1);
extern void func_8001E8C0(void);
extern void func_8003CB3C(void);
extern void SetDispMask(int mask);

/* Keep the inline return cursor explicit: it affects GCC 2.7.2 scheduling. */
static __inline__ DR_TPAGE *append_draw_tpage(unsigned long *ordering_table, DR_TPAGE *packet, unsigned int tpage)
{
    DR_TPAGE *next = packet + 1;

    ((PrimitiveTag *)packet)->length = 1;
    packet->code[0] = 0xE1000000 |
        ((GetGraphType() == 1 || GetGraphType() == 2) ?
         (tpage & 0x27FF) : (tpage & 0x09FF));
    ((PrimitiveTag *)packet)->address = ((PrimitiveTag *)ordering_table)->address;
    ((PrimitiveTag *)ordering_table)->address = (unsigned long)packet;
    return next;
}

extern int rsin(int angle);
extern int rcos(int angle);

extern int CdControl(unsigned char command, unsigned char *param, unsigned char *result);

extern DiscFileRecord disc_file_records[];

extern int CdControlF(unsigned char command, unsigned char *param);

extern int D_80045364;
extern FileReadRequest D_800502E8[20];

extern int CdPosToInt(CdlLOC *position);
extern int VSync(int mode);
extern void func_800140A0(unsigned char status, unsigned char *result);
extern int D_80045354;
extern int D_80045350;

// FUNCTION: SCUS_943.58:0x800121B8
void boot_main(void)
{
    int mode;
    int state[16];
    CdlFILE file;

    boot_noop();
    mode = 6;
    func_8001228C();
    func_80015814();
    func_80016EE4();
    func_8001B9A4();
    CdSearchFile(&file, init_filename);
    D_800580A8 = *(unsigned int *)&file.pos;
    D_800580AC = file.size;
    func_8001C0F4(state, 1);
    do {
        state[1] = func_800123F8(state[0], state[1]);
        func_800142F0(mode);
        func_80012508();
        func_8001B59C();
        mode = func_8005AB6C(state);
        func_8001B64C();
        func_8001263C();
    } while (mode != -1);
    func_80044938();
}

// FUNCTION: SCUS_943.58:0x8001228C
void func_8001228C(void)
{
    RECT rect;

    func_8003CB2C();
    func_8003CA24(0);
    ResetCallback();
    func_8001B228();
    ChangeClearPAD(0);
    ResetGraph(0);
    func_8001F824();
    func_8001787C();
    rect.w = 1024;
    rect.x = 0;
    rect.y = 0;
    rect.h = 512;
    ClearImage(&rect, 0, 0, 0);
    DrawSync(0);
    GsInitGraph(320, 240, 4, 1, 0);
    D_80058C00.screen.y = 0;
    D_80058C00.screen.h = 239;
    func_8002ECB0(0, 0, 0, 240);
    GsInit3D();
    func_8001E8C0();
    func_8003CB3C();
    SetDispMask(1);
}

// FUNCTION: SCUS_943.58:0x80012378
void func_80012378(void)
{
    CdSyncCallback(0);
    CdReadyCallback(0);
    DrawSyncCallback(0);
    SpuSetKey(0, 0xFFFFFF);
    func_8003F094();
    func_8001BA78();
    ResetGraph(3);
    CdFlush();
    StopPAD();
    func_800178AC();
    PadStop();
    StopCallback();
}

// FUNCTION: SCUS_943.58:0x800123F8
int func_800123F8(int overlay, int size)
{
    OverlayFileIndices file_indices;
    OverlayHeapStarts heap_starts;
    char *heap_end;

    file_indices = overlay_file_indices;
    heap_starts = overlay_heap_starts;
    func_80013B60(file_indices.entries[overlay], overlay_load_addresses[0], 0, 1);
    heap_end = (char *)0x801F1E00 - size;
    func_8001F150(heap_starts.entries[overlay], heap_end);
    return (int)heap_end;
}

// FUNCTION: SCUS_943.58:0x80012508
void func_80012508(void)
{
    D_80058E68 = D_80058BA0;
    D_80058EC8 = D_80058C00;
    D_8005AA90 = D_80058F70;
    D_80058ADC = D_80058030;
    D_80058ADE = D_80058032;
    D_80058B00 = D_80058034;
    D_80058B02 = D_80058036;
}

// FUNCTION: SCUS_943.58:0x8001263C
void func_8001263C(void)
{
    D_80058BA0 = D_80058E68;
    D_80058C00 = D_80058EC8;
    D_80058F70 = D_8005AA90;
    D_80058030 = D_80058ADC;
    D_80058032 = D_80058ADE;
    D_80058034 = D_80058B00;
    D_80058036 = D_80058B02;
}

// FUNCTION: SCUS_943.58:0x80012770
void func_80012770(int width, int height)
{
    unsigned char interlaced;

    interlaced = height > 256;
    D_80058C00.screen.h = 239;
    D_80058BA0.ofs[0] = width / 2;
    D_80058BA0.ofs[1] = height / 2;
    D_80058C00.disp.w = width;
    D_80058C00.disp.h = height;
    D_80058C00.screen.y = 0;
    D_80058C00.isinter = interlaced;
    D_80058BA0.clip.w = width;
    D_80058BA0.clip.h = height;
    D_80058BA0.dtd = 1;
    D_80058BA0.dfe = 0;
    D_80058E10 = width;
    D_80058E18 = height;
    D_80058F70.w = width;
    D_80058F70.h = height;
    D_80058032 = 0;
    D_80058030 = 0;
    D_80058034 = 0;
    if (height <= 256) {
        D_80058036 = 256;
    } else {
        D_80058036 = 0;
    }
    GsInit3D();
}

// FUNCTION: SCUS_943.58:0x80012868
void func_80012868(RenderBuffer *buffers, unsigned int size)
{
    SetDefDrawEnv(&buffers[0].draw, 0, 256, 320, 240);
    SetDefDrawEnv(&buffers[1].draw, 0, 0, 320, 240);
    buffers[0].draw.b0 = 0;
    buffers[0].draw.g0 = 0;
    buffers[0].draw.r0 = 0;
    buffers[1].draw.b0 = 0;
    buffers[1].draw.g0 = 0;
    buffers[1].draw.r0 = 0;
    buffers[0].draw.isbg = 1;
    buffers[1].draw.isbg = 1;
    SetDefDispEnv(&buffers[0].disp, 0, 0, 320, 240);
    SetDefDispEnv(&buffers[1].disp, 0, 256, 320, 240);
    buffers[0].disp.screen.h = 239;
    buffers[1].disp.screen.h = 239;
    buffers[0].disp.screen.y = 0;
    buffers[1].disp.screen.y = 0;
    buffers[0].allocation = func_8001F1A0(size);
    buffers[1].allocation = func_8001F1A0(size);
}

// FUNCTION: SCUS_943.58:0x80012950
SPRT *func_80012950(unsigned long *ordering_table, SPRT *sprite, short x, short y,
                    SpriteTexture *texture, unsigned char shade)
{
    ((PrimitiveTag *)sprite)->length = 4;
    sprite->code = 0x64;
    sprite->w = texture->w;
    sprite->h = texture->h;
    sprite->r0 = sprite->g0 = sprite->b0 = shade;
    sprite->x0 = x;
    sprite->y0 = y;
    sprite->u0 = texture->u;
    sprite->v0 = texture->v;
    sprite->clut = texture->clut;
    ((PrimitiveTag *)sprite)->address = ((PrimitiveTag *)ordering_table)->address;
    ((PrimitiveTag *)ordering_table)->address = (unsigned long)sprite;
    return sprite + 1;
}

// FUNCTION: SCUS_943.58:0x800129E8
SPRT *func_800129E8(unsigned long *ordering_table, SPRT *sprite, short x, short y,
                    SpriteTexture *texture, CVECTOR *color, int semitransparent)
{
    SPRT *primitive = sprite++;

    ((PrimitiveTag *)primitive)->length = 4;
    primitive->code = 0x64;
    primitive->w = texture->w;
    primitive->h = texture->h;
    if (color != 0) {
        primitive->r0 = color->r;
        primitive->g0 = color->g;
        primitive->b0 = color->b;
    } else {
        primitive->r0 = primitive->g0 = primitive->b0 = 0x80;
    }
    primitive->x0 = x;
    primitive->y0 = y;
    primitive->u0 = texture->u;
    primitive->v0 = texture->v;
    primitive->clut = texture->clut;
    if (semitransparent) {
        primitive->code |= 2;
    } else {
        primitive->code &= 0xFD;
    }
    ((PrimitiveTag *)primitive)->address = ((PrimitiveTag *)ordering_table)->address;
    ((PrimitiveTag *)ordering_table)->address = (unsigned long)primitive;
    return sprite;
}

// FUNCTION: SCUS_943.58:0x80012AE8
SPRT_16 *func_80012AE8(unsigned long *ordering_table, SPRT_16 *sprite, short x, short y,
                     Sprite16Texture *texture, unsigned char shade)
{
    ((PrimitiveTag *)sprite)->length = 3;
    sprite->code = 0x7C;
    sprite->x0 = x;
    sprite->y0 = y;
    sprite->r0 = sprite->g0 = sprite->b0 = shade;
    sprite->u0 = texture->u;
    sprite->v0 = texture->v;
    sprite->clut = texture->clut;
    ((PrimitiveTag *)sprite)->address = ((PrimitiveTag *)ordering_table)->address;
    ((PrimitiveTag *)ordering_table)->address = (unsigned long)sprite;
    return sprite + 1;
}

// FUNCTION: SCUS_943.58:0x80012B6C
DR_TPAGE *func_80012B6C(unsigned long *ordering_table, DR_TPAGE *packet,
                        unsigned int tpage)
{
    DR_TPAGE *primitive = packet++;

    ((PrimitiveTag *)primitive)->length = 1;
    primitive->code[0] = 0xE1000000 |
        ((GetGraphType() == 1 || GetGraphType() == 2) ?
         (tpage & 0x27FF) : (tpage & 0x09FF));
    ((PrimitiveTag *)primitive)->address = ((PrimitiveTag *)ordering_table)->address;
    ((PrimitiveTag *)ordering_table)->address = (unsigned long)primitive;
    return packet;
}

// FUNCTION: SCUS_943.58:0x80012C30
unsigned char *func_80012C30(unsigned long *ordering_table, unsigned char *packet, RECT *texture,
                    unsigned short clut, int x, int y, unsigned char *string,
                    CVECTOR *color, unsigned short mode)
{
    int u = (short)(texture->x % 64) << 2;
    int v = (short)(texture->y % 256);
    unsigned char glyph;
    unsigned int index;
    unsigned char *text = string;
    SPRT *sprite;
    unsigned short tpage;

    while (*text != 0) {
        glyph = *text++;
        sprite = (SPRT *)packet;
        packet += sizeof(SPRT);
        ((PrimitiveTag *)sprite)->length = 4;
        sprite->code = 0x64;
        sprite->w = texture->w;
        sprite->h = texture->h;
        sprite->x0 = x;
        sprite->y0 = y;
        if (color != 0) {
            sprite->r0 = color->r;
            sprite->g0 = color->g;
            sprite->b0 = color->b;
            glyph -= color->cd;
        } else {
            sprite->r0 = sprite->g0 = sprite->b0 = 0x80;
        }
        index = glyph;
        sprite->u0 = u + (index & 15) * texture->w;
        sprite->v0 = v + (index >> 4) * texture->h;
        sprite->clut = clut;
        if (mode >= 128) {
            sprite->code |= 2;
        } else {
            sprite->code &= 0xFD;
        }
        ((PrimitiveTag *)sprite)->address = ((PrimitiveTag *)ordering_table)->address;
        ((PrimitiveTag *)ordering_table)->address = (unsigned long)sprite;
        x += texture->w;
    }
    tpage = ((mode & 3) << 5) | (((unsigned short)texture->y >> 4) & 16) |
            (((unsigned short)texture->x & 0x3FF) >> 6);
    packet = (unsigned char *)append_draw_tpage(ordering_table, (DR_TPAGE *)packet, tpage);
    return packet;
}

// FUNCTION: SCUS_943.58:0x80012EA0
unsigned char *func_80012EA0(unsigned long *ordering_table, unsigned char *packet, RECT *texture,
                    unsigned short clut, int x, int y, unsigned int number, CVECTOR *color)
{
    unsigned int digits[16];
    int count = 0;
    unsigned int quotient;
    int u, v;
    int index;
    SPRT *sprite;
    unsigned int tpage;

    do {
        quotient = number / 10;
        digits[count] = number % 10;
        number = quotient;
        count++;
    } while (number != 0);
    u = (short)(texture->x % 64) << 2;
    v = (short)(texture->y % 256);
    for (index = count - 1; index >= 0; index--) {
        sprite = (SPRT *)packet;
        packet += sizeof(SPRT);
        ((PrimitiveTag *)sprite)->length = 4;
        sprite->code = 0x64;
        sprite->w = texture->w;
        sprite->h = texture->h;
        sprite->x0 = x;
        sprite->y0 = y;
        if (color != 0) {
            sprite->r0 = color->r;
            sprite->g0 = color->g;
            sprite->b0 = color->b;
        } else {
            sprite->r0 = sprite->g0 = sprite->b0 = 0x80;
        }
        sprite->u0 = u + digits[index] * texture->w;
        sprite->v0 = v;
        sprite->clut = clut;
        ((PrimitiveTag *)sprite)->address = ((PrimitiveTag *)ordering_table)->address;
        ((PrimitiveTag *)ordering_table)->address = (unsigned long)sprite;
        x += texture->w;
    }
    tpage = ((((unsigned short)texture->y >> 8) & 1) << 4) |
            (((unsigned short)texture->x & 0x3FF) >> 6);
    packet = (unsigned char *)append_draw_tpage(ordering_table, (DR_TPAGE *)packet, tpage);
    return packet;
}

// FUNCTION: SCUS_943.58:0x800130F0
unsigned char *func_800130F0(unsigned long *ordering_table, unsigned char *packet, RECT *texture,
                    unsigned short clut, int x, int y, unsigned int number, int max_digits,
                    CVECTOR *color)
{
    int u, v;
    int index;
    SPRT *sprite;
    unsigned int tpage;

    x += texture->w * max_digits;
    u = (short)(texture->x % 64) << 2;
    v = (short)(texture->y % 256);
    for (index = 0; index < max_digits; index++) {
        x -= texture->w;
        sprite = (SPRT *)packet;
        packet += sizeof(SPRT);
        ((PrimitiveTag *)sprite)->length = 4;
        sprite->code = 0x64;
        sprite->w = texture->w;
        sprite->h = texture->h;
        sprite->x0 = x;
        sprite->y0 = y;
        if (color != 0) {
            sprite->r0 = color->r;
            sprite->g0 = color->g;
            sprite->b0 = color->b;
        } else {
            sprite->r0 = sprite->g0 = sprite->b0 = 0x80;
        }
        sprite->u0 = u + (number % 10) * texture->w;
        sprite->v0 = v;
        sprite->clut = clut;
        number /= 10;
        ((PrimitiveTag *)sprite)->address = ((PrimitiveTag *)ordering_table)->address;
        ((PrimitiveTag *)ordering_table)->address = (unsigned long)sprite;
        if (number == 0) {
            break;
        }
    }
    tpage = ((((unsigned short)texture->y >> 8) & 1) << 4) |
            (((unsigned short)texture->x & 0x3FF) >> 6);
    packet = (unsigned char *)append_draw_tpage(ordering_table, (DR_TPAGE *)packet, tpage);
    return packet;
}

// FUNCTION: SCUS_943.58:0x80013340
POLY_FT4 *func_80013340(unsigned long *ordering_table, POLY_FT4 *packet,
                      SpriteTexture *texture, int x, int y, int angle,
                      int scale, CVECTOR *color)
{
    int left = -((texture->w / 2) * scale) >> 8;
    int right = (((texture->w + 1) / 2) * scale) >> 8;
    int top = -((texture->h / 2) * scale) >> 8;
    int bottom = (((texture->h + 1) / 2) * scale) >> 8;
    int sine = rsin(angle);
    int cosine = rcos(angle);
    POLY_FT4 *primitive = packet++;
    int left_cos, top_sin, right_cos, bottom_sin;
    int left_sin, top_cos, right_sin, bottom_cos;

    ((PrimitiveTag *)primitive)->length = 9;
    primitive->code = 0x2C;
    primitive->clut = texture->clut;
    primitive->tpage = texture->tpage;
    if (color == 0) {
        primitive->r0 = primitive->g0 = primitive->b0 = 0x80;
    } else {
        primitive->r0 = color->r;
        primitive->g0 = color->g;
        primitive->b0 = color->b;
        if (color->cd != 0) {
            primitive->code |= 2;
        }
    }
    left_cos = left * cosine;
    top_sin = top * sine;
    right_cos = right * cosine;
    bottom_sin = bottom * sine;
    left_sin = left * sine;
    primitive->u0 = primitive->u2 = texture->u;
    primitive->u1 = primitive->u3 = texture->u + (unsigned char)texture->w;
    top_cos = top * cosine;
    primitive->v0 = primitive->v1 = texture->v;
    primitive->v2 = primitive->v3 = texture->v + (unsigned char)texture->h;
    right_sin = right * sine;
    primitive->x0 = x + ((left_cos - top_sin) >> 12);
    primitive->x1 = x + ((right_cos - top_sin) >> 12);
    primitive->x2 = x + ((left_cos - bottom_sin) >> 12);
    bottom_cos = bottom * cosine;
    primitive->x3 = x + ((right_cos - bottom_sin) >> 12);
    primitive->y0 = y + ((left_sin + top_cos) >> 12);
    primitive->y1 = y + ((right_sin + top_cos) >> 12);
    primitive->y2 = y + ((left_sin + bottom_cos) >> 12);
    primitive->y3 = y + ((right_sin + bottom_cos) >> 12);
    ((PrimitiveTag *)primitive)->address = ((PrimitiveTag *)ordering_table)->address;
    ((PrimitiveTag *)ordering_table)->address = (unsigned long)primitive;
    return packet;
}

// FUNCTION: SCUS_943.58:0x80013A20
void func_80013A20(unsigned char *data)
{
    int block, index, next;
    unsigned char *front, *back;
    unsigned char first, last;

    for (block = 0; block < 2; block++) {
        for (index = 0; index < 0x400;) {
            front = data + (block * 0x800 + index);
            next = index + 1;
            back = data + ((block + 1) * 0x800 - next);
            first = *front;
            last = *back;
            first = ((first << 4) & 0xF0) | (first >> 4);
            last = ((last << 4) & 0xF0) | (last >> 4);
            *front = last;
            *back = first;
            index = next;
        }
    }
}

// FUNCTION: SCUS_943.58:0x80013C24
void func_80013C24(void)
{
    int result;
    unsigned char mode[8];

    do {
        mode[0] = 0xA0;
        result = CdControl(0x0E, mode, 0);
    } while (result != 1);
}

// FUNCTION: SCUS_943.58:0x80013AA0
void func_80013AA0(char **filenames, int count)
{
    int index;
    int offset;
    char **cursor;
    CdlFILE file;

    index = 1;
    if (index < count) {
        offset = 8;
        cursor = filenames + 1;
        do {
            CdSearchFile(&file, *cursor);
            cursor++;
            /* Copy the aligned CdlLOC representation as one word; direct
             * struct assignment makes GCC 2.7.2 emit unaligned transfers. */
            *(unsigned long *)((char *)&disc_file_records[0].pos + offset) =
                *(unsigned long *)&file.pos;
            index++;
            *(unsigned long *)((char *)&disc_file_records[0].size + offset) = file.size;
            offset += 8;
        } while (index < count);
    }
}

// FUNCTION: SCUS_943.58:0x80013B30
void func_80013B30(int file_index)
{
    CdControlF(0x15, (unsigned char *)&disc_file_records[file_index].pos);
}

// FUNCTION: SCUS_943.58:0x80013B60
int func_80013B60(int file_index, char *destination, int completion_address, int flags)
{
    unsigned long size;
    int slot;

    slot = (D_80045364 + 1) % 20;
    if (D_800502E8[slot].flags == 0) {
        D_800502E8[slot].flags = flags | 1;
        D_800502E8[slot].file_index = file_index;
        size = disc_file_records[file_index].size;
        D_80045364 = slot;
        D_800502E8[slot].destination = destination;
        /* The existing interface carries this pointer as a 32-bit address. */
        D_800502E8[slot].completion = (char **)completion_address;
        D_800502E8[slot].size = size;
        return disc_file_records[file_index].size;
    }
    return -1;
}

// FUNCTION: SCUS_943.58:0x80013FB8
void func_80013FB8(int index)
{
    int sector;
    int file_index;

    sector = CdPosToInt(&disc_file_records[D_800502E8[index].file_index].pos);
    file_index = D_800502E8[index].file_index;
    D_800502E8[index].sector = sector;
    D_800502E8[index].cursor = D_800502E8[index].destination;
    D_800502E8[index].remaining = D_800502E8[index].size;
    D_80045354 = CdPosToInt(&disc_file_records[file_index].pos);
    CdReadyCallback(func_800140A0);
    CdControl(6, (unsigned char *)&disc_file_records[D_800502E8[index].file_index].pos, 0);
    D_80045350 = VSync(-1);
}
