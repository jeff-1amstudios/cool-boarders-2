#include "bp.h"

extern void *func_8001F1A0(unsigned int size);
extern void *func_800782E8(int count, CVECTOR *color);
extern void func_80078DAC(void *effects);
extern void func_800641E8(void *first, void *second, int player, int special);
extern void func_80076F64(int red, int green, int blue, int first, int second);
extern short *D_8009FC94;
extern CVECTOR D_80095904;
extern BpPlayer *D_8009FA4C[];
extern void *D_8009D670[];
extern unsigned char D_8009D6F8[];
extern void *D_8009E928[];
extern void *D_8009E92C[];
extern void *D_8009E948;
extern short D_8009E958;
extern unsigned char D_80097984[];
extern unsigned char D_80097985[];
extern unsigned char D_80097986[];
extern short D_80097A00[];
extern short D_80097A02[];
extern void func_8002F750(PACKET *work_base);
extern GsOT D_8009E900[2];
extern unsigned short D_8009E93C;
extern GsOT *D_8009FC44;

extern unsigned int D_80097A40;

extern int D_8009D66C;

extern PACKET *D_80058ACC;
extern TILE D_8009E95C;
extern DR_MODE D_8009E97C;
extern void func_8009456C(GsOT_TAG *tag, DR_MODE *mode);

extern int func_8001F378(void *allocation);
extern int D_800570E0;
extern void func_80078E34(void *effects);
extern void func_80077D74(void);

extern TILE D_8009E96C;
extern DR_MODE D_8009E988;

extern int D_80058E10;
extern int D_80058E18;
extern DVECTOR D_8009E930;
extern short D_8009E934;
extern short D_8009E936;
extern short D_8009E938;
extern short D_8009E93A;
extern short D_8009E93E;
extern DVECTOR D_8009E940;

extern void func_8005E95C(GsOT *ot, BpPlayer *player, int first, int second);
extern int func_8005D33C(GsOT *ot, int player, int first, int second, int mode);
extern void func_8005D760(GsOT *ot, int player, RECT *clip,
                          unsigned char red, unsigned char green, unsigned char blue);
extern void func_80016748(void);
extern void func_80016AEC(void);
extern unsigned char D_80097980[];
extern unsigned char D_80097981[];
extern unsigned char D_80097982[];

extern void func_8005E81C(GsOT *ot, BpPlayer *player, int first, int second);
extern void func_8005F8F8(BpPlayer *player);
extern void func_8005F964(BpPlayer *player, int state);
extern void func_80060E6C(BpPlayer *player, int different);
extern void func_8005D8C4(GsOT *ot, BpViewState *state, int *position, int mode, void *data);
extern int func_8005D820(int mode, int player_mode);
extern void func_80077190(GsOT *ot, void *scratch, int distance);
extern void func_80077E68(GsOT *ot, int x, int y, int z, int one, void *scratch, int different);
extern void func_8007882C(GsOT *ot, void *effects, void *transform);
extern void func_8005E0E0(GsOT *ot, BpPlayer *player, int index);
extern void func_8005E454(GsOT *ot, BpPlayer *player);
extern unsigned short D_8009D630;
extern SVECTOR D_8009D648;
extern int D_8009757C;
extern int D_8009E994;

extern int D_8009E944;
extern int D_8009E9AC;
extern MATRIX D_80058E20;
extern BpRenderEntry *D_8009FA28;
extern int D_8009FC9C;
extern void func_8005DA14(GsOT *ot, BpViewState *state, int *position,
                          int index, int fifth, int sixth, int seventh,
                          int mode, int ninth);

extern int D_80097578;
extern long D_8009D658;
extern void func_8005E6C0(BpPlayer *player);
extern void func_80069A1C(GsOT *ot, BpPlayer *player, int shift);

extern short D_800958D0;
extern short D_800958D2;
extern int D_80097BA4;
extern void *D_8009FC64;
extern PACKET *func_8007D4B8(GsOT *ot, int index, PACKET *packets, int shift);
extern void func_8007F1BC(void);
extern void func_8007F2A8(void);
extern void func_8007F368(void *data);

extern void func_8007EF5C(GsOT *ot, int kind, int flags, int index,
                          int *position, int shift, void *scratch);

extern void func_8007C37C(int distance, short angle, int enabled, int mode, int value);

extern void func_8006A668(GsOT *ot, void *data, int different);
extern void func_8006AE14(GsOT *ot, int mode, int first, int second, int value);
extern void func_8006AF48(GsOT *ot, BpPlayer *player, int different);
extern void func_80069934(GsOT *ot, int different);
extern int D_8009D65C;
extern int D_8009D660;

extern int D_80097A44[10];

extern int func_8008C86C(int value);

extern int func_80015F68(void);
extern void func_800172B8(void);

extern int func_80015F98(void);
extern void func_800173C0();

extern long D_8009FC98;
extern long D_8009FCA0;

extern VECTOR D_800959CC;

extern void func_8001BF08(void);
extern void func_80012770(int width, int height);
extern int func_80061FB0(FileLoadState *state, int mode);
extern void func_80015C8C(int pitch, unsigned long voices, int sample, int volume);
extern int func_8001EC78(void *data);
extern int func_8001C67C(unsigned int *state, int mode);
extern int func_8001C208(FileLoadState *state, int mode);
extern int func_8001C130(FileLoadState *state, short mode);
extern unsigned int D_800570C8;
extern short D_800570DC;
extern short D_80047CF8[10];
extern CardGameSettings *D_8009FA54;
extern CardAudioSettings *D_8009FC88;
extern int *D_8009FC4C;
extern CardGameRecord *D_8009FA40;
extern int D_8009FC84;
extern void func_8002EE40(long projection);
extern int D_800959FC;
extern int D_80095A00;
extern int D_80095A04;
extern void func_8006E41C(BpSurfaceState *surface, VECTOR *position);
extern int func_8006EA84(BpSurfaceState *surface, int heading);

// FUNCTION: BP.BIN:0x8005B4A4
int func_8005B4A4(void)
{
    int other_player = 0;
    int player = other_player;
    BpPlayer **current = D_8009FA4C;
    int allocation_offset = 0;
    unsigned char *base = D_8009D6F8;
    unsigned char *third = base + 0xD00;
    unsigned char *first = base;
    int buffer_offset = 0;
    int selected = D_8009FC94[0];
    int mode = D_8009FC94[10];
    CVECTOR color = D_80095904;
    int index;
    int offset;
    void *allocation;
    CVECTOR *effect_color;
    int special;
    int is_second;
    int chosen;
    int fifth;
    int work_size;
    int color_offset;
    int settings_offset;
    unsigned char *middle;
    BpPlayer *entry;

    do {
        *current = func_8001F1A0(0x491C);
        allocation = func_8001F1A0(0x1B8);
        *(void **)((unsigned char *)D_8009D670 + allocation_offset) = allocation;
        if (*current == 0) goto failure;
        if (allocation == 0) return 0;
        effect_color = 0;
        if (D_8009FC94[10] == 6) effect_color = &color;
        (*current)->effects = func_800782E8(8, effect_color);
        func_80078DAC((*current)->effects);
        (*current)->buffer_1f4 = first;
        middle = base + 0x300;
        (*current)->buffer_1f8 = (void *)(buffer_offset + (unsigned int)middle);
        (*current)->buffer_1fc = third;
        is_second = player != 0;
        if (player == 0) chosen = selected == 6;
        else { special = D_8009FC94[5]; chosen = special == 6; }
        func_800641E8((unsigned char *)*current + 0x1E4,
                     (unsigned char *)*current + 0xC58, is_second, -chosen);
        current++;
        allocation_offset += 4;
        third += 0x1288;
        first += 0x1288;
        buffer_offset += 0x1288;
        player++;
    } while (player < 1);

    index = 0;
    work_size = 0x20C0;
    offset = 0;
    do {
        *(void **)((unsigned char *)D_8009E928 + offset) = func_8001F1A0(work_size);
        if (*(void **)((unsigned char *)D_8009E928 + offset) == 0) return 0;
        *(void **)((unsigned char *)D_8009E92C + offset) = func_8001F1A0(work_size);
        index++;
        if (*(void **)((unsigned char *)D_8009E92C + offset) == 0) goto failure;
        offset += 8;
    } while (index < 1);
    D_8009E948 = func_8001F1A0(0x50);
    if (D_8009E948 == 0) goto failure;
    other_player = mode;
    color_offset = other_player * 8;
    other_player *= 4;
    D_8009E958 = mode;
    special = *(short *)((int)D_80097A00 + other_player);
    func_80076F64(D_80097984[color_offset], D_80097985[color_offset],
                 D_80097986[color_offset],
                 special,
                 *(short *)((int)D_80097A02 + other_player));
    return -1;
failure:
    return 0;
}

// FUNCTION: BP.BIN:0x8005B7E4
void func_8005B7E4(void)
{
    GsOT *ot;
    int buffer = GsGetActiveBuff();
    GsOT **current;
    int index;
    GsOT_TAG *tags;

    func_8002F750(((PACKET **)&D_8009E93C)[buffer]);
    index = 0;
    current = &D_8009FC44;
    ot = &D_8009E900[buffer];
    do {
        *current = ot;
        current++;
        tags = ot->org;
        ot += 2;
        index++;
        ClearOTagR((u_long *)tags, 0x800);
    } while (index < 1);
}

// FUNCTION: BP.BIN:0x8005B87C
unsigned int func_8005B87C(unsigned int value, int mode)
{
    switch (mode) {
    case 0:
        D_80097A40 = value;
        break;
    case 1:
        if (D_80097A40 < ~value)
            D_80097A40 += value;
        break;
    }
    return D_80097A40;
}

// FUNCTION: BP.BIN:0x8005B91C
void func_8005B91C(int value)
{
    if (value >= 0 && value != 1) {
        D_8009D66C = value;
    }
}

// FUNCTION: BP.BIN:0x8005B93C
void func_8005B93C(GsOT *ot, unsigned char intensity)
{
    if (intensity != 0) {
        D_8009E95C.r0 = intensity;
        D_8009E95C.g0 = intensity;
        D_8009E95C.b0 = intensity;
        *(TILE *)D_80058ACC = D_8009E95C;
        ((GsOT_TAG *)D_80058ACC)->p = ot->org[3].p;
        ot->org[3].p = (unsigned long)D_80058ACC;
        D_80058ACC += sizeof(TILE);
        func_8009456C(&ot->org[3], &D_8009E97C);
    }
}
// FUNCTION: BP.BIN:0x8005AEA8
void func_8005AEA8(FileLoadState *state)
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
// FUNCTION: BP.BIN:0x8005B720
void func_8005B720(void)
{
    int count = 0;
    BpPlayer **player = D_8009FA4C;
    void **allocation = D_8009D670;
    int offset;

    do {
        count++;
        func_80078E34((*player)->effects);
        func_8001F378(*allocation++);
        func_8001F378(*player++);
    } while (count < 1);

    offset = 0;
    do {
        func_8001F378(*(void **)((unsigned char *)D_8009E928 + offset));
        func_8001F378(*(void **)((unsigned char *)D_8009E92C + offset));
        offset += 8;
    } while (offset < 1);
    func_8001F378(D_8009E948);
    func_80077D74();
}

// FUNCTION: BP.BIN:0x8005BA14
void func_8005BA14(GsOT *ot, unsigned char intensity)
{
    if (intensity != 0) {
        D_8009E96C.r0 = intensity;
        D_8009E96C.g0 = intensity;
        D_8009E96C.b0 = intensity;
        *(TILE *)D_80058ACC = D_8009E96C;
        ((GsOT_TAG *)D_80058ACC)->p = ot->org[3].p;
        ot->org[3].p = (unsigned long)D_80058ACC;
        D_80058ACC += sizeof(TILE);
        func_8009456C(&ot->org[3], &D_8009E988);
    }
}

// FUNCTION: BP.BIN:0x8005BAEC
void func_8005BAEC(int mode)
{
    switch (mode) {
    case 1:
        D_8009E930.vx = D_80058E10 / 4;
        D_8009E930.vy = D_80058E18 / 2;
        D_8009E934 = D_80058E10 * 3 / 4;
        D_8009E936 = D_80058E18 / 2;
        D_8009E934 = 0;
        D_8009E936 = 0;
        D_8009E938 = D_80058E10 / 2;
        D_8009E93A = D_80058E18;
        D_8009E93C = D_80058E10 / 2;
        D_8009E93E = 0;
        D_8009E940.vx = D_80058E10 / 2;
        D_8009E940.vy = D_80058E18;
        break;
    case 2:
        D_8009E930.vx = D_80058E10 / 2;
        D_8009E930.vy = D_80058E18 / 4;
        D_8009E934 = D_80058E10 / 2;
        D_8009E936 = D_80058E18 * 3 / 4;
        D_8009E934 = 0;
        D_8009E936 = 0;
        D_8009E938 = D_80058E10;
        D_8009E93A = D_80058E18 / 2;
        D_8009E93C = 0;
        D_8009E93E = D_80058E18 / 2;
        D_8009E940.vx = D_80058E10;
        D_8009E940.vy = D_80058E18 / 2;
        break;
    default:
        D_8009E934 = 0;
        D_8009E936 = 0;
        D_8009E930.vx = D_80058E10 / 2;
        D_8009E930.vy = D_80058E18 / 2;
        D_8009E938 = D_80058E10;
        D_8009E93A = D_80058E18;
        break;
    }
}

// FUNCTION: BP.BIN:0x8005D1AC
void func_8005D1AC(int first, int second, int mode)
{
    int player = 0;
    GsOT **base = &D_8009FC44;
    GsOT **current;
    int results[2];
    int offset;
    int color_offset;
    RECT *clip;
    GsOT *ot;
    GsOT *initial_ot;

    func_8005E95C(*base, D_8009FA4C[0], first, second);
    current = base;
    initial_ot = *current;
    results[0] = func_8005D33C(initial_ot, 0, first, second, mode);
    offset = 0;
    VSync(D_8009D66C);
    ResetGraph(1);
    GsSwapDispBuff();
    do {
        GsSetOffset(*(short *)((unsigned char *)&D_8009E930.vx + offset),
                    *(short *)((unsigned char *)&D_8009E930.vy + offset));
        clip = (RECT *)(&D_8009E934 + player * 4);
        GsSetClip(clip);
        if (*(int *)((unsigned char *)results + offset) == -1) {
            color_offset = D_8009E958 * 8;
            GsSortClear(D_80097980[color_offset], D_80097981[color_offset],
                        D_80097982[color_offset], D_8009FC44);
        } else {
            func_8005D760(*current, player, clip, 0, 0, 0);
        }
        ot = *current;
        current++;
        offset += 4;
        player++;
        DrawOTag((u_long *)(ot->org + 0x7FF));
    } while (player < 1);
    func_80016748();
    func_80016AEC();
}

// FUNCTION: BP.BIN:0x8005D33C
int func_8005D33C(GsOT *ot, int player, int first, int second, int mode)
{
    BpPlayer *selected = D_8009FA4C[player];
    BpMotion *motion;
    BpViewState *view = *(BpViewState **)((unsigned char *)selected + 0x6D8);
    int state = view->state;
    int busy = *(int *)((unsigned char *)selected + 0x738);
    int different = -(first != second);
    int result;
    int distance;
    int index;
    BpPlayer **current;
    BpPlayer *entry;

    motion = (BpMotion *)((unsigned char *)selected + 0x28);
    func_8005E81C(ot, selected, first, second);
    if (first == second && first != 3) {
        index = 0;
        if (*(short *)((unsigned char *)selected + 0x48FE) == 0) {
            if ((motion->buttons ^ motion->previous_buttons) & motion->buttons & D_8009D630) {
                if (first == 0 && (unsigned int)(state - 3) >= 2)
                    index = -(busy == 0);
            } else if (first == 5) {
                if (state == 1) index = -1;
            } else if (*(short *)((unsigned char *)selected + 0x4900) != 0) {
                if (view->previous_state != state && busy == 0) index = -1;
            } else if (busy != 0) {
                if (*(int *)((unsigned char *)selected + 0x73C) == 0) {
                    view->previous_state = state;
                    if (state == 1) index = -1;
                }
            } else {
                if (*(int *)((unsigned char *)selected + 0x73C) != 0 && view->previous_state != state)
                    index = -1;
            }
        } else if (*(short *)((unsigned char *)selected + 0x4900) == 0) {
            view->previous_state = state;
            if (state == 1) index = -1;
        }
        if (index == -1 && *(short *)(*(unsigned char **)((unsigned char *)selected + 0x4918) + 0x64) < 0) {
            func_8005F8F8(selected);
            state = view->state;
            func_8005F964(selected, state);
        }
    }
    func_80060E6C(selected, different);
    func_8005D8C4(ot, view, &motion->x, 3, view->data);
    GsScaleScreen(&D_8009D648);
    result = func_8005D820(D_8009FC94[10], motion->mode);
    if (result == -1) {
        distance = 150;
        if (second != 3) distance = -1;
        func_80077190(ot, (void *)(D_8009757C * 4 + 0x1F800000), distance);
    }
    if (D_8009E994 != 0) {
        if (state != 3 && state != 5) {
            func_80077E68(ot, motion->camera_x - motion->previous_x,
                         motion->camera_y - motion->previous_y,
                         motion->camera_z - motion->previous_z,
                         1, (void *)(D_8009757C * 4 + 0x1F800000), first != second);
        } else {
            func_80077E68(ot, 0, 0, 0, 1, (void *)(D_8009757C * 4 + 0x1F800000), 0);
        }
    }
    index = 0;
    current = D_8009FA4C;
    do {
        entry = *current;
        if (*(int *)((unsigned char *)entry + 0xCBC) != 0) {
            if (index == player) {
                if (state == 1 && view->timer < 0 && busy == 0 &&
                    *(short *)((unsigned char *)selected + 0x48FE) != 3) goto next;
                *(int *)((unsigned char *)entry + 0x1A0) = 0;
                *(int *)((unsigned char *)entry + 0x1A4) = 0;
                *(int *)((unsigned char *)entry + 0x1A8) = 0;
            } else {
                if (mode == 0) goto next;
                *(int *)((unsigned char *)entry + 0x1A0) = *(int *)((unsigned char *)entry + 0x2C) - motion->x;
                *(int *)((unsigned char *)entry + 0x1A4) = *(int *)((unsigned char *)entry + 0x30) - motion->y;
                *(int *)((unsigned char *)entry + 0x1A8) = *(int *)((unsigned char *)entry + 0x34) - motion->z;
            }
            func_8007882C(ot, entry->effects, (unsigned char *)entry + 0x188);
            func_8005E0E0(ot, entry, index);
            *(int *)((unsigned char *)entry + 0x1A0) = 0;
            *(int *)((unsigned char *)entry + 0x1A4) = 0;
            *(int *)((unsigned char *)entry + 0x1A8) = 0;
        }
next:
        index++;
        current++;
    } while (index < 1);
    func_8005E454(ot, selected);
    return result;
}

// FUNCTION: BP.BIN:0x8005D760
void func_8005D760(GsOT *ot, int player, RECT *clip,
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

// FUNCTION: BP.BIN:0x8005D820
int func_8005D820(int mode, int player_mode)
{
    int result = -1;

    switch (mode) {
    case 0:
        if ((unsigned int)(player_mode - 0x71) < 0x11) result = 0;
        break;
    case 7:
        if ((unsigned int)(player_mode - 0x30) < 5) result = 0;
        break;
    case 8:
        if ((unsigned int)(player_mode - 100) < 6) result = 0;
        break;
    case 9:
        if ((unsigned int)(player_mode - 4) < 0x30 ||
            (unsigned int)(player_mode - 0x39) < 0x10 ||
            (unsigned int)(player_mode - 0x50) < 0x73) result = 0;
        break;
    }
    return result;
}

// FUNCTION: BP.BIN:0x8005D8C4
void func_8005D8C4(GsOT *ot, BpViewState *state, int *position, int mode, void *data)
{
    int related;
    int index;
    int empty = -1;

    D_8009E944 = 0;
    for (index = 19; index >= 0; index--) {
        ((int *)D_8009E948)[index] = empty;
    }
    index = 0;
    GsSetLightMatrix(&D_80058E20);
    D_8009E9AC = -1;
    do {
        related = D_8009FA28[(int)data].related[index];
        if (related >= 0) {
            func_8005DA14(ot, state, position, related, -1, 0, 1, 0, 0);
        }
        index++;
    } while (index < 3);
    D_8009E9AC = (int)data;
    func_8005DA14(ot, state, position, (int)data, -1, 0, D_8009FC9C, mode, 2);
}

// FUNCTION: BP.BIN:0x8005E0E0
void func_8005E0E0(GsOT *ot, BpPlayer *player, int index)
{
    MATRIX local_screen;
    MATRIX local_world;
    MATRIX saved;
    VECTOR scale;
    VECTOR temporary;
    int count = ((BpRenderView *)player)->object_count;
    GsDOBJ3 *objects = ((BpRenderView *)player)->objects;
    int forward = ((BpRenderView *)player)->forward_order;
    int shift;
    int i;
    int offset;
    GsDOBJ3 *object;
    SVECTOR rotation;
    int value;
    int invalid;

    saved = ((BpRenderView *)player)->coordinate.coord;
    shift = D_80097578 + 3;
    memset_800449E8((unsigned char *)&temporary, 0, sizeof(temporary));
    temporary.vx = (&D_8009D658)[index];
    temporary.vy = (&D_8009D658)[index];
    temporary.vz = (&D_8009D658)[index];
    i = 0;
    scale = temporary;
    ScaleMatrix(&((BpRenderView *)player)->coordinate.coord, &scale);
    func_8005E6C0(player);
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
    GsGetLs(&((BpRenderView *)player)->coordinate, &local_screen);
    GsSetLsMatrix(&local_screen);
    ot->org += 4;
    func_80069A1C(ot, player, shift);
    ot->org -= 4;
    ((BpRenderView *)player)->coordinate.coord = saved;
}

// FUNCTION: BP.BIN:0x8005E31C
void func_8005E31C(GsOT *ot, int shift, int mode, int value)
{
    int offset = 0;

    do {
        if (mode == *(short *)((unsigned char *)&D_800958D0 + offset) &&
            value == *(short *)((unsigned char *)&D_800958D2 + offset)) {
            switch (mode) {
            case 2:
                D_80058ACC = func_8007D4B8(ot, (short)D_80097BA4, D_80058ACC, shift);
                break;
            case 7:
                func_8007F1BC();
                break;
            case 9:
                func_8007F2A8();
                break;
            }
        }
        offset += 4;
    } while (offset < 16);

    if (mode == 9 && ((unsigned int)(value - 0x8E) < 3 ||
                      (unsigned int)(value - 0xB4) < 5)) {
        func_8007F368(D_8009FC64);
    }
}

// FUNCTION: BP.BIN:0x8005E454
void func_8005E454(GsOT *ot, BpPlayer *player)
{
    int offset = 0;
    int kind = -1;
    int flags = 0;
    int shift = D_80097578 + 3;
    BpMotion *motion = (BpMotion *)((unsigned char *)player + 0x28);
    int mode = motion->mode;
    int *position = &motion->x;

    switch (D_8009FC94[10]) {
    case 3:
        if ((unsigned int)(mode - 99) < 12) {
            kind = 0;
        } else {
            offset = 8;
            if (mode < 148) break;
            kind = 1;
        }
        flags = -1;
        offset = 8;
        break;
    case 4:
        if (mode == 83) {
            kind = 0;
            flags = -1;
            offset = 48;
        }
        break;
    }
    if ((unsigned int)(*(int *)(D_8009FC94 + 12) - 1) < 2 && kind >= 0) {
        ot->org += offset;
        func_8007EF5C(ot, kind, (short)flags >> 15, (short)D_80097BA4,
                      position, shift, (void *)(D_8009757C * 4 + 0x1F800000));
        ot->org -= offset;
    }
}

// FUNCTION: BP.BIN:0x8005E594
int func_8005E594(VECTOR *origin, MATRIX *matrix, VECTOR *normal)
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

// FUNCTION: BP.BIN:0x8005E6C0
void func_8005E6C0(BpPlayer *player)
{
    int scratch[2];
    GsDOBJ3 *object = ((BpRenderView *)player)->objects;
    int count = ((BpRenderView *)player)->object_count;
    int i = 0;
    GsCOORDINATE2 *coordinate;

    ((BpRenderView *)player)->coordinate.flg = 0;
    *(int *)((unsigned char *)player + 0xC58) = 0;
    if (count > 0) {
        do {
            coordinate = object->coord2;
            object++;
            i++;
            coordinate->flg = 0;
        } while (i < count);
    }
}

// FUNCTION: BP.BIN:0x8005E700
void func_8005E700(BpPlayer *player, int distance)
{
    int enabled;
    int mode;
    int value;
    int angle;

    mode = *(short *)((unsigned char *)player + 0x72E);
    value = *(int *)((unsigned char *)player + 0x724);
    enabled = -(*(short *)((unsigned char *)player + 0x96) == 0);
    angle = ratan2(-*(int *)((unsigned char *)player + 0x48E0),
                   -*(int *)((unsigned char *)player + 0x48E8))
            - *(short *)((unsigned char *)player + 0x1DA);
    angle = (short)(angle * 0x100000 >> 20);
    switch (D_8009FC94[10]) {
    case 3:
    case 5:
    case 9:
        if (*(int *)((unsigned char *)player + 0x148) == -1 && enabled == -1) {
            mode = 4;
        }
        break;
    }
    if ((unsigned int)(**(unsigned short **)((unsigned char *)player + 0x6D8) - 3) < 2) {
        enabled = 0;
    }
    func_8007C37C(distance, (short)angle, enabled, mode, value);
}

// FUNCTION: BP.BIN:0x8005E95C
void func_8005E95C(GsOT *ot, BpPlayer *player, int first, int second)
{
    int different = -(first != second);

    func_8006A668(ot, (unsigned char *)player + 0x7C8, different);
    if (second != 3) {
        func_8006AE14(ot, ((BpMotion *)((unsigned char *)player + 0x28))->mode,
                     *(int *)((unsigned char *)player + 0x748),
                     *(int *)((unsigned char *)player + 0x744), D_8009D65C);
        func_8006AF48(ot, player, different);
    } else if (D_8009D660 < 8) {
        if (D_8009D660 > 5) {
            func_80069934(ot, different);
        }
    }
}

// FUNCTION: BP.BIN:0x8005F0F0
int func_8005F0F0(int index, int value, int elapsed, int penalty)
{
    int result;

    value += 2500;
    value += (D_80097A44[index] - elapsed) / 20;
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

// FUNCTION: BP.BIN:0x8005F570
void func_8005F570(BpSetupState *state, BpSetupSource *source, int *progress)
{
    state->values = source->values;
    state->word_0c = source->word_0c;
    state->value = source->value;
    state->link = source->link;
    state->link->value = state->value;
    state->link->progress = progress;
}

// FUNCTION: BP.BIN:0x8005F6C0
int func_8005F6C0(BpPlayer *player)
{
    int result;

    result = func_8008C86C(*(short *)((unsigned char *)player + 0x4914));
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

// FUNCTION: BP.BIN:0x8005F714
void func_8005F714(void)
{
    func_80015F68();
    func_800172B8();
    SpuSetKey(0, 0x780FF);
}

// FUNCTION: BP.BIN:0x8005F74C
void func_8005F74C(void)
{
    int selected;

    func_80015F98();
    selected = D_8009FC94[11];
    if (selected >= 0) {
        func_800173C0(selected, 2);
    }
}

// FUNCTION: BP.BIN:0x8005F7D8
void func_8005F7D8(BpMotionState *state, short mode, int value, int x, int y, int z)
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
    ReadGeomOffset(&D_8009FC98, &D_8009FCA0);
}

// FUNCTION: BP.BIN:0x8005FBFC
void func_8005FBFC(BpMotionState *state, int *position)
{
    unsigned int selected;

    selected = state->selected + 1;
    selected &= -(unsigned int)((int)selected < 4);
    state->selected = selected;
    state->samples[selected].x = position[0];
    state->samples[selected].y = position[1];
    state->samples[selected].z = position[2];
}

// FUNCTION: BP.BIN:0x8005FC48
int func_8005FC48(BpMotionState *state, int sample, int target)
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

// FUNCTION: BP.BIN:0x80060464
void func_80060464(BpMotionPlayer *player, int progress, int unchanged)
{
    BpMotionState *state = player->motion;
    short height = player->height;
    unsigned int pitch = (unsigned short)state->mode;
    int yaw = state->value;
    int elevation = state->negative_height;
    short airborne = player->airborne;
    int *position = player->position;
    BpMotionSample *current;
    BpMotionSample *previous;
    int selected;
    int other;
    int angle;
    int length;
    int height_clamp;
    int delta;
    int difference[3];
    BpMotionTransform *transform;

    if (!unchanged) {
        selected = state->selected;
        other = selected - 3;
        if (other < 0) {
            other = selected + 1;
        }
        current = &state->samples[selected];
        previous = &state->samples[other];
        delta = current->x - previous->x;
        length = delta * delta;
        delta = current->z - previous->z;
        length = SquareRoot0(length + delta * delta);
        if (length >= 11) {
            length = ratan2(current->y - previous->y, length);
        } else {
            length = 0;
        }
        angle = (length * 3600) >> 12;
        if (angle > 0) {
            angle = angle > 800 ? 800 : angle;
        } else {
            angle = angle < -800 ? -800 : angle;
        }
        pitch = func_8005FC48(state, angle, (short)pitch);
        other = state->selected + 1;
        current = state->samples;
        if (other < 4) current += other;
        difference[0] = current->x - position[0];
        difference[1] = current->y - position[1];
        difference[2] = current->z - position[2];
        if (player->progress < progress) {
            angle = ratan2(difference[0], difference[2]);
        } else if (player->reverse == 0) {
            angle = player->heading;
        } else {
            angle = player->heading - 2048;
        }
        delta = angle - yaw;
        delta = (delta << 20) >> 20;
        if ((unsigned int)(delta + 91) > 182) {
            if (delta > 0) yaw += 91;
            else yaw -= 91;
        } else {
            yaw = angle;
        }
        yaw = (yaw << 20) >> 20;
        if (player->vertical_speed < 0 && airborne != 0) {
            elevation += 10;
            delta = -state->unknown_086;
            height_clamp = delta < elevation;
        } else {
            elevation -= 5;
            delta = -state->height;
            height_clamp = elevation < delta;
        }
        if (height_clamp) elevation = delta;
    }
    transform = (BpMotionTransform *)state;
    transform->coordinate.coord = D_80058E20;
    angle = ((short)pitch << 12) / 3600;
    RotMatrixX(angle, &transform->coordinate.coord);
    RotMatrixY(yaw, &transform->coordinate.coord);
    transform->coordinate.coord.t[1] = elevation;
    transform->coordinate.super = 0;
    transform->coordinate.flg = 0;
    transform->rotation.vx = angle;
    transform->rotation.vy = yaw;
    transform->rotation.vz = 0;
    state->mode = pitch;
    state->value = yaw;
    state->negative_height = elevation;
    ((BpMotionExtended *)state)->saved_height = height;
}

// FUNCTION: BP.BIN:0x80060A88
void func_80060A88(BpMotionPlayer *player, int *position)
{
    BpMotionTransform *transform = (BpMotionTransform *)player->motion;
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

// FUNCTION: BP.BIN:0x80060AD4
void func_80060AD4(BpMotionPlayer *player, int *position)
{
    BpMotionTransform *transform = (BpMotionTransform *)player->motion;
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

// FUNCTION: BP.BIN:0x80060B20
void func_80060B20(BpMotionPlayer *player, int *position)
{
    BpCameraState *camera = (BpCameraState *)player->motion;

    camera->view.vpx = camera->position[0] - player->position[0];
    camera->view.vpy = camera->position[1] - player->position[1];
    camera->view.vpz = camera->position[2] - player->position[2];
    camera->view.vrx = position[0] - player->position[0];
    camera->view.vry = position[1] - player->position[1] - 150;
    camera->view.vrz = position[2] - player->position[2];
}

// FUNCTION: BP.BIN:0x80060C44
void func_80060C44(BpPosePlayer *player)
{
    VECTOR position;
    VECTOR rotation;
    BpPoseState *state = (BpPoseState *)player->player.motion;
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
    rotation = D_800959CC;
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

// FUNCTION: BP.BIN:0x8005AB6C
int func_8005AB6C(FileLoadState *state)
{
    short saved_record;
    int result;
    BpSelectionView *selection;
    FileLoadState *assets;

    func_8001BF08();
    D_8009FC94 = (short *)&D_800570C8;
    D_8009FA54 = (CardGameSettings *)((char *)&D_800570C8 + 0x2c);
    D_8009FC88 = (CardAudioSettings *)((char *)&D_800570C8 + 0x46);
    D_8009FC4C = (int *)((char *)&D_800570C8 + 0x50);
    D_8009FA40 = (CardGameRecord *)((char *)&D_800570C8 + 0x59c) + D_800570DC;
    SpuSetKey(0, 0x800000);
    assets = 0;
    if (((BpSelectionView *)D_8009FC94)->mode == 0) {
        state->file_5c = 0;
    }
    func_80012770(320, 240);
    D_8009FC84 = state->unknown_04;
    saved_record = ((BpSelectionView *)D_8009FC94)->record;
    switch (((BpSelectionView *)D_8009FC94)->mode) {
    case 0:
    case 5:
        ((BpSelectionView *)D_8009FC94)->record = 14;
        break;
    case 1:
        ((BpSelectionView *)D_8009FC94)->record = D_80047CF8[((BpSelectionView *)D_8009FC94)->course];
        break;
    case 2:
    case 3:
    case 4:
        selection = (BpSelectionView *)D_8009FC94;
        if (saved_record != 10) {
            selection->record = D_80047CF8[saved_record];
        } else {
            selection->record = 0;
        }
        break;
    case 7:
        ((BpSelectionView *)D_8009FC94)->record = 15;
        break;
    case 8:
        ((BpSelectionView *)D_8009FC94)->record = 20;
        break;
    default:
        ((BpSelectionView *)D_8009FC94)->record = 0;
        break;
    }
    result = func_80061FB0(state, 0);
    ((BpSelectionView *)D_8009FC94)->record = saved_record;
    if (result < 5) {
        if (result > 1) goto process;
    } else if (result == 5) {
        return 3;
    }
    goto fallback;
process:
    switch (((BpSelectionView *)D_8009FC94)->mode) {
    case 0:
        ((BpSelectionView *)D_8009FC94)->mode = 1;
        func_80015C8C(0xdc9, 0x800000, 12, 192);
        func_8001EC78(assets->file_13);
        DrawSync(0);
        func_8001EC78(assets->file_14);
        DrawSync(0);
        func_8005AEA8(state);
        return func_8001C67C((unsigned int *)state, 2);
    case 1:
        func_80015C8C(0xdc9, 0x800000, 12, 192);
        ((BpSelectionView *)D_8009FC94)->mode = 0;
        ++*D_8009FC4C;
        if (result == 2) {
            func_8005AEA8(state);
            result = func_8001C67C((unsigned int *)state, 1);
            state->file_5c = 0;
            return result;
        }
        func_8001C208(state, result == 4);
        return 6;
    case 2: goto load_next;
    case 3: goto load_next;
    case 4: goto load_next;
    case 5: goto load_next;
    case 6: goto load_next;
    case 7: goto load_next;
    case 8: goto load_next;
    }
load_next:
    func_8001C208(state, 0);
    return 3;
fallback:
    func_8001BF08();
    func_8001C130(state, 1);
    return 3;
}

// FUNCTION: BP.BIN:0x8005F8F8
void func_8005F8F8(BpPlayer *player)
{
    BpViewState *view = *(BpViewState **)((unsigned char *)player + 0x6D8);
    int state = view->state;
    int angle;

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
    view->state = state;
    angle = *(short *)((unsigned char *)player + 0x1DA);
    if (*(short *)((unsigned char *)player + 0xC4C) != 0) {
        angle += 0x800;
    }
    *(int *)((unsigned char *)view + 0x118) = angle;
    view->timer = 0;
}

// FUNCTION: BP.BIN:0x8005F964
void func_8005F964(BpPlayer *player, int state)
{
    BpCameraState *camera = *(BpCameraState **)((unsigned char *)player + 0x6D8);
    int active = -(*(signed char *)((unsigned char *)player + 0x4916) < 1);

    switch (state) {
    case 5:
        camera = *(BpCameraState **)((unsigned char *)player + 0x6D8);
        (*(short *)camera) = 5;
        (*(unsigned short *)((unsigned char *)camera + 0xA)) = 600;
        (*(short *)((unsigned char *)camera + 0x84)) = 350;
        (*(short *)((unsigned char *)camera + 0x86)) = 200;
        camera->view.vpx = *(int *)((unsigned char *)player + 4);
        camera->view.vpy = (*(int *)((unsigned char *)player + 8) +
                            *(int *)((unsigned char *)player + 0x30)) >> 1;
        camera->view.vpz = *(int *)((unsigned char *)player + 12);
        camera->view.vrx = 0;
        camera->view.vry = 0;
        camera->view.vrz = 0;
        camera->view.rz = 0;
        camera->view.super = 0;
        if (active == -1) func_8002EE40((*(unsigned short *)((unsigned char *)camera + 0xA)));
        ((BpMotionTransform *)camera)->rotation.vx = 0;
        ((BpMotionTransform *)camera)->rotation.vy = 0;
        ((BpMotionTransform *)camera)->rotation.vz = 0;
        break;
    case 2:
        (*(short *)camera) = 2;
        (*(unsigned short *)((unsigned char *)camera + 0xA)) = 150;
        (*(short *)((unsigned char *)camera + 0x84)) = 350;
        (*(short *)((unsigned char *)camera + 0x86)) = 200;
        camera->view.vpz = 512;
        camera->view.super = &((BpMotionTransform *)camera)->coordinate;
        camera->view.vpx = 0;
        camera->view.vpy = 0;
        camera->view.vrx = 0;
        camera->view.vry = 0;
        camera->view.vrz = 0;
        camera->view.rz = 0;
        if (active == -1) func_8002EE40((*(unsigned short *)((unsigned char *)camera + 0xA)));
        ((BpMotionTransform *)camera)->rotation.vx = 0;
        ((BpMotionTransform *)camera)->rotation.vy = 0;
        ((BpMotionTransform *)camera)->rotation.vz = 0;
        break;
    case 1:
        (*(short *)camera) = 1;
        (*(unsigned short *)((unsigned char *)camera + 0xA)) = 150;
        (*(short *)((unsigned char *)camera + 0x84)) = 250;
        (*(short *)((unsigned char *)camera + 0x86)) = 250;
        camera->view.vrz = -512;
        camera->view.super = &((BpMotionTransform *)camera)->coordinate;
        camera->view.vpx = 0;
        camera->view.vpy = 0;
        camera->view.vpz = 0;
        camera->view.vrx = 0;
        camera->view.vry = 0;
        camera->view.rz = 0;
        if (active == -1) func_8002EE40((*(unsigned short *)((unsigned char *)camera + 0xA)));
        break;
    case 0:
        (*(unsigned short *)((unsigned char *)camera + 0xA)) = 150;
        (*(short *)((unsigned char *)camera + 0x84)) = 350;
        (*(short *)((unsigned char *)camera + 0x86)) = 200;
        camera->view.vpz = 512;
        camera->view.super = &((BpMotionTransform *)camera)->coordinate;
        (*(short *)camera) = 0;
        camera->view.vpx = 0;
        camera->view.vpy = 0;
        camera->view.vrx = 0;
        camera->view.vry = 0;
        camera->view.vrz = 0;
        camera->view.rz = 0;
        if (active == -1) func_8002EE40((*(unsigned short *)((unsigned char *)camera + 0xA)));
        break;
    case 6:
        (*(short *)camera) = 6;
        (*(unsigned short *)((unsigned char *)camera + 0xA)) = 250;
        (*(short *)((unsigned char *)camera + 0x84)) = 350;
        (*(short *)((unsigned char *)camera + 0x86)) = 200;
        camera->view.vpz = 1024;
        camera->view.super = &((BpMotionTransform *)camera)->coordinate;
        camera->view.vpx = 0;
        camera->view.vpy = 0;
        camera->view.vrx = 0;
        camera->view.vry = 0;
        camera->view.vrz = 0;
        camera->view.rz = 0;
        if (active == -1) func_8002EE40((*(unsigned short *)((unsigned char *)camera + 0xA)));
        break;
    case 3:
        (*(short *)camera) = 3;
        (*(unsigned short *)((unsigned char *)camera + 0xA)) = 150;
        (*(short *)((unsigned char *)camera + 0x84)) = 350;
        (*(short *)((unsigned char *)camera + 0x86)) = 200;
        camera->view.vpz = 512;
        camera->view.super = &((BpMotionTransform *)camera)->coordinate;
        camera->view.vpx = 0;
        camera->view.vpy = 0;
        camera->view.vrx = 0;
        camera->view.vry = 0;
        camera->view.vrz = 0;
        camera->view.rz = 0;
        if (active == -1) func_8002EE40((*(unsigned short *)((unsigned char *)camera + 0xA)));
        break;
    case 4:
        (*(unsigned short *)((unsigned char *)camera + 0xA)) = 150;
        (*(short *)((unsigned char *)camera + 0x84)) = 350;
        (*(short *)((unsigned char *)camera + 0x86)) = 200;
        camera->view.vpz = 512;
        camera->view.super = &((BpMotionTransform *)camera)->coordinate;
        (*(short *)camera) = 0;
        camera->view.vpx = 0;
        camera->view.vpy = 0;
        camera->view.vrx = 0;
        camera->view.vry = 0;
        camera->view.vrz = 0;
        camera->view.rz = 0;
        if (active == -1) func_8002EE40((*(unsigned short *)((unsigned char *)camera + 0xA)));
        (*(short *)camera) = 4;
        break;
    }
}

// FUNCTION: BP.BIN:0x800612DC
void func_800612DC(BpPlayer *player)
{
    VECTOR projected;
    BpSurfaceState original;
    BpSurfaceState moved;
    VECTOR delta;
    int slow = *(int *)((unsigned char *)player + 0xC54);
    int mode = ((BpSurfaceState *)((unsigned char *)player + 0x80))->mode;
    int alternate = *(int *)((unsigned char *)player + 0x148);

    switch (mode) {
    case 1:
    case 2:
        delta.vx = 0;
        delta.vy = D_80095A04;
        delta.vz = 0;
        break;
    case 0:
    default:
        delta.vx = 0;
        if (((BpMotionPlayer *)player)->vertical_speed > 0xA000)
            delta.vy = D_800959FC;
        else
            delta.vy = D_80095A00;
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
            original = *(BpSurfaceState *)((unsigned char *)player + 0x80);
        else
            original = *(BpSurfaceState *)((unsigned char *)player + 0x130);
        moved = original;
        moved.x += delta.vx;
        moved.y += delta.vy;
        moved.z += delta.vz;
        if (original.height - moved.y < 0) {
            func_8006E41C(&moved, &projected);
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
    *(int *)((unsigned char *)player + 0x48D0) += delta.vx;
    *(int *)((unsigned char *)player + 0x48D4) += delta.vy;
    *(int *)((unsigned char *)player + 0x48D8) += delta.vz;
}

// FUNCTION: BP.BIN:0x80061908
int func_80061908(BpMotionPlayer *player, int limit, int divisor)
{
    int x;
    int z;
    int speed;
    int magnitude;
    unsigned short heading;
    int scaled_x;
    int folded;
    int turn;
    int angle;

    x = *(int *)((unsigned char *)player + 0x48E0);
    scaled_x = x >> 6;
    z = *(int *)((unsigned char *)player + 0x48E8);
    speed = scaled_x * scaled_x;
    scaled_x = z >> 6;
    heading = player->heading;
    speed += scaled_x * scaled_x;
    speed = SquareRoot12(speed);
    angle = ratan2(-x, -z);
    scaled_x = ((angle - (short)heading) << 20) >> 20;
    magnitude = __builtin_abs(scaled_x);
    folded = magnitude;
    if (magnitude > 0x400)
        folded = 0x800 - magnitude;
    speed = (speed * (0x400 - folded / divisor)) >> 10;
    if (limit < folded) {
        turn = limit;
        if (scaled_x < 0)
            turn = -turn;
        scaled_x = turn;
        if (magnitude >= 0x400)
            angle += scaled_x;
        else
            angle -= scaled_x;
    } else {
        if (magnitude < 0x400)
            angle = (heading << 16) >> 16;
        else
            angle = ((((heading << 16) >> 16) - 0x800) << 20) >> 20;
    }
    x = rsin(angle);
    x = -(speed * x);
    z = rcos(angle);
    z = -(speed * z);
    *(int *)((unsigned char *)player + 0x48E0) = x >> 12;
    *(int *)((unsigned char *)player + 0x48E8) = z >> 12;
    return -(magnitude < 0x400);
}

// FUNCTION: BP.BIN:0x80061ABC
unsigned int func_80061ABC(BpMotionPlayer *player)
{
    int index;
    BpSurfaceState *surface;
    int heading;
    int *history_a;
    int *history_b;
    int first;
    int second;
    int *read_a;
    int *read_b;
    int count;
    int value_a;
    int value_b;

    index = *(int *)((unsigned char *)player + 0xCB8) % 16;
    if (*(int *)((unsigned char *)player + 0x148) == 0)
        surface = (BpSurfaceState *)((unsigned char *)player + 0x28);
    else
        surface = (BpSurfaceState *)((unsigned char *)player + 0x130);
    heading = (player->heading << 20) >> 20;
    history_a = player->motion->history_a;
    history_b = player->motion->history_b;
    first = (short)func_8006EA84(surface, heading);
    second = (short)func_8006EA84(surface,
        heading = ((heading - 0x400) << 20) >> 20);
    if (surface->mode == 0) {
        history_a[index] = first;
        history_b[index] = second;
    } else {
        history_b[index] = 0;
    }
    first = 0;
    second = 0;
    count = 0;
    read_b = history_b;
    read_a = history_a;
    do {
        value_b = *read_b++;
        value_a = *read_a++;
        count++;
        first += value_a;
        second += value_b;
    } while (count < 16);
    first /= 16;
    second /= 16;
    second = (second << 20) >> 20;
    return ((first << 20) >> 4) | (second & 0xFFFFU);
}
