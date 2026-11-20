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

extern int D_80095A08;
extern int D_80095A0C;

extern void func_80093D74(SVECTOR *input, SVECTOR *output);

extern BpModelHeader *D_8009E9BC;
extern BpCoordinateHeader *D_8009E9C0;
extern GsDOBJ2 *D_8009FA44;
extern GsCOORDINATE2 *D_8009FC50;

extern unsigned int D_8009FA24;
extern unsigned int D_8009FC78;
extern unsigned int D_8009FC80;
extern unsigned int D_8009FD58;
extern unsigned int D_8009FC70;
extern unsigned int D_8009FC74;
extern char *D_8009FC54;
extern char D_80095A80[30];

extern BpLightingTable D_80095AA0;
extern GsF_LIGHT D_8009FC10[3];
extern int D_8009FEC0;
extern int D_8009FEB8;

extern BpStartOffsets D_80095B78;
extern BpStartOffsets D_80095C38;
extern int func_8006889C(BpStartPlayer *player, int start, short x, short y, short z);
extern int func_8006E328(VECTOR *position, int start, BpSurfaceState *surface, int mode);

extern void func_80064B84(BpRiderTuning *tuning, int mode);
extern BpRiderCourseValues D_80095CF8;
extern BpRiderBaseValues D_80095D18;
extern BpRiderCourseFlags D_80095F98;
extern BpRiderCourseScales D_800960D8;
extern BpRiderProfile D_80097AB8[8];
extern int D_80097BA8;

extern void func_8009479C(BpAnimationState *state, GsDOBJ3 *objects,
                          GsCOORDINATE2 *coordinates, GsCOORD2PARAM *parameters,
                          int object_count, unsigned long *pmd);
extern void func_80064398(BpAnimationState *state, void *second, int animation,
                          int offset, int extent);
extern BpAnimationHeader **D_8009E9B0;
extern BpAnimationHeader **D_8009E9B4;
extern unsigned long *D_8009E9C8;
extern unsigned long *D_8009E9D0;
extern const int D_80097A6C[12];
extern const int D_80097A9C[7];
extern const unsigned char D_80097B3C[101];

extern BpAnimationFrame *func_80094910(unsigned int frame_index, BpAnimationFrame *frame,
                                       BpAnimationState *state, const int *coordinate_indices,
                                       unsigned long *pmd, int mode);
extern BpAnimationFrame *func_8009538C(unsigned int frame_index, BpAnimationFrame *frame,
                                       BpAnimationState *state, const int *coordinate_indices,
                                       unsigned long *pmd, int mode);
extern GsCOORDINATE2 *func_80064460(BpAnimationState *state, GsCOORDINATE2 *parent,
                                 int offset, int extent);

extern void func_800139C4(unsigned char *buffer);
extern void func_80013A20(unsigned char *data);
extern RECT D_800960EC;
extern RECT D_800960F4;
extern RECT D_800960FC;
extern RECT D_80096104;

extern void func_800648D8(int first_bank, int second_bank);
extern void func_80016804(void);
extern const BpCourseSoundBanks D_8009610C;
extern const BpCourseSoundBanks D_8009611C;
extern const BpModeSoundBanks D_8009612C;

extern void func_80015FC8(void);
/* The resident routine stores mode as an unsigned byte. This call passes -1
   in the full argument register. */
extern void func_80016010();
extern int func_80016220(void);

extern int func_80016A24(unsigned long voice_bit, int sample_index);

extern void func_80068F40(BpScore *score);
extern int func_80068FD0(BpScore *score);
extern int D_80097BAC[13];
extern unsigned short D_80097BE0[16];
extern unsigned short D_80097C00[8];

extern BpMoveAlias D_80097C10[6];
extern unsigned short D_8009D63E;
extern unsigned short D_8009D640;
extern unsigned short D_8009D642;
extern unsigned short D_8009D644;

extern POLY_FT4 D_8009EA48;
extern char D_800961B0[9];
extern char D_8009EA70[9];
extern int D_8009EA80;
extern int D_8009FC5C;

extern int func_80092D7C(short x, short z);
extern int func_800753D0(GsOT_TAG *tag, int value, int x, int y);
extern void func_800745E4(GsOT_TAG *tag, int x, int y);

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

// FUNCTION: BP.BIN:0x80061C24
void func_80061C24(BpMotionPlayer *player, int alternate)
{
    int acceleration;

    if (alternate != 0)
        acceleration = -D_80095A0C;
    else
        acceleration = -D_80095A08;
    acceleration *= **(int **)((unsigned char *)player + 0xC30) >> 6;
    *(int *)((unsigned char *)player + 0x48D4) += acceleration / 64;
}

// FUNCTION: BP.BIN:0x80061C88
void func_80061C88(BpMotionPlayer *player, int direction)
{
    int force;
    int x;
    int z;
    SVECTOR velocity;

    force = 0x64000;
    if (player->reverse == 0)
        force = -0x64000;
    x = 0;
    z = 0;
    switch (direction) {
    case 0:
        z = (force >> 4) / 900;
        break;
    case 1:
        z = (-force >> 4) / 900;
        break;
    case 2:
        x = (-force >> 4) / 900;
        break;
    case 3:
        x = (force >> 4) / 900;
        break;
    }
    velocity.vx = x;
    velocity.vy = 0;
    velocity.vz = z;
    func_80093D74(&velocity, &velocity);
    *(int *)((unsigned char *)player + 0x48D0) += velocity.vx * 0x3840;
    *(int *)((unsigned char *)player + 0x48D4) += velocity.vy * 0x3840;
    *(int *)((unsigned char *)player + 0x48D8) += velocity.vz * 0x3840;
}

// FUNCTION: BP.BIN:0x80061E1C
void func_80061E1C(BpMotionPlayer *player, int direction)
{
    int force;
    int x;
    int z;
    SVECTOR velocity;

    force = 0x64000;
    if (player->reverse == 0)
        force = -0x64000;
    x = 0;
    z = 0;
    switch (direction) {
    case 0:
        z = (force >> 4) / 900;
        break;
    case 1:
        z = (-force >> 4) / 900;
        break;
    case 2:
        x = (-force >> 4) / 900;
        break;
    case 3:
        x = (force >> 4) / 900;
        break;
    }
    velocity.vx = x;
    velocity.vy = 0;
    velocity.vz = z;
    func_80093D74(&velocity, &velocity);
    *(int *)((unsigned char *)player + 0x48D0) += velocity.vx * 0x3840;
    *(int *)((unsigned char *)player + 0x48D4) += velocity.vy * 0x3840;
    *(int *)((unsigned char *)player + 0x48D8) += velocity.vz * 0x3840;
}

// FUNCTION: BP.BIN:0x80062CC0
int func_80062CC0(void)
{
    unsigned int object_count;
    unsigned short coordinate_count;

    object_count = D_8009E9BC->object_count;
    coordinate_count = D_8009E9C0->coordinate_count;
    D_8009FA44 = func_8001F1A0(object_count * sizeof(GsDOBJ2));
    if (D_8009FA44 == 0)
        return 0;
    D_8009FC50 = func_8001F1A0(coordinate_count * sizeof(GsCOORDINATE2));
    return -(D_8009FC50 != 0);
}

// FUNCTION: BP.BIN:0x80062D38
void func_80062D38(void)
{
    func_8001F378(D_8009FA44);
    func_8001F378(D_8009FC50);
}

// FUNCTION: BP.BIN:0x80062D70
void func_80062D70(void)
{
    char *record_path = D_80095A80;

    D_8009FA24 = 0;
    D_8009FC78 = 0;
    D_8009FC80 = 0;
    D_8009FD58 = 0;
    D_8009FC70 = 0;
    D_8009FC74 = 0;
    D_8009FC54 = record_path;
}

// FUNCTION: BP.BIN:0x80062DB8
void func_80062DB8(int course)
{
    GsFOGPARAM fog;
    BpLightingTable table;
    BpLightColor *colors;
    int offset;

    table = D_80095AA0;

    D_8009FC10[0].vx = 0;
    D_8009FC10[0].vy = 1;
    D_8009FC10[0].vz = -1;
    offset = course * sizeof(table.colors[0]);
    colors = (BpLightColor *)((unsigned char *)&table + offset);
    D_8009FC10[0].r = colors[0].r;
    D_8009FC10[0].g = colors[0].g;
    D_8009FC10[0].b = colors[0].b;
    GsSetFlatLight(0, &D_8009FC10[0]);

    D_8009FC10[1].vx = -2;
    D_8009FC10[1].vy = 1;
    D_8009FC10[1].vz = 1;
    D_8009FC10[1].r = colors[1].r;
    D_8009FC10[1].g = colors[1].g;
    D_8009FC10[1].b = colors[1].b;
    GsSetFlatLight(1, &D_8009FC10[1]);

    D_8009FC10[2].vx = 1;
    D_8009FC10[2].vy = 1;
    D_8009FC10[2].vz = 1;
    D_8009FC10[2].r = colors[2].r;
    D_8009FC10[2].g = colors[2].g;
    D_8009FC10[2].b = colors[2].b;
    GsSetFlatLight(2, &D_8009FC10[2]);

    GsSetAmbient(colors[0].ambient | 0x400, colors[1].ambient | 0x400,
                 colors[2].ambient | 0x400);

    if (course == 6) {
        D_8009FEC0 = 1;
        GsSetLightMode(1);
        fog.dqa = -0x1C00;
        fog.dqb = 0x147AE14;
        fog.rfc = 0;
        fog.gfc = 0;
        fog.bfc = 0;
        GsSetFogParam(&fog);
    } else {
        D_8009FEC0 = 0;
        GsSetLightMode(0);
    }
}

// FUNCTION: BP.BIN:0x80063000
void func_80063000(GsOT *ot, GsOT_TAG *tags, unsigned long length)
{
    ot->org = tags;
    ot->length = length;
    ot->tag = tags + (1UL << length) - 1;
    ot->offset = 0;
    ot->point = 0;
}

// FUNCTION: BP.BIN:0x80063028
void func_80063028(BpPlayer *player)
{
    BpCameraState *camera = *(BpCameraState **)((unsigned char *)player + 0x6D8);

    D_8009FEB8 = 0;
    GsInitCoordinate2(0, &((BpMotionTransform *)camera)->coordinate);
    func_8005F964(player, 0);
    GsSetRefView2(&camera->view);
}

// FUNCTION: BP.BIN:0x800633AC
void func_800633AC(BpStartPlayer *player, int course, int side)
{
    BpStartOffsets x_offsets;
    BpStartOffsets z_offsets;
    int column;
    int x;
    short z;

    x_offsets = D_80095B78;
    z_offsets = D_80095C38;

    if (side != 0)
        column = (side != -1) * 2;
    else
        column = 1;

    x = x_offsets.course[course][column];
    z = z_offsets.course[course][column];
    if (D_80097BA4 == -1)
        x = -x;

    func_8006889C(player, 1, x, 0, z);
    player->initial_surface = player->surface;
}

// FUNCTION: BP.BIN:0x80063510
void func_80063510(BpRider *rider, int player, int character, int tuning, int option,
                   void *first, void *second, void *third)
{
    BpRiderCourseValues course_values;
    BpRiderBaseValues base_values;
    BpRiderCourseFlags course_flags;
    BpRiderCourseScales course_scales;
    BpRiderTuning *rider_tuning;
    BpSetupState *setup;
    BpSetupInputs *inputs;
    int course;
    int alternate;
    int course_flag;
    short course_value;
    int scaled_value;
    int base_value;

    course_values = D_80095CF8;
    base_values = D_80095D18;
    course_flags = D_80095F98;
    course_scales = D_800960D8;

    course = ((BpSelectionView *)D_8009FC94)->course;
    rider_tuning = &rider->tuning;
    setup = func_8001F1A0(sizeof(*setup));

    if (character >= 0) {
        alternate = 0;
    } else {
        alternate = -character;
        character = ~character;
    }

    course_flag = course_flags.values[D_80097BA8][course][character];
    base_value = base_values.values[D_80097BA8][course][character];

    rider->unknown_66 = -1;
    rider->active = 1;
    rider->flags = 0xFF;
    rider->profile = &D_80097AB8[character];
    rider->variant = alternate + 1;
    rider->character = character;
    rider->alternate = alternate;
    rider->unknown_5b = 0;

    if (character == 5)
        tuning = 12;
    if (character == 6)
        tuning = 13;

    rider_tuning->index = tuning;
    rider_tuning->word_44 = 0;
    rider_tuning->word_48 = 0;
    func_80064B84(rider_tuning, 0);

    course_value = course_values.values[course];
    rider->course_flag = course_flag;
    rider->course_value = course_value;
    scaled_value = base_value * course_scales.values[course] / 100;
    rider->unknown_6c = 0;
    rider->unknown_62 = 0;
    rider->interval = 30;
    rider->scaled_value = scaled_value;

    if (option != 0)
        rider->option = option;
    else
        rider->option = -1;

    inputs = (BpSetupInputs *)setup->unknown_0010;
    inputs->first = first;
    inputs->second = second;
    inputs->third = third;
    func_800641E8(setup, 0, alternate != 0, -(character == 6));

    rider->values = setup->values;
    rider->word_7c = setup->word_0c;
    rider->value = setup->value;
    rider->link = setup->link;
    func_8001F378(setup);
}

// FUNCTION: BP.BIN:0x8006391C
void func_8006391C(BpPlayer *player, int reverse)
{
    BpPlayerReset *state = (BpPlayerReset *)player;
    int row;
    int row_offset;
    short no_entry;
    int initial_angle;

    state->rotation.vx = 0;
    if (reverse != 0) {
        state->rotation.vy = 0x800;
        row = 0;
        row_offset = 0;
        initial_angle = 0x1CC;
        no_entry = -1;
    } else {
        state->rotation.vy = 0;
        row = 0;
        row_offset = 0;
        initial_angle = 0x1CC;
        no_entry = -1;
    }

    state->word_48f8 = initial_angle;
    state->value_0760 = 15;
    state->rotation.vz = 0;
    state->value_0ca8 = 0;
    state->value_0caa = 0;
    state->value_0cac = 0;
    state->vector_48d0.vx = 0;
    state->vector_48d0.vy = 0;
    state->vector_48d0.vz = 0;
    state->vector_48e0.vx = 0;
    state->vector_48e0.vy = 0;
    state->vector_48e0.vz = 0;
    state->word_48f0 = 0;
    state->word_48f4 = 0;
    state->distance = 0;
    state->word_07a4 = 0;
    state->word_07a8 = 0;
    state->word_0734 = 0;
    state->word_0738 = 0;
    state->word_073c = 0;
    state->word_0744 = 0;
    state->word_0748 = 0;
    state->word_074c = 0;
    state->word_0750 = 0;
    state->word_0754 = 0;
    state->word_0758 = 0;
    state->value_0740 = 0;
    state->value_0742 = 0;
    state->value_075c = state->value_075e = no_entry;
    state->value_0762 = 0;
    state->value_0768 = 1;
    state->value_076a = no_entry;
    state->value_076c = 0;
    state->value_076e = 0;
    state->value_0770 = 0;
    state->value_0772 = 0;

    do {
        int column = 0;
        int entry_offset = row_offset;

        do {
            BpResetEntry *entry = (BpResetEntry *)((unsigned char *)state + entry_offset + 0x774);

            entry->word_00 = 0;
            entry->word_04 = 0;
            entry->value_08 = 0;
            entry->value_0a = 0;
            entry_offset += sizeof(BpResetEntry);
        } while (++column < 2);
        row++;
        row_offset += sizeof(state->entries[0]);
    } while (row < 2);

    state->value_001a = 0;
    state->value_001c = -1;
    state->word_0020 = 0;
    state->word_0024 = 0;
    state->value_48fe = 0;
    state->value_4900 = 0;
    state->value_4902 = 0;
    state->value_4904 = 0;
    state->value_4906 = 0;
    state->value_4908 = 0;
    state->byte_490a = 0;
    state->byte_490b = 0;
    state->value_48fc = 0;
}

// FUNCTION: BP.BIN:0x800641E8
void func_800641E8(void *setup, void *second, int player, int special)
{
    unsigned long *pmd = 0;
    const int *coordinate_indices;
    int extent;
    int offset;
    int i;
    BpAnimationHeader **source;
    BpAnimationHeader **sequential;

    switch (player) {
    case 0:
        pmd = D_8009E9C8;
        break;
    case 1:
        pmd = D_8009E9D0;
        break;
    }

    offset = 0;
    if (special == 0) {
        coordinate_indices = D_80097A6C;
        offset = -120;
        extent = 120;
        i = 1;
        sequential = D_8009E9B0 + 1;
        do {
            ((BpAnimationState *)setup)->animations[i].header = *sequential;
            sequential++;
            i++;
        } while (i < 101);
    } else {
        coordinate_indices = D_80097A9C;
        extent = 100;
        i = 1;
        source = D_8009E9B4;
        do {
            ((BpAnimationState *)setup)->animations[i].header = source[D_80097B3C[i]];
            i++;
        } while (i < 101);
    }

    ((BpAnimationState *)setup)->word_4dc = -1;
    ((BpAnimationState *)setup)->word_4e0 = -1;
    ((BpAnimationState *)setup)->word_4e8 = 1;
    ((BpAnimationState *)setup)->pmd = pmd;
    ((BpAnimationState *)setup)->coordinate_indices = coordinate_indices;
    ((BpAnimationState *)setup)->word_4e4 = 0;
    func_8009479C(setup, ((BpAnimationState *)setup)->object_buffer,
                  ((BpAnimationState *)setup)->coordinates,
                  ((BpAnimationState *)setup)->parameters, 32, ((BpAnimationState *)setup)->pmd);

    /* Cache the header values before repointing each slot to its frame list. */
    i = 1;
    do {
        ((BpAnimationState *)setup)->frame_counts[i] =
            ((BpAnimationState *)setup)->animations[i].header->frame_count;
        ((BpAnimationState *)setup)->first_frame_starts[i] =
            ((BpAnimationState *)setup)->animations[i].header->first_frame->start;
        ((BpAnimationState *)setup)->animations[i].frames =
            &((BpAnimationState *)setup)->animations[i].header->first_frame;
        i++;
    } while (i < 101);

    func_80064398(setup, second, 1, offset, extent);
}

// FUNCTION: BP.BIN:0x80064398
void func_80064398(BpAnimationState *state, void *parent, int animation,
                   int offset, int extent)
{
    int i = 0;
    BpAnimationFrame *frame = *(animation + state->animations)->frames;
    int count = *(animation + state->frame_counts);
    unsigned int start = *(animation + state->first_frame_starts);
    GsCOORDINATE2 *coordinate;

    if (i < count) {
        do {
            frame = func_80094910(i + start, frame, state, state->coordinate_indices,
                                  state->pmd, 0);
            i++;
        } while (i < count);
    }

    coordinate = func_80064460(state, parent, offset, extent);
    ((BpAnimationPlayback *)state)->root_coordinate = coordinate;
    ((BpAnimationPlayback *)state)->saved_y = (short)coordinate->coord.t[1];
}

// FUNCTION: BP.BIN:0x80064460
GsCOORDINATE2 *func_80064460(BpAnimationState *state, GsCOORDINATE2 *parent,
                            int offset, int extent)
{
    int count = state->active_count;
    GsDOBJ3 *objects = state->objects;
    int i = 0;
    GsDOBJ3 *entry;
    GsCOORDINATE2 *root;
    int roots;
    unsigned int flags;
    unsigned int root_bits;
    unsigned int previous_flags;

    if (i < count) {
        entry = objects;
        do {
            if (entry->coord2->super == 0) {
                goto check_index;
            }
            i++;
            entry++;
        } while (i < count);

check_index:
        if (i < count) {
            root = (i++ + objects)->coord2;
            root->super = parent;
            root->flg = 0;
            root->coord.t[1] += offset + extent;

            roots = 1;
            if (i < count) {
                unsigned long address = i * sizeof(*objects);
                address += (unsigned long)objects;
                objects = (GsDOBJ3 *)address;
                do {
                    if (objects->coord2->super == 0) {
                        roots++;
                    }
                    i++;
                    objects++;
                } while (i < count);
            }

            if (roots <= 1) {
                goto done;
            }
            root = 0;
            root_bits = (unsigned int)roots << 16;
            previous_flags = D_8009FA24;
            previous_flags |= 0x10;
            flags = previous_flags | root_bits;
            D_8009FA24 = flags;
            goto done;
        }
    }

    flags = D_8009FA24;
    root = 0;
    flags |= 2;
    D_8009FA24 = flags;
done:
    return root;
}

// FUNCTION: BP.BIN:0x80064574
void func_80064574(int flip_first, int flip_second)
{
    RECT first_image = D_800960EC;
    RECT second_image = D_800960F4;
    RECT first_detail = D_800960FC;
    RECT second_detail = D_80096104;
    u_long pixels[2048];

    if (flip_first == -1) {
        StoreImage(&first_image, pixels);
        DrawSync(0);
        func_800139C4((unsigned char *)pixels);
        LoadImage(&first_image, pixels);
        DrawSync(0);
        StoreImage(&first_detail, pixels);
        DrawSync(0);
        func_80013A20((unsigned char *)pixels);
        LoadImage(&first_detail, pixels);
        DrawSync(0);
    }
    if (flip_second == -1) {
        StoreImage(&second_image, pixels);
        DrawSync(0);
        func_800139C4((unsigned char *)pixels);
        LoadImage(&second_image, pixels);
        DrawSync(0);
        StoreImage(&second_detail, pixels);
        DrawSync(0);
        func_80013A20((unsigned char *)pixels);
        LoadImage(&second_detail, pixels);
        DrawSync(0);
    }
}

// FUNCTION: BP.BIN:0x8006474C
void func_8006474C(void)
{
    BpCourseSoundBanks odd_banks = D_8009610C;
    BpCourseSoundBanks even_banks = D_8009611C;
    BpModeSoundBanks mode_banks;
    const short *course_banks;
    int mode;
    int course;

    course_banks = (rand() & 1) ? odd_banks.banks : even_banks.banks;
    mode_banks = D_8009612C;
    mode = D_8009FC94[10];
    course = D_8009FC94[0];
    func_800648D8(mode_banks.banks[mode], course_banks[course]);
    func_80016804();
}

// FUNCTION: BP.BIN:0x800648D8
void func_800648D8(int first_bank, int second_bank)
{
    func_80015FC8();
    VSync(3);
    func_80016010(first_bank, second_bank, -1);
    while (func_80016220() != 0) {
        VSync(0);
    }
}

// FUNCTION: BP.BIN:0x80064944
void func_80064944(BpAnimationPlayback *playback, int animation, int mode, int end_frame)
{
    unsigned int start = *(animation + playback->animation.first_frame_starts);
    int index = playback->animation.word_4e4 - start;
    int count = *(animation + playback->animation.frame_counts);
    BpAnimationFrame **frames = (animation + playback->animation.animations)->frames;
    BpAnimationFrame *frame;
    int limit;
    int status = 0;

    if (animation > 0 && mode != 0) {
        if (animation != playback->animation.word_4e0) {
            index = count & -(mode == 3);
        }
        limit = index + 1;

        switch (mode) {
        case 2:
            if (index < 0 || index >= count) {
                index = 0;
            }
            /* Fall through. */
        case 1:
            frame = frames[index];
            if (index < 0) {
                goto inactive;
            }
            if (index < count) {
                if (end_frame == 0) {
                    limit = index + 1;
                }
                if (end_frame > 0) {
                    if (limit < end_frame) {
                        limit = end_frame;
                    }
                } else {
                    limit = limit > count ? count : limit;
                }
                if (index < limit) {
                    do {
                        frame = func_80094910(index + start, frame, &playback->animation,
                                              playback->animation.coordinate_indices,
                                              playback->animation.pmd, 2);
                        index++;
                    } while (index < limit);
                }
            }
            if (index < 0) {
                goto inactive;
            }
            if (index < count) {
                status = -1;
            } else {
                status = -2;
            }
            break;
        case 3:
            index--;
            frame = frames[index];
            if (index >= 0 && index < count) {
                func_8009538C(index + start, frame, &playback->animation,
                              playback->animation.coordinate_indices, playback->animation.pmd, 2);
            }
            if (index < 2) {
                status = -2;
            } else if (index < count) {
                status = -1;
            } else {
inactive:
                status = -2;
            }
            break;
        }
        playback->animation.word_4e4 = index + start;
    }

    playback->root_coordinate->coord.t[1] = playback->saved_y;
    playback->animation.word_4e8 = mode;
    if (mode == 0) {
        playback->animation.word_4e8 = 1;
    }
    if (status == -1) {
        playback->animation.word_4dc = animation;
    } else {
        playback->animation.word_4dc = -1;
    }
}

// FUNCTION: BP.BIN:0x80064B7C
void func_80064B7C(void)
{
    return;
}

// FUNCTION: BP.BIN:0x80068770
void func_80068770(BpPlayerCoordinates *player)
{
    long saved_y = player->secondary_coordinate.coord.t[1];
    MATRIX *matrix;

    player->secondary_coordinate.coord = D_80058E20;
    player->body_coordinate.coord = D_80058E20;

    matrix = &player->body_coordinate.coord;
    RotMatrixX(player->body_rotation.vx, matrix);
    RotMatrixZ(player->body_rotation.vz, matrix);
    RotMatrixY(player->body_rotation.vy, matrix);

    matrix = &player->secondary_coordinate.coord;
    RotMatrixY(player->body_rotation.vy - player->secondary_heading, matrix);
    RotMatrixX(player->secondary_pitch, matrix);
    RotMatrixY(player->secondary_heading - player->body_rotation.vy, matrix);

    player->secondary_coordinate.coord.t[1] = saved_y;
    player->secondary_coordinate.flg = 0;
    player->body_coordinate.flg = 0;
}

// FUNCTION: BP.BIN:0x8006889C
int func_8006889C(BpStartPlayer *player, int start, short x, short y, short z)
{
    BpSurfaceState query;
    VECTOR position;
    VECTOR offset_position;
    BpSurfaceState *surface;
    BpStartLocation *location;
    BpRenderEntry *entry;
    int result;
    int height;

    entry = &D_8009FA28[start];
    memset_800449E8((unsigned char *)&offset_position, 0, sizeof(offset_position));
    result = 0;
    offset_position.vx = ((int *)entry->unknown_00)[0] + x;
    offset_position.vy = ((int *)entry->unknown_00)[1] + y;
    offset_position.vz = ((int *)entry->unknown_00)[2] + z;
    position = offset_position;
    query.y = 0x800;
    query.surface = -1;
    query.kind = 0;
    query.x = 0;
    query.z = 0;
    query.mode = 2;
    query.height = 0;
    query.word_18 = 0;
    query.word_54 = 0;
    query.word_50 = 0;
    surface = &player->surface;
    height = func_8006E328(&position, start, &query, 0);
    if (query.surface >= 0) {
        *surface = query;
        player->surface.surface = surface->surface;
        location = (BpStartLocation *)player->unknown_00;
        location->entry = &D_8009FA28[surface->surface];
        location->related[0] = location->entry->related[0];
        location->related[1] = location->entry->related[1];
        location->related[2] = location->entry->related[2];
        location->x = ((int *)location->entry->unknown_00)[0];
        location->y = ((int *)location->entry->unknown_00)[1];
        result = -1;
        location->z = ((int *)location->entry->unknown_00)[2];
        surface->y = height;
        surface->mode = 0;
    }
    return result;
}

// FUNCTION: BP.BIN:0x80068B10
int func_80068B10(int id)
{
    /* Preserve the original unused stack frame. */
    volatile unsigned char unused[16];

    return 0;
}

// FUNCTION: BP.BIN:0x80068B24
void func_80068B24(int sample_index, int mode)
{
    if (sample_index < 0) {
        return;
    }
    if (mode == 5) {
        if (sample_index < 34) {
            if (sample_index < 29) {
                sample_index = 27;
            } else {
                sample_index = 26;
            }
        } else {
            sample_index = 27;
        }
    } else if (mode == 6) {
        if (sample_index < 34) {
            if (sample_index >= 29) {
                sample_index = 26;
            } else {
                sample_index = 27;
            }
        } else {
            sample_index = 27;
        }
    }
    func_80016A24(0x10000UL, sample_index);
}

// FUNCTION: BP.BIN:0x80068BA0
int func_80068BA0(BpScorePlayer *player, BpScore *score)
{
    BpScoreLimit *limit = &player->limit;
    BpScoreMotion *motion = &player->motion;
    int *position = player->position;
    int value, angle, absolute_angle, slot, x, z;
    int result;
    BpScorePlayer *move_player;

    if (player->motion.heading_active) {
        angle = ratan2(player->velocity.vx, player->velocity.vz);
        value = (int)((unsigned int)(player->rotation.vy - angle) << 20) >> 20;
        absolute_angle = value < 0 ? -value : value;
        value = absolute_angle;
        angle = value > 0x400 ? 0x800 - value : value;
        value = angle >> 7;
        /* Index 8 reads the first halfword of the following table. */
        score->heading_bonus = *(unsigned short *)((unsigned long)D_80097C00 +
                                                   value * sizeof(unsigned short));
    } else {
        score->heading_bonus = 0;
    }

    slot = 0;
    do {
        angle = player->selections[slot].override;
        if (angle != 0) {
            value = angle;
            goto check_selection;
        }
        goto fallback_selection;
    found_move:
        score->move_index[slot] = result;
        goto score_move;
    fallback_selection:
        value = player->selections[slot].fallback;
    check_selection:
        score->move_index[slot] = -1;
        if (value >= 0) {
            result = 0;
            move_player = player;
            do {
                if (value == move_player->moves[0].id) {
                    goto found_move;
                }
                result++;
                move_player = (BpScorePlayer *)((unsigned char *)move_player + sizeof(BpScoreMove));
            } while (result < 13);
    score_move:
            /* A missed ID reads the first word at the next table address. */
            value = *(int *)((unsigned long)D_80097BAC + result * sizeof(int));
            if (slot != 0) {
                score->move_bonus[slot] = value + value / 2;
            } else {
                score->move_bonus[0] = value;
            }
        }
        slot++;
    } while (slot < 2);

    if (score->move_bonus[0] != 0) {
        score->duration_bonus = motion->move_duration + 20;
    } else {
        score->duration_bonus = 0;
    }

    x = position[0] - score->origin_x;
    z = position[2] - score->origin_z;
    score->distance_bonus = (unsigned long)SquareRoot0(x * x + z * z) >> 8;

    value = score->height * 100 / limit->maximum;
    if (value < 50) {
        value = value / 10;
    } else {
        if (value >= 99) {
            value = 100;
        }
        value = (value - 50) / 5 + 5;
    }
    score->height_bonus = D_80097BE0[value];

    value = motion->first_rotation;
    score->rotations[0] = value;
    score->rotation_bonus[0] = (value >= 0 ? value * 25 : -value * 25) * 2;
    if (score->reverse == 0) {
        angle = -(value > 0);
    } else {
        angle = value >> 31;
    }
    result = angle & 30;
    score->reverse_bonus = result;
    score->unknown_07 = 0;

    value = motion->second_rotation;
    score->rotations[1] = value;
    score->rotation_bonus[1] = (value >= 0 ? value * 25 : -value * 25) * 8;

    if (motion->first_rotation != 0 && motion->second_rotation != 0) {
        short bonus = 50;

        if (score->combined_flag != 0) {
            bonus = 100;
        }
        score->combined_bonus = bonus;
    }
    if (motion->reduced_score != 0) {
        func_80068F40(score);
    }
    return func_80068FD0(score);
}

// FUNCTION: BP.BIN:0x80068F40
void func_80068F40(BpScore *score)
{
    int move;

    score->combined_bonus = 0;
    score->rotation_bonus[0] = score->rotation_bonus[0] / 3;
    move = score->move_bonus[1] != 0;
    score->rotation_bonus[1] = score->rotation_bonus[1] / 3;
    ((unsigned char *)score + move)[4] = 0;
    score->duration_bonus = 0;
    score->heading_bonus = 0;
    score->unknown_07 = 0;
    score->reverse = 0;
    score->unknown_08[0] = 0;
    score->reverse_bonus = 0;
    score->rotations[0] = 0;
    score->rotations[1] = 0;
    score->move_index[0] = -1;
    score->move_index[1] = -1;
    score->unknown_26 = 0;
}

// FUNCTION: BP.BIN:0x80068FD0
int func_80068FD0(BpScore *score)
{
    return score->height_bonus + score->reverse + score->reverse_bonus
         + score->move_bonus[0] + score->move_bonus[1]
         + score->duration_bonus + score->heading_bonus
         + score->rotation_bonus[0] + score->rotation_bonus[1]
         + score->combined_bonus + score->distance_bonus;
}

// FUNCTION: BP.BIN:0x80069028
int func_80069028(BpMovePlayer *player)
{
    int allowed[4];
    int value;
    int result = 0;
    unsigned int input = player->input;
    int clear_other_moves = -(player->airborne == 0);
    unsigned int previous_input = player->previous_input;
    int i;

    {
        int empty = -1;

        for (i = 3; i >= 0; i--) {
            allowed[i] = empty;
        }
    }

    i = 0;
    if (!clear_other_moves) {
        unsigned int pressed = (input ^ previous_input) & input;

        do {
            BpMoveInput *move = player->moves + i;

        check_step:
            value = move->masks[move->step];
            if (move->step < move->step_count && (pressed & value)) {
                move->timer = 0;
                if (move->step == move->step_count - 1) {
                    int alias;

                    clear_other_moves = -1;
                    value = 1;
                    alias = 0;
                    result = move->id;
                    move->state = -1;
                    allowed[0] = result;
                    do {
                        if (result == D_80097C10[alias].id) {
                            allowed[value++] = D_80097C10[alias].alias;
                        }
                        alias++;
                    } while (alias < 5);

                    if (value < 4) {
                        int *fill = (int *)(value * sizeof(int) + (unsigned int)allowed);

                        do {
                            *fill++ = -1;
                            value++;
                        } while (value < 4);
                    }
                } else {
                    unsigned char step = move->step + 1;
                    unsigned int direction_mask;

                    move->step = step;
                    direction_mask = D_8009D642 | D_8009D644 | D_8009D63E | D_8009D640;
                    if (!(value & direction_mask) && (move->masks[step] & direction_mask)) {
                        goto check_step;
                    }
                }
            } else if (move->state == -1) {
                if ((value & input) == value) {
                    result = move->id;
                    move->timer = 0;
                } else {
                    move->timer = move->timeout + 1;
                }
            } else {
                move->timer++;
            }

            if (move->timer > move->timeout) {
                move->timer = 0;
                move->step = 0;
                move->state = 0;
            }
            i++;
        } while (i < 13);

        if (!clear_other_moves) {
            return result;
        }
        i = 0;
    }

    {
        int index = 0;

        do {
            BpMoveInput *move = player->moves + index;
            int count;
            int *next;

            value = move->id;
            count = 0;
            next = allowed;

            do {
                if (value == *next) {
                    goto check_match_count;
                }
                count++;
                next++;
            } while (count < 4);

        check_match_count:
            if (count >= 4) {
                move->timer = 0;
                move->step = 0;
                move->state = 0;
            }
            index++;
            i++;
        } while (i < 13);
    }
    return result;
}

// FUNCTION: BP.BIN:0x800694E0
int func_800694E0(BpResetEntry *entry, int amount, unsigned int positive_input,
                  unsigned int negative_input, unsigned int held)
{
    int result = amount;
    int saved_amount;
    int timer;

    if (held) {
        if (entry->value_08 == 0) {
            if (result != 0 && ((result > 0 && !positive_input) ||
                                (result < 0 && !negative_input))) {
                entry->word_00 = 30;
                entry->word_04 = amount;
            }

            saved_amount = entry->word_04;
            if (saved_amount != 0 && ((saved_amount > 0 && negative_input) ||
                                      (saved_amount < 0 && positive_input))) {
                entry->value_08 = -1;
            }
        }

        saved_amount = entry->word_04;
        if (entry->value_08 != 0) {
            entry->value_08 = ((saved_amount > 0 && positive_input) ||
                               (saved_amount < 0 && negative_input)) ? -2 : -1;
        }
    }

    if (!held) {
        if (entry->word_00 <= 0) {
            goto expired;
        }
        if (entry->value_08 == -2) {
            entry->value_0a = -1;
            saved_amount = entry->word_04;
            result = saved_amount + saved_amount / 2;
        }
    }

    if (entry->word_00 > 0) {
        timer = entry->word_00 - 1;
    } else {
    expired:
        timer = 0;
    }
    entry->word_00 = timer;
    if (!held || timer == 0) {
        entry->word_00 = 0;
        entry->value_08 = 0;
        entry->word_04 = 0;
    }
    return result;
}

// FUNCTION: BP.BIN:0x80069614
void func_80069614(BpMotionPlayer *player)
{
    int i = 0;
    BpMotionState *motion = player->motion;
    int *history_b = motion->history_b;
    int *history_a = motion->history_a;

    do {
        *history_a = 0;
        *history_b = 0;
        history_b++;
        i++;
        history_a++;
    } while (i < 16);
}

// FUNCTION: BP.BIN:0x80069648
void func_80069648(int mode)
{
    int texture_index = 4;
    int texture_stride;
    int u;

    if (mode != 0) {
        D_8009EA80 = -1;
    } else {
        D_8009EA80 = 0;
    }

    __builtin_memcpy(D_8009EA70, D_800961B0, sizeof(D_800961B0));
    D_8009FC5C = 0;
    D_8009EA70[4] = '"';

    SetPolyFT4(&D_8009EA48);
    D_8009EA48.r0 = 128;
    D_8009EA48.g0 = 128;
    D_8009EA48.b0 = 128;
    D_8009EA48.clut = GetClut(816, 464);
    D_8009EA48.tpage = GetTPage(0, 2, 384, 256);

    texture_stride = 24;
    D_8009EA48.v0 = 0;
    D_8009EA48.v1 = 0;
    D_8009EA48.v2 = 47;
    D_8009EA48.v3 = 47;
    u = texture_index * texture_stride;
    D_8009EA48.u0 = u;
    D_8009EA48.u1 = u + 47;
    D_8009EA48.u2 = u;
    D_8009EA48.u3 = u + 47;
}

// FUNCTION: BP.BIN:0x8006978C
int func_8006978C(GsOT *ot, BpDistancePlayer *player, int draw, int x, int y)
{
    GsOT_TAG *tag = &ot->org[4];
    int distance;
    int value;
    int number_x = x + 88;
    int number_y = y + 80;

    distance = func_80092D7C((short)(player->x - player->previous_x),
                             (short)(player->z - player->previous_z));
    value = (distance * 108000) / 170500;
    value = (value * 100) / 110;
    value = (value * 100) / 160;
    if (draw != 0) {
        func_800753D0(tag, value, number_x, number_y);
        func_800745E4(tag, x + 112, number_y);
    }
    return distance;
}
