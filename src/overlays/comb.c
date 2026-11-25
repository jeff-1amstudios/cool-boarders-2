#include "comb.h"

extern int D_800570E0;
extern int func_8001F378(void *allocation);

extern void *func_8001F1A0(unsigned int size);
extern void *func_8007A104(int count, CVECTOR *color);
extern void func_8007ABC8(void *effects);
extern void func_80065278(void *first, void *second, int player, int mode);
extern void func_80078D80(int red, int green, int blue, int first, int second);
extern CombSettings *D_800A5D5C;
extern CVECTOR D_80098100;
extern CombPlayer *D_800A5B10[2];
extern void *D_8009FFB8[2];
extern unsigned char D_800A0044[];
extern void *D_800A4A0C[];
extern void *D_800A4A10[];
extern void *D_800A4A2C;
extern short D_800A4A3C;
extern unsigned char D_8009A2DC[];
extern unsigned char D_8009A2DD[];
extern unsigned char D_8009A2DE[];
extern short D_8009A358[];
extern short D_8009A35A[];
extern int D_8009FF90;
extern int D_8009FF94;
extern void func_8007AC50(void *effects);
extern void func_80079B90(void);

extern void func_8002F750(PACKET *work_base);
extern PACKET *D_800A4A20[2];
extern GsOT D_800A49E4[1][2];
extern GsOT *D_800A5D0C[1];

extern unsigned int D_8009A398;
extern int D_8009FFB4;

extern PACKET *D_80058ACC;
extern TILE D_800A4A50;
extern DR_MODE D_800A4A6C;
extern void func_80096D78(GsOT_TAG *ot, DR_MODE *mode);

extern int D_80058E10;
extern int D_80058E18;
extern int D_800A4A14;
extern short D_800A4A18;
extern short D_800A4A1A;
extern short D_800A4A1C;
extern short D_800A4A1E;

extern void func_8005F23C(GsOT *ot, CombPlayer *player, int first, int second);
extern int func_8005D910(GsOT *ot, int view, int first, int second, int mode);
extern void func_80080808(void);
extern void func_8005DE24(GsOT *ot, int player, RECT *clip,
                          unsigned char red, unsigned char green, unsigned char blue);
extern void func_80016748(void);
extern void func_80016AEC(void);
extern unsigned char D_8009A2D8[];
extern unsigned char D_8009A2D9[];
extern unsigned char D_8009A2DA[];

extern int D_800A4A28;
extern int D_800A4A90;
extern MATRIX D_80058E20;
extern CombSceneEntry *D_800A5AE8;
extern int D_800A5D64;
extern void func_8005E0D8(GsOT *ot, void *view, void *position, int entry,
                          int previous, int first, int last, int mode, int pass);

extern int D_80099ED0;
extern long D_8009FF88;
extern void func_8005ED84(CombPlayer *player);
extern void func_8006AC7C(GsOT *ot, CombPlayer *player, int shift);

extern short D_800980DC;
extern short D_800980DE[7];
extern short D_8009A4FC[2];
extern void *D_800A5D2C;
extern PACKET *func_8007F2D4(GsOT *ot, int mode, PACKET *packet, int shift);
extern void func_80080FD8(void);
extern void func_800810C4(void);
extern void func_80081184(void *texture);

extern int D_80099ED4;
extern void func_80080D78(GsOT *ot, int effect, int update, int mode,
                          VECTOR *position, int shift, void *scratch);

extern void func_8007E198(int value, short angle, int active, int mode, int extra);

extern short D_800A4A30;
extern int D_8009FF98;
extern int func_8006AC14(GsOT *ot, int side, int position, int mode, int extra);
extern void func_8006BBB4(GsOT *ot, int mode, int *state, int shift, int value);
extern void func_8006BB5C(GsOT *ot, int value, int *state, int shift, int extra);
extern void func_8006C2C8(GsOT *ot, int *state, int shift, int value);
extern void func_8006BC98(GsOT *ot, int *state, int shift);
extern void func_8006B528(GsOT *ot, CombHudEntry *entry, int *state, int shift, int value);
extern void func_8006C358(GsOT *ot, int *state, int shift, int value, int extra);
extern int func_8006AEEC(GsOT *ot, int *state, int shift, CombPlayerHudView *player,
                        int first, int second, int active, int mode);
extern void func_8006BEF8(GsOT *ot, void *state, int value, CombHudEntry *entries,
                         int count, int first, int second, int mode, int speed);
extern void func_8006B2D0(GsOT *ot, CombHudEntry *entry, int *state, int value, int shift, int extra);
extern void func_8006BCD4(GsOT *ot, int value, int mode, int first, int second);

extern int D_8009FFA8;
extern void func_8006BB20(GsOT *ot, int *state, int shift);
extern void func_8006BC40(GsOT *ot, int *state, int shift, int *value);
extern void func_8006BC04(GsOT *ot, int *state, int shift);
extern void func_8006AB40(GsOT *ot, unsigned int value, int first, int second);
extern void func_8006C4AC(GsOT *ot, int first, int second, int value);
extern void func_8006AB94(GsOT *ot, int shift);

extern int func_8006A9EC(GsOT *ot, CombPlayerRenderStateView *player,
                         unsigned int mask, int first, int second);

extern int D_8009A39C[10];

extern int *D_800A5AE0;
extern int D_800A4A7C[4];
extern int D_8009FFA4;

extern int func_8008DB08(int course, int *option, int active);

extern int func_80015F68(void);
extern void func_800172B8(void);

extern int func_80015F98(void);
extern void func_800173C0();

extern long D_800A5D60;
extern long D_800A5D68;

extern VECTOR D_800981C8;

extern int D_800981F8;
extern int D_800981FC;
extern int D_80098200;
extern void func_8006FC8C(CombSurfaceState *surface, VECTOR *position);

extern void func_8002EE40(long projection);
extern void func_800966A8(SVECTOR *rotation);
extern int func_80060E14(CombCameraPlayerView *player, int value, int update);
extern void func_80061440(CombCameraPlayerView *player, int speed, int update);

extern void func_8001E468(int bank, int category, int row);

extern void func_800962A0(int *destination, int *source);
extern int func_80095530(short x, short z);

extern int func_800702F4(CombSurfaceState *surface, int heading);

extern int D_80098204;
extern int D_80098208;

// FUNCTION: COMB.BIN:0x8005AEA4
void func_8005AEA4(FileLoadState *state)
{
    void *allocation;
    void **entry;
    int index;

    if (D_800570E0 < 2 && (index = 2, D_800570E0 >= 0)) {
        entry = (void **)&state->file_5c;
        do {
            allocation = *entry;
            if (allocation != 0 && (unsigned int)allocation < (unsigned int)state->unknown_04) {
                func_8001F378(allocation);
            }
            index++;
            entry++;
        } while (index < 16);
    }
}

// FUNCTION: COMB.BIN:0x8005B568
int func_8005B568(void)
{
    int i;
    int mode;
    int color_offset;
    int view_offset;
    int mask;
    int last;
    int state;
    int player;
    int offset;
    unsigned int size;
    int character = D_800A5D5C->character;
    int course = D_800A5D5C->course;
    CVECTOR color = D_80098100;

    for (i = 0; i < 2; i++) {
        D_800A5B10[i] = func_8001F1A0(0x4924);
        D_8009FFB8[i] = func_8001F1A0(0x1B8);
        if (D_800A5B10[i] == 0) return 0;
        if (D_8009FFB8[i] == 0) return 0;
        state = D_800A5D5C->course;
        D_800A5B10[i]->effects = func_8007A104(8, state == 6 ? &color : 0);
        func_8007ABC8(D_800A5B10[i]->effects);
        D_800A5B10[i]->buffer_1f4 = D_800A0044 + i * 0x1288;
        D_800A5B10[i]->buffer_1f8 = D_800A0044 + i * 0x1288 + 0x300;
        D_800A5B10[i]->buffer_1fc = D_800A0044 + i * 0x1288 + 0xD00;
        player = i != 0;
        if (i == 0) mask = -(character == 6);
        else {
            mode = D_800A5D5C->second_character;
            mask = -(mode == 6);
        }
        func_80065278((unsigned char *)D_800A5B10[i] + 0x1E4,
                      (unsigned char *)D_800A5B10[i] + 0xC60, player, mask);
    }
    for (i = 0, size = 0x20C0, offset = 0; i < 1; i++, offset += 8) {
        *(void **)((unsigned char *)D_800A4A0C + offset) = func_8001F1A0(size);
        if (*(void **)((unsigned char *)D_800A4A0C + offset) == 0) return 0;
        *(void **)((unsigned char *)D_800A4A10 + offset) = func_8001F1A0(size);
        if (*(void **)((unsigned char *)D_800A4A10 + offset) == 0) return 0;
    }
    D_800A4A2C = func_8001F1A0(0x50);
    if (D_800A4A2C == 0) return 0;
    state = course;
    color_offset = state * 8;
    state *= 4;
    view_offset = state;
    D_800A4A3C = course;
    last = *(short *)((unsigned char *)D_8009A35A + view_offset);
    mode = *(short *)((unsigned char *)D_8009A358 + view_offset);
    func_80078D80(D_8009A2DC[color_offset], D_8009A2DD[color_offset],
                  D_8009A2DE[color_offset], mode, last);
    /* The halfword view preserves the load before the independent zero store. */
    state = *(short *)((unsigned char *)D_800A5D5C + 0x1C);
    D_8009FF94 = 0;
    D_8009FF90 = state;
    return -1;
}

// FUNCTION: COMB.BIN:0x8005B808
void func_8005B808(void)
{
    int i;
    int offset;

    for (i = 0; i < 2; i++) {
        func_8007AC50(D_800A5B10[i]->effects);
        func_8001F378(D_8009FFB8[i]);
        func_8001F378(D_800A5B10[i]);
    }
    for (offset = 0; offset < 1; offset += 8) {
        func_8001F378(*(void **)((unsigned char *)D_800A4A0C + offset));
        func_8001F378(*(void **)((unsigned char *)D_800A4A10 + offset));
    }
    func_8001F378(D_800A4A2C);
    func_80079B90();
}

// FUNCTION: COMB.BIN:0x8005B8D0
void func_8005B8D0(void)
{
    int active;
    GsOT *ot;
    GsOT **current;
    int i;

    active = GsGetActiveBuff();
    func_8002F750(D_800A4A20[active]);
    i = 0;
    current = D_800A5D0C;
    ot = &D_800A49E4[0][active];
    do {
        *current++ = ot;
        ClearOTagR((u_long *)ot->org, 0x800);
        ot += 2;
        i++;
    } while (i < 1);
}

// FUNCTION: COMB.BIN:0x8005B968
unsigned int func_8005B968(unsigned int value, int mode)
{
    switch (mode) {
    case 0:
        D_8009A398 = value;
        break;
    case 1:
        if (D_8009A398 < ~value) {
            D_8009A398 += value;
        }
        break;
    }
    return D_8009A398;
}

// FUNCTION: COMB.BIN:0x8005BA08
void func_8005BA08(int value)
{
    if (value >= 0 && value != 1) {
        D_8009FFB4 = value;
    }
}

// FUNCTION: COMB.BIN:0x8005BB00
void func_8005BB00(GsOT *ot, unsigned char shade)
{
    if (shade != 0) {
        D_800A4A50.r0 = shade;
        D_800A4A50.g0 = shade;
        D_800A4A50.b0 = shade;
        *(TILE *)D_80058ACC = D_800A4A50;
        ((GsOT_TAG *)D_80058ACC)->p = ot->org[3].p;
        ot->org[3].p = (unsigned long)D_80058ACC;
        D_80058ACC += sizeof(TILE);
        func_80096D78(&ot->org[3], &D_800A4A6C);
    }
}

// FUNCTION: COMB.BIN:0x8005BBD8
void func_8005BBD8(int mode)
{
    switch (mode) {
    case 1:
        ((short *)&D_800A4A14)[0] = D_80058E10 / 4;
        ((short *)&D_800A4A14)[1] = D_80058E18 / 2;
        D_800A4A18 = D_80058E10 * 3 / 4;
        D_800A4A1A = D_80058E18 / 2;
        D_800A4A18 = 0;
        D_800A4A1A = 0;
        D_800A4A1C = D_80058E10 / 2;
        D_800A4A1E = D_80058E18;
        ((short *)D_800A4A20)[0] = D_80058E10 / 2;
        ((short *)D_800A4A20)[1] = 0;
        ((short *)D_800A4A20)[2] = D_80058E10 / 2;
        ((short *)D_800A4A20)[3] = D_80058E18;
        break;
    case 2:
        ((short *)&D_800A4A14)[0] = D_80058E10 / 2;
        ((short *)&D_800A4A14)[1] = D_80058E18 / 4;
        D_800A4A18 = D_80058E10 / 2;
        D_800A4A1A = D_80058E18 * 3 / 4;
        D_800A4A18 = 0;
        D_800A4A1A = 0;
        D_800A4A1C = D_80058E10;
        D_800A4A1E = D_80058E18 / 2;
        ((short *)D_800A4A20)[0] = 0;
        ((short *)D_800A4A20)[1] = D_80058E18 / 2;
        ((short *)D_800A4A20)[2] = D_80058E10;
        ((short *)D_800A4A20)[3] = D_80058E18 / 2;
        break;
    default:
        D_800A4A18 = 0;
        D_800A4A1A = 0;
        ((short *)&D_800A4A14)[0] = D_80058E10 / 2;
        ((short *)&D_800A4A14)[1] = D_80058E18 / 2;
        D_800A4A1C = D_80058E10;
        D_800A4A1E = D_80058E18;
        break;
    }
}

// FUNCTION: COMB.BIN:0x8005D774
void func_8005D774(int first, int second, int mode)
{
    int result[2];
    int view;
    int offset;
    int color_offset;
    int sync_mode;
    RECT *clip;
    GsOT **current;
    GsOT *ot;

    view = 0;
    func_8005F23C(D_800A5D0C[0], D_800A5B10[0], first, second);
    result[0] = func_8005D910(D_800A5D0C[0], 0, first, second, mode);
    if (first == second) {
        func_80080808();
    }
    sync_mode = D_8009FFB4;
    VSync(sync_mode);
    current = D_800A5D0C;
    offset = 0;
    ResetGraph(1);
    GsSwapDispBuff();
    do {
        GsSetOffset(*(short *)((unsigned char *)&D_800A4A14 + offset),
                    *(short *)((unsigned char *)&D_800A4A14 + offset + 2));
        clip = (RECT *)(&D_800A4A18 + view * 4);
        GsSetClip(clip);
        if (*(int *)((unsigned char *)result + offset) == -1) {
            color_offset = D_800A4A3C * 8;
            GsSortClear(D_8009A2D8[color_offset], D_8009A2D9[color_offset],
                        D_8009A2DA[color_offset], D_800A5D0C[0]);
        } else {
            func_8005DE24(*current, view, clip, 0, 0, 0);
        }
        ot = *current++;
        offset += 4;
        view++;
        DrawOTag((u_long *)(ot->org + 0x7FF));
    } while (view < 1);
    func_80016748();
    func_80016AEC();
}

// FUNCTION: COMB.BIN:0x8005DE24
void func_8005DE24(GsOT *ot, int player, RECT *clip,
                   unsigned char red, unsigned char green, unsigned char blue)
{
    TILE *tile = (TILE *)D_80058ACC;

    ((GsOT_TAG *)tile)->num = 3;
    tile->code = 0x60;
    tile->r0 = red;
    tile->g0 = green;
    tile->b0 = blue;
    tile->x0 = -clip->w / 2;
    tile->y0 = -clip->h / 2;
    tile->w = clip->w;
    tile->h = clip->h;
    ((GsOT_TAG *)tile)->p = ot->org[0x7FF].p;
    ot->org[0x7FF].p = (unsigned long)tile;
    D_80058ACC = (PACKET *)(tile + 1);
}

// FUNCTION: COMB.BIN:0x8005DEE4
int func_8005DEE4(int mode, int value)
{
    int result = -1;

    switch (mode) {
    case 0:
        if ((unsigned int)value - 0x71 < 0x11) {
            result = 0;
        }
        break;
    case 7:
        if ((unsigned int)value - 0x30 < 5) {
            result = 0;
        }
        break;
    case 8:
        if ((unsigned int)value - 100 < 6) {
            result = 0;
        }
        break;
    case 9:
        if ((unsigned int)value - 4 < 0x30 ||
            (unsigned int)value - 0x39 < 0x10 ||
            (unsigned int)value - 0x50 < 0x73) {
            result = 0;
        }
        break;
    }
    return result;
}

// FUNCTION: COMB.BIN:0x8005DF88
void func_8005DF88(GsOT *ot, void *view, void *position, int mode, int entry)
{
    int i;
    int linked;
    int fill = -1;

    D_800A4A28 = 0;
    for (i = 19; i >= 0; i--) {
        ((int *)D_800A4A2C)[i] = fill;
    }
    i = 0;
    GsSetLightMatrix(&D_80058E20);
    D_800A4A90 = -1;
    do {
        linked = D_800A5AE8[entry].linked_entries[i];
        if (linked >= 0) {
            func_8005E0D8(ot, view, position, linked, -1, 0, 1, 0, 0);
        }
        i++;
    } while (i < 3);
    D_800A4A90 = entry;
    func_8005E0D8(ot, view, position, entry, -1, 0, D_800A5D64, mode, 2);
}

// FUNCTION: COMB.BIN:0x8005E7A4
void func_8005E7A4(GsOT *ot, CombPlayer *player, int index)
{
    MATRIX local_screen;
    MATRIX local_world;
    MATRIX saved;
    VECTOR scale;
    VECTOR temporary;
    int count = ((CombRenderView *)player)->object_count;
    GsDOBJ3 *objects = ((CombRenderView *)player)->objects;
    int forward = ((CombRenderView *)player)->forward_order;
    int shift;
    int i;
    int offset;
    GsDOBJ3 *object;
    SVECTOR rotation;
    int value;
    int invalid;

    saved = ((CombRenderView *)player)->coordinate.coord;
    shift = D_80099ED0 + 3;
    memset_800449E8((unsigned char *)&temporary, 0, sizeof(temporary));
    temporary.vx = (&D_8009FF88)[index];
    temporary.vy = (&D_8009FF88)[index];
    temporary.vz = (&D_8009FF88)[index];
    i = 0;
    scale = temporary;
    ScaleMatrix(&((CombRenderView *)player)->coordinate.coord, &scale);
    func_8005ED84(player);
    if (count > 0) {
        offset = 0;
        do {
            object = (GsDOBJ3 *)((unsigned char *)objects + offset);
            if (forward == 0) {
                value = count - 1;
                value -= i;
                invalid = value * 24;
                object = (GsDOBJ3 *)((unsigned char *)objects + invalid);
            }
            value = object->id;
            invalid = -1;
            if (value != invalid && object->pmd != 0) {
                object->attribute = 0;
                GsGetLws(object->coord2, &local_world, &local_screen);
                GsSetLsMatrix(&local_screen);
                GsSortObject3(object, ot, shift);
            }
            i++;
            offset += sizeof(GsDOBJ3);
        } while (i < count);
    }
    GsGetLs(&((CombRenderView *)player)->coordinate, &local_screen);
    GsSetLsMatrix(&local_screen);
    ot->org += 4;
    func_8006AC7C(ot, player, shift);
    ot->org -= 4;
    ((CombRenderView *)player)->coordinate.coord = saved;
}

// FUNCTION: COMB.BIN:0x8005E9E0
void func_8005E9E0(GsOT *ot, int shift, int kind, int entry)
{
    int offset = 0;

    do {
        if (kind == *(short *)((unsigned char *)&D_800980DC + offset) &&
            entry == *(short *)((unsigned char *)D_800980DE + offset)) {
            switch (kind) {
            case 2:
                D_80058ACC = func_8007F2D4(ot, D_8009A4FC[0], D_80058ACC, shift);
                break;
            case 7:
                func_80080FD8();
                break;
            case 9:
                func_800810C4();
                break;
            }
        }
        offset += 4;
    } while (offset < 16);
    if (kind == 9 && ((unsigned int)(entry - 142) < 3 ||
                      (unsigned int)(entry - 180) < 5)) {
        func_80081184(D_800A5D2C);
    }
}

// FUNCTION: COMB.BIN:0x8005EB18
void func_8005EB18(GsOT *ot, CombPlayer *player)
{
    int offset = 0;
    int effect = -1;
    int update = 0;
    int shift = D_80099ED0 + 3;
    int course = D_800A5D5C->course;
    int entry = ((CombPlayerEffectView *)player)->entry;
    VECTOR *position = &((CombPlayerEffectView *)player)->position;

    switch (course) {
    case 3:
        if ((unsigned int)(entry - 99) < 12) {
            effect = 0;
        } else {
            offset = 8;
            if (entry < 148) goto render;
            effect = 1;
        }
        update = -1;
        offset = 8;
        break;
    case 4:
        if (entry == 83) {
            effect = 0;
            update = -1;
            offset = 48;
        }
        break;
    }
render:
    if ((unsigned int)(*(int *)(D_800A5D5C->unknown_16 + 2) - 1) < 2 && effect >= 0) {
        GsOT_TAG *org = ot->org;
        int scratch_offset = D_80099ED4 * 4;
        int mode = D_8009A4FC[0];
        ot->org = org + offset;
        func_80080D78(ot, effect, (update << 16) >> 31, mode,
                      position, shift, (void *)(0x1F800000 + scratch_offset));
        ot->org -= offset;
    }
}

// FUNCTION: COMB.BIN:0x8005EC58
int func_8005EC58(VECTOR *origin, MATRIX *matrix, VECTOR *normal)
{
    int x, y, z;
    VECTOR difference;
    int dx, dy, dz;
    int nx, ny, nz;
    int offset, first, second;

    if (origin) {
        x = origin->vx;
        y = origin->vy;
        z = origin->vz;
    } else {
        x = 0;
        y = 0;
        z = 0;
    }
    nx = normal->vx;
    ny = normal->vy;
    nz = normal->vz;
    dx = x - nx;
    dy = y - ny;
    dz = z - nz;
    offset = -(dx * nx + dy * ny + dz * nz);
    first = x * nx + y * ny + z * nz + offset;
    second = matrix->t[0] * nx + matrix->t[1] * ny + matrix->t[2] * nz + offset;
    difference.vx = dx;
    difference.vy = dy;
    difference.vz = dz;
    if ((first > 0 && second >= 0) || (first < 0 && second <= 0)) {
        return -1;
    }
    return 0;
}

// FUNCTION: COMB.BIN:0x8005EDC4
void func_8005EDC4(CombPlayerHeadingView *view, int value)
{
    int mode = view->mode;
    int extra = view->value_724;
    int active = -(unsigned int)(view->state_96 == 0);
    int angle;

    angle = ratan2(-view->motion_x, -view->motion_z) - view->heading;
    angle = (angle << 20) >> 20;
    switch (D_800A5D5C->course) {
    case 3:
    case 5:
    case 9:
        if (view->state_148 == -1 && active == -1) {
            mode = 4;
        }
        break;
    }
    if ((unsigned int)(*view->surface - 3) < 2) {
        active = 0;
    }
    func_8007E198(value, (short)angle, active, mode, extra);
}

// FUNCTION: COMB.BIN:0x8005EEE0
void func_8005EEE0(GsOT *ot, CombPlayerHudView *player, int first, int second)
{
    CombHudState *hud;
    CombMovementHudView *movement;
    int entry = player->entry_count - 1;
    int shift = -(unsigned int)(first != second);
    int surface;
    int value;
    int speed;
    unsigned char before;
    int angular_speed;

    hud = &player->hud;
    movement = &player->movement;
    if (entry < 0) entry = 0;
    surface = *player->surface;
    if (surface == 1 || *(int *)(D_800A5D5C->unknown_16 + 2) == 6) {
        player->display_position = func_8006AC14(ot, player->display_side,
                                                player->display_position, 0, 0);
    } else {
        player->display_position = 200;
    }
    func_8006BBB4(ot, 2, &hud->value_1c, shift, hud->value_20);
    func_8006BB5C(ot, hud->value_08, &hud->value_04, shift, player->value_491c);
    func_8006C2C8(ot, &hud->value_3c, shift, player->value_cd6);
    func_8006BC98(ot, &hud->value_0c, shift);
    value = 120;
    if (D_800A4A30 != 0) {
        func_8006B528(ot, (CombHudEntry *)((int)player->entries + entry * 0x34), &hud->value_4c, shift, value);
    }
    if (second != 3) {
        angular_speed = movement->angular_speed;
        if (angular_speed != 0) {
            value = angular_speed * 180;
            if (value < 0) value = -value;
            func_8006C358(ot, &hud->value_44, shift, value, hud->value_48);
        }
        if (second != 4) {
            value = 0;
            if (surface != 3 && surface != 5 && first != 5 && player->state >= 5) {
                before = player->previous_progress < player->progress;
                if (before || hud->countdown != 0) {
                    value = -1;
                    if (!before && shift == 0 && hud->countdown != 0) {
                        hud->countdown--;
                    }
                }
            }
            hud->value_58 = func_8006AEEC(ot, &hud->value_50, shift, player,
                                          hud->value_5c, hud->value_58, value, 3);
        }
        value = D_8009FF98;
        if (value == 4) speed = player->value_cd6;
        else speed = movement->speed;
        func_8006BEF8(ot, player->display_e10, player->value_cb6, player->entries,
                      player->entry_count, 0, 0, value, speed);
        if (D_8009FF98 == 1 && hud->value_10 >= 0) hud->value_10 = -1;
        func_8006B2D0(ot, (CombHudEntry *)((int)player->entries + entry * 0x34), &hud->value_10, hud->value_14, shift, 0);
        func_8006BCD4(ot, player->value_491c, 1, 0, 0);
    }
}

// FUNCTION: COMB.BIN:0x8005F23C
void func_8005F23C(GsOT *ot, CombPlayer *player, int first, int second)
{
    CombPlayerHudView *view = (CombPlayerHudView *)player;
    int shift = -(unsigned int)(first != second);
    int first_state = ((CombPlayerHudView *)D_800A5B10[0])->state;
    int second_state = ((CombPlayerHudView *)D_800A5B10[1])->state;
    int first_value = ((CombPlayerHudView *)D_800A5B10[0])->hud.unknown_40;
    int second_value = ((CombPlayerHudView *)D_800A5B10[1])->hud.unknown_40;

    func_8006BB20(ot, &view->hud.unknown_00, shift);
    func_8006BC40(ot, (int *)(view->hud.unknown_24 + 4), shift, &D_8009FF90);
    func_8006BBB4(ot, 1, &view->hud.unknown_18, shift, 0);
    func_8006BC04(ot, (int *)view->hud.unknown_24, shift);
    if (second != 3) {
        if (view->hud.unknown_40 >= 0) {
            int value = first_value;
            if (second_value < value) value = second_value;
            func_8006AB40(ot, value, 0, 0);
        }
        func_8006C4AC(ot, first_state, second_state,
                      ((CombPlayerRaceHudView *)player)->race->value_5c);
    } else {
        int mode = D_8009FFA8;
        if (mode < 8) {
            if (mode > 5) func_8006AB94(ot, shift);
        }
    }
}

// FUNCTION: COMB.BIN:0x8005FB48
void func_8005FB48(int mode, unsigned int mask)
{
    CombPlayerRenderStateView *player;
    unsigned int active_mask;
    int index = 0;
    int zero = 0;
    GsOT **current = D_800A5D0C;

    do {
        player = (CombPlayerRenderStateView *)D_800A5B10[index];
        active_mask = 0;
        if (index == 0 && mode == 2) {
            active_mask = mask & -(unsigned int)(*player->surface != 3);
        }
        player->result = func_8006A9EC(*current, player, active_mask, zero, zero);
        index++;
    } while (index < 2);
}

// FUNCTION: COMB.BIN:0x8005FC1C
int func_8005FC1C(int course, int value, int difference, int penalty)
{
    int result;

    value += 2500;
    value += (D_8009A39C[course] - difference) / 20;
    if (penalty != 0) {
        result = value - penalty * 10;
    } else {
        result = value + 500;
    }
    if (result < 0) {
        result = 0;
    }
    return result;
}

// FUNCTION: COMB.BIN:0x8006009C
void func_8006009C(CombSetupState *state, CombSetupSource *source, int *progress)
{
    state->values = source->values;
    state->word_0c = source->word_0c;
    state->value = source->value;
    state->link = source->link;
    state->link->value = state->value;
    state->link->progress = progress;
}

// FUNCTION: COMB.BIN:0x800600E8
unsigned int func_800600E8(unsigned int value, int time)
{
    int *bonuses;
    CombPlayer **players = D_800A5B10;
    CombPlayer **cursor;
    int second_side = 0;
    int first_side = 0;
    unsigned int index;
    int other;
    int checkpoint;
    int previous;
    int last_checkpoint;
    int last_time;
    CombPlayerCheckpointView *player;
    index = 0;
    bonuses = D_800A5AE0;

    cursor = players;
    for (; (int)index < 2; index++, cursor++) {
        player = (CombPlayerCheckpointView *)*cursor;
        checkpoint = player->checkpoint;
        previous = player->previous_checkpoint;
        if (checkpoint < 4) {
            player->checkpoint_times[checkpoint] = time;
            if (player->side == 1) D_800A4A7C[checkpoint] = time;
        }
        if (checkpoint >= 2 && checkpoint > previous) {
            if (checkpoint <= D_8009FFA4 && checkpoint < 4) {
                player->comparison_state = 0;
            }
            if (checkpoint > D_8009FFA4 && checkpoint < 4) {
                value += bonuses[checkpoint - 1];
                D_8009FFA4 = checkpoint;
                for (other = 0; other < 2; other++) {
                    ((CombPlayerCheckpointView *)players[other])->checkpoint_reset = 0;
                }
            }
            if (index < 2 && checkpoint >= 4) {
                player->finished_value = player->checkpoint_value;
            }
        } else if (player->comparison_state == 0) {
            player->comparison_state = -1;
        }
    }
    last_checkpoint = 0;
    last_time = 0;
    for (index = 0; (int)index < 2; index++) {
        player = (CombPlayerCheckpointView *)D_800A5B10[index];
        if (player->side == 1) first_side = index;
        if (player->comparison_state == 0) {
            other = player->checkpoint - 1;
            player->time_difference = player->checkpoint_times[other] - D_800A4A7C[other];
            if (player->side == 2) {
                last_checkpoint = other;
                last_time = player->checkpoint_times[other];
                second_side = index;
            }
        }
    }
    if (last_checkpoint != 0) {
        CombPlayerCheckpointView *player;
        player = (CombPlayerCheckpointView *)D_800A5B10[first_side];
        player->time_difference = D_800A4A7C[last_checkpoint] - last_time;
        player->comparison_state = 0;
        if (player->time_difference == 0) {
            CombPlayerCheckpointView *second_player = (CombPlayerCheckpointView *)D_800A5B10[second_side];
            second_player->time_difference = 0;
            second_player->comparison_state = 0;
        }
    }
    return value;
}

// FUNCTION: COMB.BIN:0x80060328
static __inline__ int comb_result_score(int course, int value, int difference, int penalty)
{
    int result;
    value += 2500;
    value += (D_8009A39C[course] - difference) / 20;
    if (penalty != 0) result = value - penalty * 10;
    else result = value + 500;
    if (result < 0) result = 0;
    return result;
}

void func_80060328(CombPlayerResultView *first, CombPlayerResultView *second, int result)
{
    int first_total = 0;
    int second_total = 0;
    int i = 0;
    int bank = *(int *)(D_800A5D5C->unknown_16 + 2) != 3;
    int first_time = first->finish_time - first->start_time;
    int second_time = second->finish_time - second->start_time;
    int first_finished = first->finished;
    int second_finished = second->finished;
    int count = first->entry_count;
    int course = D_800A5D5C->course;
    CombResultWork work;
    CombResultEntry *second_entry;
    int first_score;
    int second_score;
    int tied;
    int first_rank;
    int second_rank;

    if (count > 0) {
        second_entry = second->entries;
        work.entry = first->entries;
        do {
            second_total += second_entry->score;
            first_total += work.entry->score;
            second_entry++;
            work.entry++;
            i++;
        } while (i < count);
    }
    {
        int score;
        int penalty;
        work.value = first_total + 2500;
        work.value += (D_8009A39C[course] - first_time) / 20;
        penalty = first->penalty;
        if (penalty != 0) score = work.value - penalty * 10;
        else score = work.value + 500;
        if (score < 0) score = 0;
        first_score = score;
    }
    second_score = comb_result_score(course, second_total, second_time, second->penalty);

    first_rank = 1;
    second_rank = 1;
    if (result == 0) {
        switch (D_8009FF90) {
        case 0:
        case 1:
            if (first_finished == 0) first_rank = 2;
            else if (second_finished == 0) second_rank = 2;
            break;
        case 2:
            if (first_finished == 0) first_rank = 2;
            else if (second_finished == 0) second_rank = 2;
            else if (first_total != second_total) {
                if (first_total > second_total) second_rank = 2;
                else first_rank = 2;
            }
            break;
        case 3:
        default:
            do {
                tied = first_score == second_score;
            } while (0);
            if (!tied) {
                if (first_score > second_score) {
                    first_rank = 1;
                    second_rank = 2;
                } else {
                    first_rank = 2;
                    second_rank = 1;
                }
            }
            break;
        }
    } else if (result == 1) first_rank = 2;
    else if (result == 2) second_rank = 2;
    first->rank = first_rank;
    first->result->rank = first_rank;
    second->rank = second_rank;
    second->result->rank = second_rank;
    if (first_rank != second_rank) {
        if (first_rank == 1) D_8009FF94 = 0;
        else D_8009FF94 = 1;
    } else D_8009FF94 = -1;
    func_8001E468(bank, D_8009FF90, D_8009FF94);
}

// FUNCTION: COMB.BIN:0x800605A8
int func_800605A8(void)
{
    int active = *(int *)(D_800A5D5C->unknown_16 + 2) != 3;
    int result;
    int *option;

    if (((CombPlayerCheckpointView *)D_800A5B10[0])->side ==
        ((CombPlayerCheckpointView *)D_800A5B10[1])->side) {
        *(long *)&D_8009FF94 = -1;
    }
    option = &D_8009FF90;
    result = func_8008DB08(D_800A5D5C->course, option, active);
    D_800A5D5C->option = *option;
    if (result != 1) {
        if (result < 2) {
            if (result == 0) {
                result = 2;
            } else {
                result = 0;
            }
        } else {
            result = 0;
        }
    }
    return result;
}

// FUNCTION: COMB.BIN:0x80060660
int func_80060660(CombPlayerCheckpointView *player)
{
    CombSettings *settings = D_800A5D5C;
    int active = *(int *)(settings->unknown_16 + 2) != 3;
    int result;
    int *option;

    player->finished_value = player->checkpoint_value;
    option = &D_8009FF90;
    result = func_8008DB08(settings->course, option, active);
    D_800A5D5C->option = *option;
    if (result != 1) {
        if (result < 2) {
            if (result == 0) {
                result = 2;
            } else {
                result = 0;
            }
        } else {
            result = 0;
        }
    }
    return result;
}

// FUNCTION: COMB.BIN:0x800606F0
void func_800606F0(void)
{
    func_80015F68();
    func_800172B8();
    SpuSetKey(0, 0x780FF);
}

// FUNCTION: COMB.BIN:0x80060728
void func_80060728(void)
{
    int selected;

    func_80015F98();
    selected = *(short *)D_800A5D5C->unknown_16;
    if (selected >= 0) {
        func_800173C0(selected, 2);
    }
}

// FUNCTION: COMB.BIN:0x800607B4
void func_800607B4(CombMotionState *state, short mode, int value, int x, int y, int z)
{
    int i;

    for (i = 0; i < 16; i++) {
        state->history_a[i] = 0;
        state->history_b[i] = 0;
    }
    state->angle_buffer = state->angles;
    for (i = 3; i >= 0; i--) {
        state->angles[i] = 0;
    }
    state->mode = mode;
    state->value = value;
    state->samples[0].x = x;
    state->samples[0].y = y;
    state->samples[0].z = z;
    state->samples[0].rotation.vx = 0;
    state->samples[0].rotation.vy = 0;
    state->samples[0].rotation.vz = 0;
    state->samples[0].active = 1;
    state->negative_height = -state->height;
    for (i = 1; i < 4; i++) {
        state->samples[i] = state->samples[0];
    }
    state->selected = -1;
    state->timer = -10;
    state->word_124 = 0;
    state->word_128 = 0;
    state->word_190 = 0;
    state->word_194 = 0;
    state->word_198 = 0;
    state->word_19c = 0;
    state->word_1a0 = 0;
    ReadGeomOffset(&D_800A5D60, &D_800A5D68);
}

// FUNCTION: COMB.BIN:0x80060BD8
void func_80060BD8(CombMotionState *state, int *position)
{
    unsigned int selected;

    selected = state->selected + 1;
    selected &= -(unsigned int)((int)selected < 4);
    state->selected = selected;
    state->samples[selected].x = position[0];
    state->samples[selected].y = position[1];
    state->samples[selected].z = position[2];
}

// FUNCTION: COMB.BIN:0x80060C24
int func_80060C24(CombMotionState *state, int sample, int target)
{
    short *history = state->angles;
    int i;
    int delta;
    int j;
    int difference;
    int adjusted;

    delta = history[0] - target;
    for (i = 1; i < 4; i++) {
        if (history[i] == target) {
            continue;
        }
        j = history[i] - target;
        if (delta == 0) {
            delta = j;
        } else if (delta * j < 0) {
            delta = 0;
            break;
        }
    }
    if (delta != 0) {
        if (i != 4) {
            for (j = 3; j >= 0; j--) {
                history[j] = target;
            }
        } else if (delta > 0) {
            for (i = 0; i < 4; i++) {
                if (history[i] > sample) {
                    for (j = i; j < 4; j++) {
                        history[j] = sample;
                    }
                    break;
                }
            }
        } else {
            for (i = 0; i < 4; i++) {
                if (history[i] < sample) {
                    for (j = i; j < 4; j++) {
                        history[j] = sample;
                    }
                    break;
                }
            }
        }
    }
    delta = sample;
    j = history[0];
    for (i = 0; i < 3; i++) {
        history[i] = history[i + 1];
    }
    history[i] = delta;
    delta = j;
    if (delta >= target) {
        difference = delta - target;
        if (difference > 5) {
            delta = difference;
            delta /= 64;
            adjusted = target + 5;
            target = adjusted + delta;
        } else {
            target = delta;
        }
    } else {
        difference = target - delta;
        if (difference > 5) {
            delta = difference;
            delta /= 64;
            adjusted = target - 5;
            target = adjusted - delta;
        } else {
            target = delta;
        }
    }
    return target;
}

// FUNCTION: COMB.BIN:0x80061440
void func_80061440(CombCameraPlayerView *camera_player, int progress, int frozen)
{
    CombPlayerMotionView *player = (CombPlayerMotionView *)camera_player;
    VECTOR delta;
    short saved_state;
    short airborne;
    CombMotionState *state;
    CombMotionSample *current;
    CombMotionSample *next;
    VECTOR *position;
    long slope;
    int heading;
    int height;
    int angle;
    int index;
    int bounded;
    int difference;
    int mode;

    state = player->motion;
    saved_state = player->state;
    mode = (unsigned short)state->mode;
    heading = state->value;
    height = state->negative_height;
    airborne = player->airborne;
    position = &player->position;
    if (!frozen) {
        index = state->selected - 3;
        if (index < 0) index = state->selected + 1;
        current = &state->samples[state->selected];
        next = &state->samples[index];
        difference = current->x - next->x;
        angle = difference * difference;
        difference = current->z - next->z;
        angle = SquareRoot0(angle + difference * difference);
        if (angle >= 11) angle = ratan2(current->y - next->y, angle);
        else angle = 0;
        slope = angle * 3600 >> 12;
        if (slope > 0) {
            slope = slope > 800 ? 800 : slope;
        } else {
            slope = slope < -800 ? -800 : slope;
        }
        mode = func_80060C24(state, slope, (short)mode);
        index = state->selected + 1;
        current = state->samples;
        if (index < 4) current += index;
        delta.vx = current->x - position->vx;
        delta.vy = current->y - position->vy;
        delta.vz = current->z - position->vz;
        if (player->progress < progress) {
            slope = ratan2(delta.vx, delta.vz);
        } else if (player->reverse == 0) {
            slope = player->heading;
        } else {
            slope = player->heading - 2048;
        }
        difference = slope - heading;
        difference = difference << 20 >> 20;
        if ((unsigned int)(difference + 91) > 182) {
            if (difference > 0) heading += 91;
            else heading -= 91;
        } else heading = slope;
        heading = heading << 20 >> 20;
        if (player->vertical_motion < 0 && airborne != 0) {
            height += 10;
            difference = -state->unknown_086;
            bounded = difference < height;
        } else {
            height -= 5;
            difference = -state->height;
            bounded = height < difference;
        }
        if (bounded) height = difference;
    }
    ((CombMotionTransformView *)state)->matrix = D_80058E20;
    slope = ((short)mode << 12) / 3600;
    RotMatrixX(slope, &((CombMotionTransformView *)state)->matrix);
    RotMatrixY(heading, &((CombMotionTransformView *)state)->matrix);
    ((CombMotionTransformView *)state)->matrix.t[1] = height;
    ((CombMotionTransformView *)state)->word_7c = 0;
    ((CombMotionTransformView *)state)->word_34 = 0;
    ((CombMotionTransformView *)state)->rotation.vx = slope;
    ((CombMotionTransformView *)state)->rotation.vy = heading;
    ((CombMotionTransformView *)state)->rotation.vz = 0;
    state->mode = mode;
    state->value = heading;
    state->negative_height = height;
    *(int *)(state + 1) = saved_state;
}

// FUNCTION: COMB.BIN:0x80060E14
int func_80060E14(CombCameraPlayerView *camera_player, int active, int frozen)
{
    CombPitchMotionPlayer *player = (CombPitchMotionPlayer *)camera_player;
    CombPitchMotionState *state = &player->motion->motion;
    unsigned int pitch = (unsigned short)state->mode;
    int heading = state->value;
    short *history = state->angle_buffer;
    int position[3];
    int i, j, sample, next_direction;
    int *point;

    int selected, denominator, decay;
    CombPitchWork angle;
    MATRIX *matrix;

    if (frozen == 0) {
        selected = state->selected + 1;
        point = &state->samples[0].x;
        if (selected < 4) {
            point += selected * 6;
        }
        position[0] = player->position[0] - point[0];
        position[1] = player->position[1] - point[1];
        position[2] = player->position[2] - point[2];
        func_800962A0(position, position);
        denominator = func_80095530((short)position[0], (short)position[2]);
        if (position[2] > 0) {
            denominator = -denominator;
        }
        if (denominator != 0) {
            angle.value = position[1];
            angle.bits = angle.value << 12;
            angle.value = catan(angle.value / denominator);
        } else {
            angle.value = 0;
        }
        if (player->reverse != 0) {
            angle.value = -angle.value;
        }
        sample = (angle.value * 3600) >> 12;
        angle.value = 0;
        if (history[0] != (short)pitch) {
            angle.value = -1;
            if ((short)pitch < history[0]) {
                angle.value = 1;
            }
        }
        for (i = 1; i < 4; i++) {
            if (history[i] == (short)pitch) {
                continue;
            }
            next_direction = -1;
            if ((short)pitch < history[i]) {
                next_direction = 1;
            }
            if (angle.value == 0) {
                angle.value = next_direction;
            } else if (angle.value * next_direction < 0) {
                angle.value = 0;
                break;
            }
        }
        if (angle.value != 0) {
            if (i != 4) {
                for (j = 3; j >= 0; j--) {
                    history[j] = pitch;
                }
            } else if (angle.value > 0) {
                for (i = 0; i < 4; i++) {
                    if (history[i] > sample) {
                        for (j = i; j < 4; j++) {
                            history[j] = sample;
                        }
                        break;
                    }
                }
            } else {
                for (i = 0; i < 4; i++) {
                    if (history[i] < sample) {
                        for (j = i; j < 4; j++) {
                            history[j] = sample;
                        }
                        break;
                    }
                }
            }
        }
        angle.value = sample;
        j = history[0];
        for (i = 0; i < 3; i++) {
            history[i] = history[i + 1];
        }
        history[i] = angle.value;
        angle.value = j;
        if (state->word_1a0 != 0 && active != 0 && state->word_19c <= 8) {
            i = 15;
            j = 5;
            if (active > 0) {
                j = 15;
            }
        } else {
            i = 5;
            j = 5;
        }
        if (angle.value >= (short)pitch) {
            if (state->word_1a0 == -2) {
                state->word_1a0 = 0;
            }
            if (angle.value - (short)pitch > i) {
                pitch += j;
            } else {
                pitch = angle.value;
            }
        } else {
            if (state->word_1a0 == -1) {
                state->word_1a0 = -2;
            }
            if ((short)pitch - angle.value > i) {
                pitch -= j;
            } else {
                pitch = angle.value;
            }
        }
        if (active > 0) {
            if (state->word_194 == 0) {
                state->word_198 = 100;
                state->word_19c = 10;
                state->word_1a0 = -1;
                state->word_194 = ~state->word_194;
            } else if (state->word_19c != 0) {
                state->word_19c--;
            } else {
                decay = state->word_198;
                if (decay > 10) {
                    decay = 10;
                }
                state->word_198 -= decay;
            }
            if ((unsigned int)(state->word_19c - 7) < 2) {
                pitch += 60;
            }
            if (state->word_19c == 0) {
                active--;
            }
        } else {
            active = 0;
            state->word_194 = 0;
            state->word_198 = 0;
            state->word_19c = 0;
            state->word_1a0 = 0;
        }
        if ((short)pitch > 800) {
            pitch = 800;
        }
        if ((short)pitch < -800) {
            pitch = -800;
        }
        if (player->reverse == 0) {
            angle.value = player->heading;
            i = angle.value - heading;
        } else {
            angle.value = player->heading - 2048;
            i = angle.value - heading;
        }
        i = (i << 20) >> 20;
        if ((player->airborne != 0 && player->movement != 0) ||
            (unsigned int)(i + 169) < 339) {
            heading = angle.value;
        } else if (i > 0) {
            heading += 170;
        } else {
            heading -= 170;
        }
        heading = (heading << 20) >> 20;
    }
    state->matrix = D_80058E20;
    sample = ((short)pitch << 12) / 3600;
    if (player->reverse == 0) {
        i = player->rotation_x;
    } else {
        i = -player->rotation_x;
    }
    matrix = &state->matrix;
    state->rotation.vx = sample + i;
    state->rotation.vy = 0;
    state->rotation.vz = 0;
    RotMatrixX(sample, matrix);
    RotMatrixY(heading - player->rotation_y, matrix);
    RotMatrixX(player->rotation_x, matrix);
    RotMatrixY(player->rotation_y, matrix);
    state->word_07c = 0;
    state->word_034 = 0;
    state->mode = pitch;
    state->value = heading;
    state->matrix.t[1] = -state->height - state->translation_y;
    ((CombMotionExtended *)state)->saved_height = player->height;
    return active;
}

// FUNCTION: COMB.BIN:0x80061A64
void func_80061A64(CombMotionPlayer *player, int *position)
{
    CombMotionTransform *transform = (CombMotionTransform *)player->motion;
    int z;
    int player_z;

    transform->coordinate.coord.t[0] = position[0] - player->position[0];
    transform->coordinate.coord.t[1] = position[1] - player->position[1] - player->motion->height;
    z = position[2];
    player_z = player->position[2];
    transform->coordinate.super = 0;
    transform->coordinate.flg = 0;
    transform->coordinate.coord.t[2] = z - player_z;
}

// FUNCTION: COMB.BIN:0x80061AB0
void func_80061AB0(CombMotionPlayer *player, int *position)
{
    CombMotionTransform *transform = (CombMotionTransform *)player->motion;
    int z;
    int player_z;

    transform->coordinate.coord.t[0] = position[0] - player->position[0];
    transform->coordinate.coord.t[1] = position[1] - player->position[1] - player->motion->height;
    z = position[2];
    player_z = player->position[2];
    transform->coordinate.super = 0;
    transform->coordinate.flg = 0;
    transform->coordinate.coord.t[2] = z - player_z;
}

// FUNCTION: COMB.BIN:0x80061AFC
void func_80061AFC(CombMotionPlayer *player, int *position)
{
    CombCameraState *camera = (CombCameraState *)player->motion;

    camera->view.vpx = camera->position[0] - player->position[0];
    camera->view.vpy = camera->position[1] - player->position[1];
    camera->view.vpz = camera->position[2] - player->position[2];
    camera->view.vrx = position[0] - player->position[0];
    camera->view.vry = position[1] - player->position[1] - 150;
    camera->view.vrz = position[2] - player->position[2];
}

// FUNCTION: COMB.BIN:0x80061C20
void func_80061C20(CombPosePlayer *player)
{
    VECTOR position;
    VECTOR rotation;
    CombPoseState *state = (CombPoseState *)player->player.motion;
    int amount = player->pose->amount;
    short *height = &player->player.height;
    int threshold = 70;
    int delta;
    int angle;
    int limit;
    GsCOORDINATE2 *coordinate;

    memset_800449E8((unsigned char *)&rotation, 0, sizeof(rotation));
    rotation.vx = 0;
    rotation.vy = -state->height;
    rotation.vz = 0;
    position = rotation;
    rotation = D_800981C8;
    state->transform.coordinate.coord = D_80058E20;
    coordinate = &state->transform.coordinate;
    if (amount > threshold) {
        delta = amount - 70;
        angle = 100;
        angle *= delta;
        position.vy -= angle;
        angle = 17;
        rotation.vz = delta * angle;
    } else {
        angle = 3072;
        angle = ((threshold - amount) * angle) / threshold;
        limit = 1024;
        if (angle < 1025) limit = angle;
        rotation.vx -= limit;
    }
    RotMatrixZ(rotation.vz, &coordinate->coord);
    RotMatrixX(rotation.vx, &coordinate->coord);
    coordinate->coord.t[0] = position.vx;
    coordinate->coord.t[1] = position.vy;
    coordinate->coord.t[2] = position.vz;
    coordinate->super = 0;
    coordinate->flg = 0;
    state->transform.rotation.vx = rotation.vx;
    state->transform.rotation.vy = rotation.vy;
    state->transform.rotation.vz = rotation.vz;
    *(int *)((unsigned char *)state + 0x24) = 0;
    state->saved_height = *height;
}

// FUNCTION: COMB.BIN:0x800622D8
void func_800622D8(CombSurfaceMotionPlayer *player)
{
    VECTOR projected;
    CombSurfaceState original;
    CombSurfaceState moved;
    VECTOR delta;
    int slow = player->slow;
    int mode = player->surface.mode;
    int alternate = player->alternate_surface.word_18;

    switch (mode) {
    case 1:
    case 2:
        delta.vx = 0;
        delta.vy = D_80098200;
        delta.vz = 0;
        break;
    case 0:
    default:
        delta.vx = 0;
        if (player->vertical_speed > 0xA000)
            delta.vy = D_800981F8;
        else
            delta.vy = D_800981FC;
        delta.vz = 0;
    }
    delta.vx = ((delta.vx << 4) >> 4) / 900;
    delta.vy = ((delta.vy << 4) >> 4) / 900;
    delta.vz = ((delta.vz << 4) >> 4) / 900;
    if (mode == 0) {
        original.kind = 0;
        original.x = 0;
        original.y = 0x800;
        original.z = 0;
        original.surface = -1;
        original.mode = 2;
        original.height = 0;
        original.word_18 = 0;
        original.word_54 = 0;
        original.word_50 = 0;
        moved.kind = 0;
        moved.x = 0;
        moved.y = 0x800;
        moved.z = 0;
        moved.surface = -1;
        moved.mode = 2;
        moved.height = 0;
        moved.word_18 = 0;
        moved.word_54 = 0;
        moved.word_50 = 0;
        if (alternate == 0)
            original = player->surface;
        else
            original = player->alternate_surface;
        moved = original;
        moved.x += delta.vx;
        moved.y += delta.vy;
        moved.z += delta.vz;
        if (original.height - moved.y < 0) {
            func_8006FC8C(&moved, &projected);
            delta.vx = projected.vx - original.x;
            delta.vy = projected.vy - original.y;
            delta.vz = projected.vz - original.z;
        }
    }
    delta.vx = delta.vx * 0x3840 >> 4;
    delta.vy = delta.vy * 0x3840 >> 4;
    delta.vz = delta.vz * 0x3840 >> 4;
    if (slow > 0) {
        delta.vx = delta.vx * 3 / 10;
        delta.vz = delta.vz * 3 / 10;
    }
    player->motion_x += delta.vx;
    player->motion_y += delta.vy;
    player->motion_z += delta.vz;
}

// FUNCTION: COMB.BIN:0x800608D4
void func_800608D4(CombPlayerSurfaceMotionView *player)
{
    CombSurfaceMotionView *motion = player->motion;
    int state = motion->state;
    int heading;

    switch (state) {
    case 0:
        state = 1;
        break;
    case 2:
        state = 6;
        break;
    case 1:
    case 5:
    case 6:
        state = 0;
        break;
    }
    motion->state = state;
    heading = player->heading;
    if (player->reverse != 0) {
        heading += 0x800;
    }
    motion->heading = heading;
    motion->reset = 0;
}

// FUNCTION: COMB.BIN:0x80060940
void func_80060940(CombPlayerCameraView *player, int mode)
{
    CombCameraModeView *state = player->camera;
    int primary = -(player->secondary_view < 1);

    switch (mode) {
    case 5:
        state = player->camera;
        state->camera_mode = 5;
        state->projection = 600;
        state->height = 350;
        state->unknown_086 = 200;
        state->view.vpx = player->x;
        state->view.vpy = (player->y + player->target_y) >> 1;
        state->view.vpz = player->z;
        state->view.vrx = 0;
        state->view.vry = 0;
        state->view.vrz = 0;
        state->view.rz = 0;
        state->view.super = 0;
        if (primary == -1) {
            func_8002EE40(state->projection);
        }
        state->rotation.vx = 0;
        state->rotation.vy = 0;
        state->rotation.vz = 0;
        break;
    case 2:
        state->camera_mode = 2;
        state->projection = 150;
        state->height = 350;
        state->unknown_086 = 200;
        state->view.vpz = 512;
        state->view.super = &state->camera_coordinate;
        state->view.vpx = 0;
        state->view.vpy = 0;
        state->view.vrx = 0;
        state->view.vry = 0;
        state->view.vrz = 0;
        state->view.rz = 0;
        if (primary == -1) {
            func_8002EE40(state->projection);
        }
        state->rotation.vx = 0;
        state->rotation.vy = 0;
        state->rotation.vz = 0;
        break;
    case 1:
        state->camera_mode = 1;
        state->projection = 150;
        state->height = 250;
        state->unknown_086 = 250;
        state->view.vrz = -512;
        state->view.super = &state->camera_coordinate;
        state->view.vpx = 0;
        state->view.vpy = 0;
        state->view.vpz = 0;
        state->view.vrx = 0;
        state->view.vry = 0;
        state->view.rz = 0;
        if (primary == -1) {
            func_8002EE40(state->projection);
        }
        break;
    case 0:
        state->projection = 150;
        state->height = 350;
        state->unknown_086 = 200;
        state->view.vpz = 512;
        state->view.super = &state->camera_coordinate;
        state->camera_mode = 0;
        state->view.vpx = 0;
        state->view.vpy = 0;
        state->view.vrx = 0;
        state->view.vry = 0;
        state->view.vrz = 0;
        state->view.rz = 0;
        if (primary == -1) {
            func_8002EE40(state->projection);
        }
        break;
    case 6:
        state->camera_mode = 6;
        state->projection = 250;
        state->height = 350;
        state->unknown_086 = 200;
        state->view.vpz = 1024;
        state->view.super = &state->camera_coordinate;
        state->view.vpx = 0;
        state->view.vpy = 0;
        state->view.vrx = 0;
        state->view.vry = 0;
        state->view.vrz = 0;
        state->view.rz = 0;
        if (primary == -1) {
            func_8002EE40(state->projection);
        }
        break;
    case 3:
        state->camera_mode = 3;
        state->projection = 150;
        state->height = 350;
        state->unknown_086 = 200;
        state->view.vpz = 512;
        state->view.super = &state->camera_coordinate;
        state->view.vpx = 0;
        state->view.vpy = 0;
        state->view.vrx = 0;
        state->view.vry = 0;
        state->view.vrz = 0;
        state->view.rz = 0;
        if (primary == -1) {
            func_8002EE40(state->projection);
        }
        break;
    case 4:
        state->projection = 150;
        state->height = 350;
        state->unknown_086 = 200;
        state->view.vpz = 512;
        state->view.super = &state->camera_coordinate;
        state->camera_mode = 0;
        state->view.vpx = 0;
        state->view.vpy = 0;
        state->view.vrx = 0;
        state->view.vry = 0;
        state->view.vrz = 0;
        state->view.rz = 0;
        if (primary == -1) {
            func_8002EE40(state->projection);
        }
        state->camera_mode = 4;
        break;
    }
}

// FUNCTION: COMB.BIN:0x80061E48
int func_80061E48(CombCameraPlayerView *player, int update)
{
    CombCameraControl *camera = (CombCameraControl *)player->motion;
    int angle_b = player->angle_b;
    int angle_a = player->angle_a;
    int mode = camera->mode;
    int result = 0;
    int roll;
    int step;
    int value;
    int yaw;
    int speed;
    int lean;
    int state;
    int saved_z;
    int saved_x;
    CombCameraControl *transform;

    if (player->pose->amount >= 0) {
        roll = 0;
        func_80061C20((CombPosePlayer *)player);
        goto apply_view;
    }
    step = camera->transition;
    if (step >= 0) {
        step++;
        if (step < 4) {
            roll = 100;
            if (mode == 0) {
                camera->height = 250 + step * roll / 4;
                camera->view.vpz = step << 7;
                camera->view.vrz = 0;
            } else {
                state = 4 - step;
                camera->height = 250 + state * roll / 4;
                camera->view.vpz = state << 7;
                camera->view.vrz = 0;
            }
            camera->previous_height = camera->height;
            mode = 1;
        } else {
            func_80060940((CombPlayerCameraView *)player, mode);
            step = -1;
        }
        camera->transition = step;
    }
    state = player->state;
    if (state >= 44) state -= 42;
    result = -1;
    switch (mode) {
    case 2:
        transform = (CombCameraControl *)player->motion;
        transform->coordinate.coord = D_80058E20;
        RotMatrixX(1024, &transform->coordinate.coord);
        transform->coordinate.coord.t[1] = -1000;
        transform->coordinate.super = 0;
        transform->coordinate.flg = 0;
        transform->saved_height = player->height;
        camera->word_190 = 0;
        break;
    case 5:
        camera->word_190 = 0;
        break;
    case 1:
        if (state == 7) {
            yaw = player->camera_yaw;
            if (player->direction == 0) value = yaw + 170;
            else value = yaw - 170;
            player->camera_yaw = value;
        } else if (state == 8) {
            player->camera_yaw = player->yaw + player->camera_yaw - ((player->yaw + player->camera_yaw) >> 2);
        } else {
            player->camera_yaw = player->yaw;
        }
        step = player->yaw;
        value = player->camera_yaw;
        saved_x = (unsigned short)player->rotation.vx;
        saved_z = (unsigned short)player->rotation.vz;
        player->rotation.vx = 0;
        player->rotation.vz = 0;
        value = (value << 20) >> 20;
        player->camera_yaw = value;
        player->yaw = value;
        func_800966A8(&player->rotation);
        player->rotation.vx = saved_x;
        player->rotation.vz = saved_z;
        camera->word_190 = func_80060E14(player, camera->word_190, update);
        player->yaw = step;
        break;
    case 0:
        roll = player->velocity.vx >> 6;
        step = roll * roll;
        roll = player->velocity.vz >> 6;
        speed = SquareRoot12(step + roll * roll);
        func_80061440(player, ((speed * 36 / 10) * 17) / 11, update);
        camera->word_190 = 0;
        break;
    case 6:
        roll = player->velocity.vx >> 6;
        step = roll * roll;
        roll = player->velocity.vz >> 6;
        speed = SquareRoot12(step + roll * roll);
        func_80061440(player, ((speed * 36 / 10) * 17) / 11, update);
        speed = camera->coordinate.coord.t[1];
        camera->word_190 = 0;
        camera->coordinate.coord.t[1] = speed + 50;
        break;
    }
camera_ready:
    if (update == 0) {
        if (camera->mode == 1) {
            if (player->direction != 0) lean = player->lean;
            else lean = -player->lean;
            roll = lean * 360;
            if (angle_a != 0) {
                step = (angle_b - angle_a) * (90 - roll / 4096) * 128;
                roll += step;
            }
            camera->view.rz = roll;
            goto apply_view;
        }
        roll = 0;
        if (camera->mode != 5) {
            camera->view.rz = 0;
            goto apply_view;
        }
    }
    roll = camera->view.rz;
apply_view:
    camera->view.super->flg = 0;
    GsSetRefView2L(&camera->view);
    camera->view.rz = roll;
    return result;
}

// FUNCTION: COMB.BIN:0x80062694
void func_80062694(CombSpeedPlayer *player, int braking, int half_speed,
                   int strong, int timer_mode, int scale)
{
    int limit = player->speed_limit;
    int slow = player->slow;
    int percent = player->parameters->percent;
    int active = player->active;
    int mode = player->mode;
    int decrement = 98;
    int x, z, speed;
    int value;
    int new_x, new_z;

    if (timer_mode == -1 && (value = player->timer) > 0) {
        value -= 3;
        value = value < 0 ? 0 : value;
        player->timer = value;
        value = 0x2E035;
        if (value < limit)
            speed = limit;
        else
            speed = 0x2E035;
        limit = speed;
    }
    if (scale != 0)
        limit = (limit * scale) / 4096;
    if (half_speed != 0) {
        limit >>= 1;
    } else if (slow < 0 && braking == 0) {
        limit = (limit * percent) / 100;
    }
    value = player->velocity_x >> 6;
    x = value * value;
    value = player->velocity_z >> 6;
    z = value * value;
    speed = SquareRoot12(x + z);
    if (active != 0) {
        switch (mode) {
        case 3:
        case 4:
            limit = 0;
            value = !strong;
            if (value == 0) value = 4;
            else value = 1;
            decrement = (player->deceleration * value) / 100;
            break;
        case 0:
        default:
            if (speed < 0x3983)
                break;
        case 1:
        case 2:
            limit = 0;
            decrement = player->deceleration / 100;
            break;
        }
    }
    if (limit <= speed || braking != 0) {
        if (speed - limit >= 0x5C1) {
            player->acceleration_x = 0;
            player->acceleration_z = 0;
            if (half_speed != 0)
                limit = speed + decrement * -8;
            else
                limit = speed - decrement;
        } else if (braking != 0) {
            limit = speed - 0x170;
        }
        speed >>= 6;
        limit >>= 6;
        if (speed > 0 && limit > 0) {
            new_x = player->velocity_x * limit / speed;
            new_z = player->velocity_z * limit / speed;
            player->velocity_x = new_x;
            player->velocity_z = new_z;
        }
    }
}

// FUNCTION: COMB.BIN:0x80062978
int func_80062978(CombPlayerHeadingView *player, int turn_step, int divisor)
{
    long motion_x = player->motion_x;
    int difference = motion_x >> 6;
    int speed = difference * difference;
    long motion_z = player->motion_z;
    short heading = player->heading;
    int magnitude;
    int folded;
    int temporary;

    difference = motion_z >> 6;
    speed += difference * difference;
    speed = SquareRoot12(speed);
    motion_z = ratan2(-motion_x, -motion_z);
    temporary = (unsigned short)heading;
    temporary <<= 16;
    temporary >>= 16;
    temporary = motion_z - temporary;
    temporary <<= 20;
    difference = temporary >> 20;
    magnitude = __builtin_abs(difference);
    folded = magnitude;
    if (magnitude > 0x400)
        folded = 0x800 - magnitude;
    speed = speed * (0x400 - folded / divisor) >> 10;
    if (turn_step < folded) {
        difference = difference < 0 ? -turn_step : turn_step;
        if (magnitude >= 0x400)
            motion_z += difference;
        else
            motion_z -= difference;
    } else {
        if (magnitude < 0x400)
            motion_z = (short)heading;
        else {
            motion_z = ((heading - 0x800) << 20) >> 20;
        }
    }
    motion_x = -(speed * rsin(motion_z));
    motion_z = -(speed * rcos(motion_z));
    player->motion_x = motion_x >> 12;
    player->motion_z = motion_z >> 12;
    return -(magnitude < 0x400);
}

// FUNCTION: COMB.BIN:0x80062B2C
unsigned int func_80062B2C(CombPlayerSlopeView *player)
{
    int index;
    CombSurfaceState *surface;
    int heading;
    int *history_a;
    int *history_b;
    int first;
    int second;
    int i;
    int value_a;
    int value_b;
    int *cursor_a;
    int *cursor_b;

    index = player->history_index % 16;
    if (player->alternate_surface.word_18 == 0)
        surface = &player->surface;
    else
        surface = &player->alternate_surface;
    heading = (player->heading << 20) >> 20;
    history_a = player->motion->history_a;
    history_b = player->motion->history_b;
    first = (short)func_800702F4(surface, heading);
    second = (short)func_800702F4(surface, ((heading - 1024) << 20) >> 20);
    if (surface->mode == 0) {
        history_a[index] = first;
        history_b[index] = second;
    } else {
        history_b[index] = 0;
    }
    first = 0;
    second = 0;
    i = 0;
    cursor_b = history_b;
    cursor_a = history_a;
    do {
        value_b = *cursor_b++;
        value_a = *cursor_a++;
        i++;
        first += value_a;
        second += value_b;
    } while (i < 16);
    first /= 16;
    second /= 16;
    second = (second << 20) >> 20;
    first = (first << 20) >> 20;
    return (first << 16) | (second & 0xFFFF);
}

// FUNCTION: COMB.BIN:0x80062C94
void func_80062C94(CombPlayerImpulseView *player, int alternate)
{
    int impulse;

    if (alternate != 0)
        impulse = -D_80098208;
    else
        impulse = -D_80098204;
    player->acceleration_y += (impulse * (player->parameters->scale >> 6)) / 64;
}
