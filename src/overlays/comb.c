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
