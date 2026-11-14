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
