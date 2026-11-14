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
