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
extern void func_8005DE24(GsOT *ot, int view, RECT *clip, int x, int y, int mode);
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
