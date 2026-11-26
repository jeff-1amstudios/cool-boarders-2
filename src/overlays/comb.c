#include "comb.h"
#include "bp.h"

extern PACKET *func_8008D91C(GsOT_TAG *tag, PACKET *packet, int *value,
                           int *state, int shift, int active);

extern PACKET *func_8008EBB0(GsOT_TAG *tag, PACKET *packet, int *state, int shift);

extern PACKET *func_8007D780(GsOT_TAG *tag, PACKET *packet, int mode,
                           int *state, int shift, int value);

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
extern void func_8006AC7C(GsOT *ot, CombPlayer *player, unsigned int shift);

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

extern void func_80096580(SVECTOR *source, SVECTOR *destination);

extern int D_80047D58[];
extern int D_800A4B24;
extern short *D_800A5D50;
extern int D_8009A500;
extern void *D_800A4AA4;
extern CombSceneCountView *D_800A4AA8;
extern GsRVIEW2 *D_800A5EF8;
extern int D_8005AB5C;
extern int func_80063714(FileLoadState *files, CombSettings *settings);
extern int func_800761BC(void);
extern int func_80076EF8(void);
extern void func_800808C8(void);
extern void func_800657DC(void);
extern void func_80075C54(void);
extern void func_80076864(void);
extern void func_800640EC(void *data, CombSceneEntry *entries, int count);
extern void func_80063460(CombSettings *settings, int state, int update, int alternate);
extern void func_80095448(GsRVIEW2 *view, int projection);
extern void func_80079BB8(int frames);
extern int func_8005BDCC(int state, int duration);
extern void func_8007A0DC(void);
extern void func_80063DA4(void);
extern void func_80076F58(void);
extern void func_80076204(void);

extern short D_80010CE4[];
extern GsF_LIGHT D_800A5CD8[3];
extern short D_800A5B18;
extern void *D_800A4B20;
extern int D_8009A490;
extern void func_80063DDC(int course);
extern void func_80063E24(int course);
extern void func_80097074(GsF_LIGHT *first, GsF_LIGHT *second, GsF_LIGHT *third);
extern void func_80074DB4(CombPlayer *player, short *settings, int mode);
extern void func_80070CC4(void);
extern void func_8005AF30(int course, int character_flag, int motion_flag,
                          int alternate, int active);
extern void func_80065BA0(int update);
extern void func_8006A8A8(int flag);
extern void func_80074230(void);
extern void func_80074D0C(int course);
extern void func_80075A48(void);
extern void func_80080938(int mode);
extern void func_80080AB0(void *data, int course, int state, int option);
extern void func_8007FAB8(int option);

extern GsDOBJ2 *D_800A5B04;
extern GsCOORDINATE2 *D_800A5D18;

extern unsigned int D_800A5AE4;
extern unsigned int D_800A5D40;
extern unsigned int D_800A5D48;
extern unsigned int D_800A5DA0;
extern unsigned int D_800A5D38;
extern unsigned int D_800A5D3C;
extern char *D_800A5D1C;
extern char D_8009827C[30];

extern BpLightingTable D_8009829C;
extern int D_800A5F00;

extern CombStartOffsets D_80098374;
extern CombStartOffsets D_80098434;
extern int func_80069ACC(CombStartPlayer *player, int start, short x, short y, short z);

extern CombObjectCourseValues D_800984F4;
extern CombObjectSpeeds D_80098514;
extern CombObjectLimits D_80098794;
extern CombObjectPercentages D_800988D4;
extern CombObjectEntry D_8009A410[];
extern CombTuningEntry D_8009A5F4[];

extern BpAnimationFrame *func_8009711C(unsigned int frame_index, BpAnimationFrame *frame,
                                       BpAnimationState *state, const int *coordinate_indices,
                                       unsigned long *pmd, int mode);
extern GsCOORDINATE2 *func_800654F0(BpAnimationState *state, GsCOORDINATE2 *parent,
                                 int offset, int extent);

extern RECT D_800988E8;
extern RECT D_800988F0;
extern RECT D_800988F8;
extern RECT D_80098900;
extern void func_800139C4(unsigned char *buffer);
extern void func_80013A20(unsigned char *data);

extern CombCharacterBanks D_80098908;
extern CombCourseBanks D_80098918;
extern void func_800658FC(int course_bank, int character_bank);

extern void func_80015FC8(void);
/* The resident routine stores mode as an unsigned byte. This call passes -1
   in the full argument register. */
extern void func_80016010();
extern int func_80016220(void);

extern BpAnimationFrame *func_80097B98(unsigned int frame_index, BpAnimationFrame *frame,
                                       BpAnimationState *state, const int *coordinate_indices,
                                       unsigned long *pmd, int mode);

extern unsigned short D_8009FF5C;
extern unsigned short D_8009FF64;
extern void func_80065F5C(CombPlaybackPlayerView *player, int first, int second);
extern void func_8007A218(void *effects, int x, int y, int z, int active);

extern int func_8006FB98(VECTOR *position, int start, CombSurfaceState *surface, int mode);

extern int func_80016A24(unsigned long voice_bit, int sample_index);

extern void func_8006A1A0(BpScore *score);
extern int func_8006A230(BpScore *score);
extern int D_8009A504[13];
extern unsigned short D_8009A538[16];
extern unsigned short D_8009A558[8];

extern short D_8009A568[10];
extern short D_8009A56A[9];
extern unsigned short D_8009FF6E;
extern unsigned short D_8009FF70;
extern unsigned short D_8009FF72;
extern unsigned short D_8009FF74;

extern unsigned short D_8009FF58;
extern unsigned short D_8009FF5A;
extern unsigned short D_8009FF5E;
extern unsigned short D_8009FF62;
extern int func_8006A740(CombResetEntry *entry, int amount,
                        unsigned int positive, unsigned int negative, unsigned int held);

extern POLY_FT4 D_800A4B28;
extern char D_8009899C[9];
extern char D_800A4B50[9];
extern int D_800A4B60;
extern int D_800A5D24;

extern int func_80095588(short x, short z);
extern int func_8007715C(GsOT_TAG *tag, int value, int x, int y);
extern void func_800762C4(GsOT_TAG *tag, int x, int y);

extern int func_80076FA0(GsOT_TAG *ot, int value, int x, int y, int digits);

extern int D_8009A680;
extern int D_8009A684;
extern void func_800767C4(GsOT *ot);

extern void func_8007676C(GsOT *ot, int x, int y);

extern SVECTOR D_800989A8;
extern MATRIX D_80058EE0;

extern PACKET *func_8007B5C0(GsOT_TAG *ot, PACKET *packet, int value, int *state, int shift);
extern void func_8006B370(GsOT *ot, CombHudEntry *entry);

extern CombScoreLabels D_80098A58;
extern char D_80098A90[7];
extern int D_800A5D28;
extern int sprintf(char *buffer, char *format, ...);
extern void func_80095E90(GsOT_TAG *ot, int font, char *text, int x, int y, int mode);

extern CombHudFont D_800A5B38[];
extern int D_800A5D34;
extern char D_80098A98[2];
extern void func_8009624C(int font, int mode, unsigned char red, unsigned char green, unsigned char blue);
extern int func_8006B920(CombHudEntry *entry, char *text, int line);
extern char *strcat(char *destination, char *source);

extern char D_80098B50[10];
extern char D_80098B5C[10];
extern char D_80098B68[4];
extern char D_80098B6C[5];
extern char *D_8009A688[13];
extern char *D_8009A6BC[6];
extern char *strcpy(char *destination, char *source);
extern int strlen(char *text);

extern PACKET *func_8007AC70(GsOT_TAG *tag, PACKET *packets, int *data, int different);

extern int D_800A5EA8;
extern PACKET *func_8007CD1C(GsOT_TAG *tag, PACKET *packet, int value, int *state, int shift);

extern PACKET *func_8007B3A8(GsOT_TAG *tag, PACKET *packets, int *data, int different);

extern void func_80077548(GsOT_TAG *tag, int value, int mode, int first, int second);
extern void func_8007623C(GsOT *ot, int first, int second);

extern void func_80076310(GsOT *ot, int offset);

extern void func_80078560(GsOT *ot, int x, int y, int value, int frame);

extern char D_80098B90[6];

extern CVECTOR D_80098B98[2];
extern char D_80098BA0[2];
extern char D_80098BA4[2];
extern int func_8007DBB8(int course, int value);
extern void func_8009621C(int font, unsigned char red, unsigned char green, unsigned char blue);

extern int D_800A4B68;
extern int D_800A4B6C;
extern BpCoursePoint *D_800A4B70[2];
extern int D_800A4B78[2];
extern BpCoursePoint D_8009A6D8[2];
extern BpCoursePoint D_8009A700[5];
extern BpCoursePoint D_8009A764[6];
extern BpCoursePoint D_8009A7DC[3];
extern BpCoursePoint D_8009A818[4];
extern BpCoursePoint D_8009A868[2];

extern int func_80070760(VECTOR *start, VECTOR *end, VECTOR *position, VECTOR *projection);

extern int func_8006F494(BpCollisionGeometry *geometry, SVECTOR *start, SVECTOR *end,
                         SVECTOR *hit_start, SVECTOR *hit_end, int mode);
extern void func_80073254(CombPlayer *player, SVECTOR *movement, SVECTOR *edge, int surface);
extern void func_800728EC(CombPlayer *player, SVECTOR *movement, SVECTOR *edge, int surface);

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

// FUNCTION: COMB.BIN:0x80062CF8
void func_80062CF8(CombPlayerDirectionalImpulseView *player, int mode)
{
    int impulse;
    int x;
    int z;
    SVECTOR offset;

    impulse = 0x64000;
    if (player->direction == 0)
        impulse = -0x64000;
    x = 0;
    z = 0;
    switch (mode) {
    case 0:
        z = (impulse >> 4) / 900;
        break;
    case 1:
        z = (-impulse >> 4) / 900;
        break;
    case 2:
        x = (-impulse >> 4) / 900;
        break;
    case 3:
        x = (impulse >> 4) / 900;
        break;
    }
    offset.vx = x;
    offset.vy = 0;
    offset.vz = z;
    func_80096580(&offset, &offset);
    player->acceleration_x += offset.vx * 0x3840;
    player->acceleration_y += offset.vy * 0x3840;
    player->acceleration_z += offset.vz * 0x3840;
}

// FUNCTION: COMB.BIN:0x80062E8C
void func_80062E8C(CombPlayerDirectionalImpulseView *player, int mode)
{
    int impulse;
    int x;
    int z;
    SVECTOR offset;

    impulse = 0x64000;
    if (player->direction == 0)
        impulse = -0x64000;
    x = 0;
    z = 0;
    switch (mode) {
    case 0:
        z = (impulse >> 4) / 900;
        break;
    case 1:
        z = (-impulse >> 4) / 900;
        break;
    case 2:
        x = (-impulse >> 4) / 900;
        break;
    case 3:
        x = (impulse >> 4) / 900;
        break;
    }
    offset.vx = x;
    offset.vy = 0;
    offset.vz = z;
    func_80096580(&offset, &offset);
    player->acceleration_x += offset.vx * 0x3840;
    player->acceleration_y += offset.vy * 0x3840;
    player->acceleration_z += offset.vz * 0x3840;
}

// FUNCTION: COMB.BIN:0x80063020
int func_80063020(FileLoadState *files, int mode)
{
    int duration;
    short success;
    int initial_state;
    short alternate;
    unsigned short saved_option;
    int first_flag;
    int second_flag;
    int i;
    CombPlayer **players;
    CombPlayer **player_base;
    CombSessionPlayerView *player;
    CombSettings *settings;
    short *flags;

    SetDispMask(1);
    duration = 0x1B8;
    if (mode < 0)
        D_800A4B24 = D_80047D58[-mode];
    settings = D_800A5D5C;
    flags = D_800A5D50;
    first_flag = ((CombSessionSettingsView *)settings)->first_flag;
    second_flag = ((CombSessionSettingsView *)settings)->second_flag;
    flags[0] = 0;
    flags[1] = 0;
    first_flag = first_flag != 0;
    second_flag = -(second_flag != 0);
    D_8009A500 = first_flag;
    *(int *)D_8009A4FC = second_flag;
    success = func_80063714(files, settings);
    success &= func_8005B568();
    success &= func_800761BC();
    success &= func_80076EF8();
    func_800808C8();
    func_800657DC();
    func_80075C54();
    func_80076864();
    if (success != 0) {
        func_800640EC(D_800A4AA4, D_800A5AE8, D_800A4AA8->count);
        if (mode == 0) mode = 4;
        else if (mode > 0) {
            if (mode == 2) mode = 9;
            else mode = 7;
        }
        else mode = 8;
    } else {
        mode = 0;
    }
    initial_state = mode;
    if ((unsigned int)mode < 2) goto cleanup;
    if ((unsigned int)(mode - 10) < 2) goto cleanup;
    player_base = D_800A5B10;
    for (;;) {
        if ((unsigned int)(mode - 12) < 2) goto cleanup;
        alternate = -(D_800A5D5C->course == 1);
        if (D_800A5D5C->course == 6) alternate = -1;
        if (D_800A5D5C->course == 15) alternate = -1;
        func_80063460(D_800A5D5C, mode, 0, alternate);
        func_80095448(D_800A5EF8, 400);
        func_80079BB8(300);
        if (mode < 6) {
            if (mode >= 2) mode = func_8005BDCC(2, 0);
        }
        if (mode >= 6 && (mode < 9 || (alternate = -1, mode == 9))) {
            func_80063460(D_800A5D5C, mode, -1, alternate);
            saved_option = *(unsigned short *)D_800A5D5C->unknown_16;
            switch (mode) {
            case 6:
            case 7:
                duration = ((CombSessionPlayerView *)D_800A5B10[0])->duration;
                *(unsigned short *)D_800A5D5C->unknown_16 = 16;
                break;
            case 8:
                duration = 1800;
                i = rand();
                func_80060940((CombPlayerCameraView *)D_800A5B10[0], -((i & 1) == 0) & 5);
                break;
            case 9:
                duration = ((CombSessionPlayerView *)D_800A5B10[0])->duration;
                break;
            }
            i = 0;
            players = player_base;
            do {
                ((CombSessionPlayerView *)players[i])->progress = 0;
                i++;
            } while (i < 2);
            ((CombSessionPlayerView *)D_800A5B10[0])->secondary_view = -1;
            mode = func_8005BDCC(mode, duration);
            switch (initial_state) {
            case 7:
                mode = 1;
                break;
            case 8:
            case 9:
                mode = -(((CombSessionPlayerView *)D_800A5B10[0])->progress < duration) & 13;
                break;
            }
            *(unsigned short *)D_800A5D5C->unknown_16 = saved_option;
        }
        func_8007A0DC();
        if (D_8005AB5C == -1) mode = 0;
        if ((unsigned int)mode < 2) goto cleanup;
        if ((unsigned int)(mode - 10) < 2) break;
    }
cleanup:
    VSync(0);
    SpuSetKey(0, 0x7F80FF);
    func_8005B808();
    func_80063DA4();
    func_80076F58();
    func_80076204();
    switch (mode) {
    case 1: duration = 2; break;
    case 10: duration = 3; break;
    case 11: duration = 4; break;
    case 12: duration = 5; break;
    case 13: duration = 0; break;
    default: duration = 1; break;
    }
    return duration;
}

// FUNCTION: COMB.BIN:0x80063460
void func_80063460(CombSettings *settings, int state, int update, int alternate)
{
    int character_flag;
    int course;
    short motion_flag;
    short second_flag;
    int active;
    int i;
    CombPlayer **players;

    character_flag = -(D_80010CE4[settings->character] != 0);
    course = settings->course;
    active = -1;
    if (update == 0)
        motion_flag = -(D_800A5D50[0] != 0);
    else
        motion_flag = -1;
    second_flag = -(D_800A5D50[1] != 0);
    func_80063DDC(course);
    func_80063E24(course);
    func_80097074(&D_800A5CD8[0], &D_800A5CD8[1], &D_800A5CD8[2]);
    switch (state) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
    case 8:
    case 9:
        i = 0;
        players = D_800A5B10;
        do {
            if (i == 0) {
                func_80074DB4(players[0], &settings->character, 0);
                ((CombPlayerNameView *)players[0])->name[0] = 'U';
                ((CombPlayerNameView *)players[0])->name[1] = 'S';
                ((CombPlayerNameView *)players[0])->name[2] = 'E';
                ((CombPlayerNameView *)players[0])->name[3] = 'R';
            } else {
                func_80074DB4(players[i], &settings->second_character, 0);
                ((CombPlayerNameView *)players[i])->name[0] = 'C';
                ((CombPlayerNameView *)players[i])->name[1] = 'P';
                ((CombPlayerNameView *)players[i])->name[2] = 'U';
                ((CombPlayerNameView *)players[i])->name[3] = ' ';
            }
            i++;
        } while (i < 2);
        break;
    case 6:
        active = 0;
        break;
    }
    func_80070CC4();
    func_8005AF30(course, character_flag, motion_flag, alternate, active);
    func_80065BA0(update);
    func_8006A8A8(second_flag);
    func_80074230();
    func_80074D0C(course);
    func_80075A48();
    D_800A5B18 = 0;
    func_80080938(0);
    if (course < 5) {
        if (course >= 3 && D_800A4B20 != 0) {
            func_80080AB0(D_800A4B20, course, (short)D_8009A490, D_8009A4FC[0]);
            D_8009A490 = 1;
        }
    }
    func_8007FAB8(D_8009A4FC[0]);
    func_800166A4();
    func_80016804();
}

// FUNCTION: COMB.BIN:0x80063DA4
void func_80063DA4(void)
{
    func_8001F378(D_800A5B04);
    func_8001F378(D_800A5D18);
}

// FUNCTION: COMB.BIN:0x80063DDC
void func_80063DDC(int course)
{
    char *record_path = D_8009827C;

    D_800A5AE4 = 0;
    D_800A5D40 = 0;
    D_800A5D48 = 0;
    D_800A5DA0 = 0;
    D_800A5D38 = 0;
    D_800A5D3C = 0;
    D_800A5D1C = record_path;
}

// FUNCTION: COMB.BIN:0x80063E24
void func_80063E24(int course)
{
    GsFOGPARAM fog;
    BpLightingTable table;
    BpLightColor *colors;
    int offset;

    table = D_8009829C;

    D_800A5CD8[0].vx = 0;
    D_800A5CD8[0].vy = 1;
    D_800A5CD8[0].vz = -1;
    offset = course * sizeof(table.colors[0]);
    colors = (BpLightColor *)((unsigned char *)&table + offset);
    D_800A5CD8[0].r = colors[0].r;
    D_800A5CD8[0].g = colors[0].g;
    D_800A5CD8[0].b = colors[0].b;
    GsSetFlatLight(0, &D_800A5CD8[0]);

    D_800A5CD8[1].vx = -2;
    D_800A5CD8[1].vy = 1;
    D_800A5CD8[1].vz = 1;
    D_800A5CD8[1].r = colors[1].r;
    D_800A5CD8[1].g = colors[1].g;
    D_800A5CD8[1].b = colors[1].b;
    GsSetFlatLight(1, &D_800A5CD8[1]);

    D_800A5CD8[2].vx = 1;
    D_800A5CD8[2].vy = 1;
    D_800A5CD8[2].vz = 1;
    D_800A5CD8[2].r = colors[2].r;
    D_800A5CD8[2].g = colors[2].g;
    D_800A5CD8[2].b = colors[2].b;
    GsSetFlatLight(2, &D_800A5CD8[2]);

    GsSetAmbient(colors[0].ambient | 0x400, colors[1].ambient | 0x400,
                 colors[2].ambient | 0x400);

    if (course == 6) {
        D_800A5F00 = 1;
        GsSetLightMode(1);
        fog.dqa = -0x1C00;
        fog.dqb = 0x147AE14;
        fog.rfc = 0;
        fog.gfc = 0;
        fog.bfc = 0;
        GsSetFogParam(&fog);
    } else {
        D_800A5F00 = 0;
        GsSetLightMode(0);
    }
}

// FUNCTION: COMB.BIN:0x8006406C
void func_8006406C(GsOT *ot, GsOT_TAG *tags, unsigned long length)
{
    ot->org = tags;
    ot->length = length;
    ot->tag = tags + (1UL << length) - 1;
    ot->offset = 0;
    ot->point = 0;
}

// FUNCTION: COMB.BIN:0x80064094
void func_80064094(CombPlayerCameraView *player)
{
    CombCameraModeView *camera = player->camera;

    D_800A5EF8 = 0;
    GsInitCoordinate2(0, &camera->camera_coordinate);
    func_80060940(player, 0);
    GsSetRefView2(&camera->view);
}

// FUNCTION: COMB.BIN:0x80064418
void func_80064418(CombStartPlayer *player, int course, int side)
{
    CombStartOffsets x_offsets;
    CombStartOffsets z_offsets;
    int column;
    int x;
    short z;

    x_offsets = D_80098374;
    z_offsets = D_80098434;

    if (side != 0)
        column = (side != -1) * 2;
    else
        column = 1;

    x = x_offsets.course[course][column];
    z = z_offsets.course[course][column];
    if ((*(int *)D_8009A4FC) == -1)
        x = -x;

    func_80069ACC(player, 1, x, 0, z);
    player->initial_surface = player->surface;
}

// FUNCTION: COMB.BIN:0x8006457C
void func_8006457C(CombObjectState *object, int player, int index, int kind,
                   int value, int x, int y, int z)
{
    CombObjectTables tables;
    int course;
    CombSetupState *setup;
    CombObjectParameters *parameters;
    int reverse;
    int limit;
    int speed;
    unsigned short course_value;
    int percent;

    tables.course = D_800984F4;
    tables.speeds = D_80098514;
    tables.limits = D_80098794;
    tables.percentages = D_800988D4;
    course = D_800A5D5C->course;
    parameters = &object->parameters;
    setup = func_8001F1A0(0x4F4);
    if (index >= 0) {
        reverse = 0;
    } else {
        reverse = -index;
        index = ~index;
    }
    limit = tables.limits.values[D_8009A500][course][index];
    speed = tables.speeds.values[D_8009A500][course][index];
    object->value_66 = -1;
    object->active = 1;
    object->flag = 0xFF;
    object->entry = &D_8009A410[index];
    object->count = reverse + 1;
    object->index = index;
    object->reverse = reverse;
    object->flag_5b = 0;
    if (index == 5) kind = 12;
    if (index == 6) kind = 13;
    parameters->kind = kind;
    parameters->value_44 = 0;
    parameters->value_48 = 0;
    func_80065BA8(parameters, 0);
    course_value = tables.course.course[course];
    object->limit = limit;
    object->course_value = course_value;
    percent = tables.percentages.course[course];
    object->value_6c = 0;
    object->value_62 = 0;
    object->timer = 30;
    object->speed = speed * percent / 100;
    if (value != 0) object->value_64 = value;
    else object->value_64 = 0xFFFF;
    *(int *)((char *)setup + 0x10) = x;
    *(int *)((char *)setup + 0x14) = y;
    *(int *)((char *)setup + 0x18) = z;
    func_80065278(setup, 0, player, -(index == 6));
    object->values = setup->values;
    object->word_0c = setup->word_0c;
    object->value = setup->value;
    object->link = setup->link;
    func_8001F378(setup);
}

// FUNCTION: COMB.BIN:0x80064984
void func_80064984(CombPlayerReset *state, int reverse)
{
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

    state->word_4900 = initial_angle;
    state->value_0760 = 15;
    state->rotation.vz = 0;
    state->value_0cb0 = 0;
    state->value_0cb2 = 0;
    state->value_0cb4 = 0;
    state->vector_48d8.vx = 0;
    state->vector_48d8.vy = 0;
    state->vector_48d8.vz = 0;
    state->vector_48e8.vx = 0;
    state->vector_48e8.vy = 0;
    state->vector_48e8.vz = 0;
    state->word_48f8 = 0;
    state->word_48fc = 0;
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
            CombResetEntry *entry = (CombResetEntry *)((unsigned char *)state + entry_offset + 0x774);

            entry->word_00 = 0;
            entry->word_04 = 0;
            entry->value_08 = 0;
            entry->value_0a = 0;
            entry_offset += sizeof(CombResetEntry);
        } while (++column < 2);
        row++;
        row_offset += sizeof(state->entries[0]);
    } while (row < 2);

    state->value_001a = 0;
    state->value_001c = -1;
    state->word_0020 = 0;
    state->word_0024 = 0;
    state->value_4906 = 0;
    state->value_4908 = 0;
    state->value_490a = 0;
    state->value_490c = 0;
    state->value_490e = 0;
    state->value_4910 = 0;
    state->byte_4912 = 0;
    state->byte_4913 = 0;
    state->value_4904 = 0;
}

// FUNCTION: COMB.BIN:0x80065428
void func_80065428(BpAnimationState *state, void *parent, int animation,
                   int offset, int extent)
{
    int i = 0;
    BpAnimationFrame *frame = *(animation + state->animations)->frames;
    int count = *(animation + state->frame_counts);
    unsigned int start = *(animation + state->first_frame_starts);
    GsCOORDINATE2 *coordinate;

    if (i < count) {
        do {
            frame = func_8009711C(i + start, frame, state, state->coordinate_indices,
                                  state->pmd, 0);
            i++;
        } while (i < count);
    }

    coordinate = func_800654F0(state, parent, offset, extent);
    ((BpAnimationPlayback *)state)->root_coordinate = coordinate;
    ((BpAnimationPlayback *)state)->saved_y = (short)coordinate->coord.t[1];
}

// FUNCTION: COMB.BIN:0x800654F0
GsCOORDINATE2 *func_800654F0(BpAnimationState *state, GsCOORDINATE2 *parent,
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
            previous_flags = D_800A5AE4;
            previous_flags |= 0x10;
            flags = previous_flags | root_bits;
            D_800A5AE4 = flags;
            goto done;
        }
    }

    flags = D_800A5AE4;
    root = 0;
    flags |= 2;
    D_800A5AE4 = flags;
done:
    return root;
}

// FUNCTION: COMB.BIN:0x80065604
void func_80065604(int flip_first, int flip_second)
{
    RECT first_image = D_800988E8;
    RECT second_image = D_800988F0;
    RECT first_detail = D_800988F8;
    RECT second_detail = D_80098900;
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

// FUNCTION: COMB.BIN:0x800657DC
void func_800657DC(void)
{
    CombCharacterBanks characters = D_80098908;
    CombCourseBanks courses = D_80098918;

    func_800658FC(courses.values[D_800A5D5C->course],
                  characters.values[D_800A5D5C->character]);
    func_80016804();
}

// FUNCTION: COMB.BIN:0x800658FC
void func_800658FC(int first_bank, int second_bank)
{
    func_80015FC8();
    VSync(3);
    func_80016010(first_bank, second_bank, -1);
    while (func_80016220() != 0) {
        VSync(0);
    }
}

// FUNCTION: COMB.BIN:0x80065968
void func_80065968(BpAnimationPlayback *playback, int animation, int mode, int end_frame)
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
                        frame = func_8009711C(index + start, frame, &playback->animation,
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
                func_80097B98(index + start, frame, &playback->animation,
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

// FUNCTION: COMB.BIN:0x80065BA0
void func_80065BA0(int update)
{
    return;
}

// FUNCTION: COMB.BIN:0x80065BA8
int func_80065BA8(CombObjectParameters *parameters, int mode)
{
    int kind = parameters->kind;
    int response;
    int speed;
    int selected;
    int value_24 = 0xC94E;
    int value_28 = 0x7308;

    switch (mode) {
    case 1:
        response = D_8009A5F4[kind].response[2];
        ((CombTuningParameters *)parameters)->turn = D_8009A5F4[kind].turn[2] * 0x1000 / 360;
        speed = (D_8009A5F4[kind].speed[2] * 0xA000 / 36) * 11 / 17;
        selected = 1;
        break;
    case 2:
    case 3:
        response = D_8009A5F4[kind].response[1];
        ((CombTuningParameters *)parameters)->turn = D_8009A5F4[kind].turn[1] * 0x1000 / 360;
        speed = (D_8009A5F4[kind].speed[1] * 0xA000 / 36) * 11 / 17;
        selected = 2;
        break;
    case 0:
    default:
        response = D_8009A5F4[kind].response[0];
        ((CombTuningParameters *)parameters)->turn = D_8009A5F4[kind].turn[0] * 0x1000 / 360;
        speed = (D_8009A5F4[kind].speed[0] * 0xA000 / 36) * 11 / 17;
        selected = 0;
    }
    ((CombTuningParameters *)parameters)->value_24 = value_24;
    ((CombTuningParameters *)parameters)->value_28 = value_28;
    ((CombTuningParameters *)parameters)->value_30 = 0x10000;
    ((CombTuningParameters *)parameters)->value_38 = 4;
    ((CombTuningParameters *)parameters)->response = (response + 4) * 0xF000 / 0x4F1A0;
    speed = speed * 110 / 100;
    ((CombTuningParameters *)parameters)->speed = speed;
    ((CombTuningParameters *)parameters)->response_scale = response * 0x19000 / 0x4F1A0;
    ((CombTuningParameters *)parameters)->speed_steps[3] = speed / 2;
    ((CombTuningParameters *)parameters)->speed_steps[0] = speed / 12;
    ((CombTuningParameters *)parameters)->speed_steps[1] = speed / 6;
    ((CombTuningParameters *)parameters)->speed_steps[2] = speed / 3;
    ((CombTuningParameters *)parameters)->speed_steps[4] = speed * 2 / 3;
    ((CombTuningParameters *)parameters)->speed_steps[5] = speed * 5 / 6;
    ((CombTuningParameters *)parameters)->speed_steps[6] = speed * 11 / 12;
    ((CombTuningParameters *)parameters)->weight = D_8009A5F4[kind].weight;
    ((CombTuningParameters *)parameters)->unknown_4c = selected;
    return mode;
}

// FUNCTION: COMB.BIN:0x8006963C
int func_8006963C(CombPlaybackPlayerView *player, int frame)
{
    int animation;
    int *position = &player->surfaces[0].x;
    int *previous_position = &player->surfaces[1].x;
    int previous_mode = player->surfaces[1].mode;
    unsigned int buttons = D_8009FF5C | D_8009FF64;
    BpAnimationPlayback *playback = &player->playback;
    int active;
    int index;
    int i;

    animation = player->direction + 0x24;
    player->surfaces[0].kind = buttons;
    if (frame < 30) {
        if (frame == 0) {
            frame = 1;
            player->velocity_x /= 2;
            player->velocity_z /= 2;
        }
        func_80065F5C(player, 0, 0);
        if (previous_mode == 0) {
            frame++;
        }
    } else {
        func_80065968(playback, animation, 2, 0);
        player->playback.animation.word_4e0 = animation;
    }
    if (player->surfaces[0].mode == 0) {
        active = 1;
    } else {
        active = 1;
        if (player->surfaces[0].mode >= 0) {
            active = player->surfaces[0].mode > 2;
        }
    }
    func_8007A218(player->effects,
                 position[0] - previous_position[0],
                 position[1] - previous_position[1],
                 position[2] - previous_position[2], active);
    player->frame_count++;
    index = player->history_count;
    if (index < 7500) {
        player->history_count = index + 1;
        player->input_history[index] = buttons;
    }
    for (i = 2; i != 0; i--) {
        player->surfaces[i] = player->surfaces[i - 1];
    }
    return frame;
}

// FUNCTION: COMB.BIN:0x800699A0
void func_800699A0(CombPlayerCoordinates *player)
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

// FUNCTION: COMB.BIN:0x80069ACC
int func_80069ACC(CombStartPlayer *player, int start, short x, short y, short z)
{
    CombSurfaceState query;
    VECTOR position;
    VECTOR offset_position;
    CombSurfaceState *surface;
    CombStartLocation *location;
    CombSceneEntry *entry;
    int result;
    int height;

    entry = &D_800A5AE8[start];
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
    height = func_8006FB98(&position, start, &query, 0);
    if (query.surface >= 0) {
        *surface = query;
        player->surface.surface = surface->surface;
        location = (CombStartLocation *)player->unknown_00;
        location->entry = &D_800A5AE8[surface->surface];
        location->related[0] = location->entry->linked_entries[0];
        location->related[1] = location->entry->linked_entries[1];
        location->related[2] = location->entry->linked_entries[2];
        location->x = ((int *)location->entry->unknown_00)[0];
        location->y = ((int *)location->entry->unknown_00)[1];
        result = -1;
        location->z = ((int *)location->entry->unknown_00)[2];
        surface->y = height;
        surface->mode = 0;
    }
    return result;
}

// FUNCTION: COMB.BIN:0x80069D84
void func_80069D84(int sample, int mode)
{
    int lower;

    if (sample < 0) {
        return;
    }
    if (mode == 5) {
        if (sample < 34) {
            lower = sample < 29;
            sample = 28;
            if (lower) {
                sample = 29;
            }
            goto play;
        }
    } else {
        if (mode != 6) {
            goto play;
        }
        if (sample < 34) {
            lower = sample < 29;
            sample = 29;
            if (!lower) {
                sample = 28;
            }
            goto play;
        }
    }
    sample = 29;
play:
    func_80016A24(0x10000, sample);
}

// FUNCTION: COMB.BIN:0x80069E00
int func_80069E00(CombScorePlayer *player, BpScore *score)
{
    BpScoreLimit *limit = &player->limit;
    BpScoreMotion *motion = &player->motion;
    int *position = player->position;
    int value, angle, absolute_angle, slot, x, z;
    int result;
    CombScorePlayer *move_player;

    if (player->motion.heading_active) {
        angle = ratan2(player->velocity.vx, player->velocity.vz);
        value = (int)((unsigned int)(player->rotation.vy - angle) << 20) >> 20;
        absolute_angle = value < 0 ? -value : value;
        value = absolute_angle;
        angle = value > 0x400 ? 0x800 - value : value;
        value = angle >> 7;
        /* Index 8 reads the first halfword of the following table. */
        score->heading_bonus = *(unsigned short *)((unsigned long)D_8009A558 +
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
                move_player = (CombScorePlayer *)((unsigned char *)move_player + sizeof(BpScoreMove));
            } while (result < 13);
    score_move:
            /* A missed ID reads the first word at the next table address. */
            value = *(int *)((unsigned long)D_8009A504 + result * sizeof(int));
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
    score->height_bonus = D_8009A538[value];

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
        func_8006A1A0(score);
    }
    return func_8006A230(score);
}

// FUNCTION: COMB.BIN:0x8006A1A0
void func_8006A1A0(BpScore *score)
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

// FUNCTION: COMB.BIN:0x8006A230
int func_8006A230(BpScore *score)
{
    return score->height_bonus + score->reverse + score->reverse_bonus
         + score->move_bonus[0] + score->move_bonus[1]
         + score->duration_bonus + score->heading_bonus
         + score->rotation_bonus[0] + score->rotation_bonus[1]
         + score->combined_bonus + score->distance_bonus;
}

// FUNCTION: COMB.BIN:0x8006A288
int func_8006A288(CombMovePlayer *player)
{
    int result = 0;
    int clear = -(player->airborne == 0);
    unsigned int buttons = player->buttons;
    unsigned int previous = player->previous_buttons;
    int keep[4];
    int i, offset;
    int cleanup_offset;
    int cleanup_count;
    int *fill_cursor;
    int *cleanup_cursor;
    int *cursor;
    CombMoveState *move;
    CombMoveWork work;
    unsigned int pressed;
    int invalid = -1;
    int sentinel;
    unsigned int directions;
    unsigned char step;
    int table_offset;

    for (i = 3; i >= 0; i--) keep[i] = invalid;
    if (!clear) {
        pressed = (buttons ^ previous) & buttons;
        sentinel = -1;
        for (i = 0, offset = 0x890; i < 13; i++, offset += 0x48) {
            move = (CombMoveState *)((unsigned char *)player + offset);
        check_step:
            work.mask = move->masks[move->step];
            if (move->step < move->step_count && (pressed & work.mask)) {
                move->timer = 0;
                if (move->step == move->step_count - 1) {
                    clear = -1;
                    work.count = 1;
                    table_offset = 0;
                    result = move->id;
                    cursor = keep + 1;
                    move->complete = sentinel;
                    keep[0] = result;
                    do {
                        if (result == *(short *)((unsigned char *)D_8009A568 + table_offset)) {
                            *cursor = *(short *)((unsigned char *)D_8009A56A + table_offset);
                            work.count++;
                            cursor++;
                        }
                        table_offset += 4;
                    } while (table_offset < 20);
                    if (work.count < 4) {
                        fill_cursor = (int *)(work.count * 4 + (unsigned int)keep);
                        do {
                            *fill_cursor = sentinel;
                            work.count++;
                            fill_cursor++;
                        } while (work.count < 4);
                    }
                } else {
                    step = move->step + 1;
                    move->step = step;
                    directions = D_8009FF72 | D_8009FF74 | D_8009FF6E | D_8009FF70;
                    if (!(work.mask & directions) && (move->masks[step] & directions)) goto check_step;
                }
            } else {
                if (move->complete == sentinel) {
                    if ((work.mask & buttons) == work.mask) {
                        result = move->id;
                        move->timer = 0;
                        goto timeout;
                    }
                    move->timer = move->timeout + 1;
                } else {
                    move->timer++;
                }
            }
        timeout:
            if (move->timer > move->timeout) {
                move->timer = 0;
                move->step = 0;
                move->complete = 0;
            }
        }
    }
    if (clear) {
        for (i = 0, cleanup_offset = 0x890; i < 13; i++, cleanup_offset += 0x48) {
            move = (CombMoveState *)((unsigned char *)player + cleanup_offset);
            work.id = move->id;
            for (cleanup_count = 0, cleanup_cursor = keep; cleanup_count < 4; cleanup_count++, cleanup_cursor++) {
                if (work.id == *cleanup_cursor) break;
            }
            if (cleanup_count >= 4) {
                move->timer = 0;
                move->step = 0;
                move->complete = 0;
            }
        }
    }
    return result;
}

// FUNCTION: COMB.BIN:0x8006A4F8
void func_8006A4F8(CombInputPlayer *player, unsigned int side)
{
    CombResetEntry *first_y = &player->entries[0][0];
    CombResetEntry *second_y = &player->entries[0][1];
    CombResetEntry *first_x = &player->entries[1][0];
    CombResetEntry *second_x = &player->entries[1][1];
    CombInputMotion *motion = &player->motion;
    unsigned int buttons = player->buttons;
    unsigned int held;
    int x, y;
    int first, second;
    int original_first, original_second;

    if (player->airborne == 0) {
        held = buttons & ((unsigned int)D_8009FF62 << (side << 4));
    } else {
        held = 0;
    }
    {
        CombInputResults result_y = {0, 0};
        CombInputResults result_x = {0, 0};
        result_y.first = func_8006A740(first_y, motion->y,
            buttons & ((unsigned int)D_8009FF5E << (side << 4)),
            buttons & ((unsigned int)D_8009FF5C << (side << 4)), held);
        result_x.first = func_8006A740(first_x, motion->x,
            buttons & ((unsigned int)D_8009FF58 << (side << 4)),
            buttons & ((unsigned int)D_8009FF5A << (side << 4)), held);
        if (held == 0) {
            y = motion->y;
            x = motion->x;
        } else {
            first = first_y->word_04 * motion->y;
            y = motion->y & (first >> 31);
            first = first_x->word_04 * motion->x;
            x = motion->x & (first >> 31);
        }
        result_y.second = func_8006A740(second_y, y,
            buttons & ((unsigned int)D_8009FF5E << (side << 4)),
            buttons & ((unsigned int)D_8009FF5C << (side << 4)), held);
        result_x.second = func_8006A740(second_x, x,
            buttons & ((unsigned int)D_8009FF58 << (side << 4)),
            buttons & ((unsigned int)D_8009FF5A << (side << 4)), held);
        first = result_y.first < 0 ? -result_y.first : result_y.first;
        second = result_y.second < 0 ? -result_y.second : result_y.second;
        if (first >= second) {
            motion->y = result_y.first;
        } else {
            motion->y = result_y.second;
        }
        original_first = result_x.first;
        first = original_first;
        if (first < 0) first = -first;
        original_second = result_x.second;
        second = original_second;
        if (second < 0) second = -second;
        if (first >= second) {
            motion->x = original_first;
        } else {
            motion->x = original_second;
        }
    }
}

// FUNCTION: COMB.BIN:0x8006A740
int func_8006A740(CombResetEntry *entry, int amount, unsigned int positive_input,
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

// FUNCTION: COMB.BIN:0x8006A874
void func_8006A874(CombMotionPlayer *player)
{
    int i = 0;
    CombMotionState *motion = player->motion;
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

// FUNCTION: COMB.BIN:0x8006A8A8
void func_8006A8A8(int mode)
{
    int texture_index = 4;
    int texture_stride;
    int u;

    if (mode != 0) {
        D_800A4B60 = -1;
    } else {
        D_800A4B60 = 0;
    }

    __builtin_memcpy(D_800A4B50, D_8009899C, sizeof(D_8009899C));
    D_800A5D24 = 0;
    D_800A4B50[4] = '"';

    SetPolyFT4(&D_800A4B28);
    D_800A4B28.r0 = 128;
    D_800A4B28.g0 = 128;
    D_800A4B28.b0 = 128;
    D_800A4B28.clut = GetClut(816, 464);
    D_800A4B28.tpage = GetTPage(0, 2, 384, 256);

    texture_stride = 24;
    D_800A4B28.v0 = 0;
    D_800A4B28.v1 = 0;
    D_800A4B28.v2 = 47;
    D_800A4B28.v3 = 47;
    u = texture_index * texture_stride;
    D_800A4B28.u0 = u;
    D_800A4B28.u1 = u + 47;
    D_800A4B28.u2 = u;
    D_800A4B28.u3 = u + 47;
}

// FUNCTION: COMB.BIN:0x8006A9EC
int func_8006A9EC(GsOT *ot, CombPlayerRenderStateView *state, unsigned int draw, int x, int y)
{
    BpDistancePlayer *player = (BpDistancePlayer *)state;
    GsOT_TAG *tag = &ot->org[4];
    int distance;
    int value;
    int number_x = x + 88;
    int number_y = y + 80;

    distance = func_80095588((short)(player->x - player->previous_x),
                             (short)(player->z - player->previous_z));
    value = (distance * 108000) / 170500;
    value = (value * 100) / 110;
    value = (value * 100) / 160;
    if (draw != 0) {
        func_8007715C(tag, value, number_x, number_y);
        func_800762C4(tag, x + 112, number_y);
    }
    return distance;
}

// FUNCTION: COMB.BIN:0x8006AB40
void func_8006AB40(GsOT *ot, unsigned int value, int first, int second)
{
    GsOT_TAG *tag = ot->org + 3;

    value /= 1000;
    second -= 104;
    if (value > 99) {
        value = 99;
    }
    func_80076FA0(tag, value, first, second, 2);
}

// FUNCTION: COMB.BIN:0x8006AB94
void func_8006AB94(GsOT *ot, int different)
{
    if (D_8009A680 != 0) {
        func_800767C4(ot);
    }
    if (different == 0 && --D_8009A684 == 0) {
        D_8009A684 = 10;
        D_8009A680 = -(D_8009A680 == 0);
    }
}

// FUNCTION: COMB.BIN:0x8006AC14
int func_8006AC14(GsOT *ot, int move_left, int position, int x, int y)
{
    if (move_left != 0) {
        position -= 4;
    } else {
        position += 4;
    }
    if (position < 80) {
        position = 80;
    }

    if (position > 160) {
        position = 160;
    }
    func_8007676C(ot, position + x, y - 52);
    return position;
}

// FUNCTION: COMB.BIN:0x8006AC7C
void func_8006AC7C(GsOT *ot, CombPlayer *player, unsigned int shift)
{
    SVECTOR v0, v1, v2, v3;
    long p, flag;
    CombSurfaceState *surface = &((CombSurfaceMotionPlayer *)player)->surface;
    int *position = &surface->x;
    int selected = ((CombSurfaceMotionPlayer *)player)->alternate_surface.word_18;
    POLY_FT4 *quad;
    long depth;
    int height;

    memset_800449E8((unsigned char *)&v0, 0, sizeof(v0));
    v0.vx = 96;
    v0.vy = 0;
    v0.vz = 96;
    memset_800449E8((unsigned char *)&v1, 0, sizeof(v1));
    v1.vx = 96;
    v1.vy = 0;
    v1.vz = -96;
    memset_800449E8((unsigned char *)&v2, 0, sizeof(v2));
    v2.vx = -96;
    v2.vy = 0;
    v2.vz = 96;
    v3 = D_800989A8;
    PushMatrix();
    SetRotMatrix(&D_80058EE0);
    quad = (POLY_FT4 *)D_80058ACC;
    *quad = D_800A4B28;
    if (surface->mode == 0) {
        height = position[1];
    } else if (selected == -1) {
        height = ((CombSurfaceMotionPlayer *)player)->alternate_surface.height;
    } else {
        height = surface->height;
    }
    height -= position[1];
    v0.vy = height;
    v1.vy = height;
    v2.vy = height;
    v3.vy = height;
    depth = RotTransPers4(&v0, &v1, &v2, &v3,
                         (long *)&quad->x0, (long *)&quad->x1,
                         (long *)&quad->x2, (long *)&quad->x3, &p, &flag);
    if (flag >= 0) {
        depth >>= shift;
        quad->code |= 2;
        ((GsOT_TAG *)quad)->p = ot->org[depth].p;
        ot->org[depth].p = (unsigned long)quad;
        quad++;
    }
    D_80058ACC = (PACKET *)quad;
    PopMatrix();
}

// FUNCTION: COMB.BIN:0x8006B2D0
void func_8006B2D0(GsOT *ot, CombHudEntry *entry, int *state, int direction, int shift, int extra)
{
    GsOT_TAG *tag = ot->org + 3;
    int value = *(int *)(entry->unknown + 0x14);

    if (direction < 0) {
        value = -value;
    } else if (direction == 0) {
        value = 0;
    }
    if (*state >= 0) {
        D_80058ACC = func_8007B5C0(tag, D_80058ACC, value, state, shift);
    }
    if (D_800A5D24 != 0) {
        func_8006B370(ot, entry);
    }
}

// FUNCTION: COMB.BIN:0x8006B370
void func_8006B370(GsOT *ot, CombHudEntry *entry)
{
    CombScoreLabels labels;
    char lines[14][32];
    int values[14];
    BpScore *score = (BpScore *)entry;
    int i;
    int x;
    int y;

    labels = D_80098A58;
    values[0] = score->total;
    values[1] = score->height_bonus;
    values[2] = score->reverse;
    values[3] = score->reverse_bonus;
    values[4] = score->move_bonus[0];
    values[5] = score->move_bonus[1];
    values[6] = score->duration_bonus;
    values[7] = score->heading_bonus;
    values[8] = score->rotation_bonus[0];
    values[9] = score->rotation_bonus[1];
    values[10] = score->combined_bonus;
    values[11] = score->distance_bonus;
    values[12] = score->unknown_07;
    values[13] = score->unknown_08[0];
    x = -152;
    y = -16;
    for (i = 0; i < 14; i++) {
        sprintf(lines[i], D_80098A90, labels.text[i], values[i]);
    }
    for (i = 0; i < 12; i++) {
        func_80095E90(ot->org + 3, D_800A5D28, lines[i], x, y, 0);
        y += 8;
    }
}

// FUNCTION: COMB.BIN:0x8006B528
void func_8006B528(GsOT *ot, CombHudEntry *entry, int *state, int frozen, int duration)
{
    char lines[2][256];
    int widths[2];
    TILE tile;
    DR_MODE mode;
    GsOT_TAG *tag = ot->org + 3;
    int font_index = D_800A5D34;
    CombHudFont *font = &D_800A5B38[font_index];
    int height = D_800A5B38[font_index].height;
    unsigned char red = font->red;
    unsigned char green = font->green;
    unsigned char blue = font->blue;
    int time;
    int shade;
    int i;
    int count;
    int x, y;

    time = *state;
    if (time >= 0) {
        if (time < duration / 2) shade = time * 10;
        else shade = (duration - time) * 10;
        if ((short)shade > 128) shade = 128;
        if ((short)shade < 0) shade = 0;
        func_8009624C(D_800A5D34, -1, 128, 128, 128);
        for (i = 0; i < 2; i++) {
            widths[i] = func_8006B920(entry, lines[i], i);
        }
        if (widths[0] + widths[1] >= 34) {
            count = 2;
        } else {
            count = 1;
            if (widths[1] != 0) {
                if (widths[0] != 0) {
                    strcat(lines[0], D_80098A98);
                    widths[0]++;
                }
                widths[0] += widths[1];
                strcat(lines[0], lines[1]);
            }
        }
        for (i = 0; i < count; i++) {
            widths[i] *= 8;
            x = -widths[i] / 2;
            y = 72;
            if (i != 0) y = height + 72;
            font->red = shade;
            font->green = shade;
            font->blue = shade;
            func_80095E90(ot->org + 3, D_800A5D34, lines[i], x, y, 0);
            SetTile(&tile);
            tile.code |= 2;
            SetDrawMode(&mode, 0, 0, GetTPage(0, 2, 0, 0), 0);
            tile.x0 = x;
            tile.y0 = y;
            tile.w = widths[i];
            tile.r0 = shade;
            tile.g0 = shade;
            tile.b0 = shade;
            tile.h = height;
            *(TILE *)D_80058ACC = tile;
            ((GsOT_TAG *)D_80058ACC)->p = tag->p;
            tag->p = (unsigned long)D_80058ACC;
            D_80058ACC += sizeof(TILE);
            func_80096D78(tag, &mode);
        }
        func_8009624C(D_800A5D34, 0, red, green, blue);
        if (frozen == 0) {
            if (*state < duration) (*state)++;
            else *state = -1;
        }
    }
}

// FUNCTION: COMB.BIN:0x8006B920
int func_8006B920(CombHudEntry *entry, char *text, int line)
{
    int value;
    char number[16];

    text[0] = 0;
    if (line == 0) {
        if (((CombTrickLabelView *)entry)->fakie != 0 &&
            (((CombTrickLabelView *)entry)->rotation.combined != 0 || ((CombTrickLabelView *)entry)->first_trick >= 0)) {
            __builtin_memcpy(text, D_80098B50, sizeof(D_80098B50));
        }
        if (((CombTrickLabelView *)entry)->backside != 0) {
            strcat(text, D_80098B5C);
        }
        if (((CombTrickLabelView *)entry)->rotation.parts.flip != 0) {
            value = (unsigned int)(int)((CombTrickLabelView *)entry)->rotation.parts.flip >> 31;
            if (((CombTrickLabelView *)entry)->rotation.parts.spin != 0) {
                value += 2;
                if (((CombTrickLabelView *)entry)->twist != 0) value += 2;
            }
            strcat(text, D_8009A6BC[value]);
        }
        value = ((CombTrickLabelView *)entry)->rotation.parts.spin;
        if (value != 0) {
            if (value < 0) value = -value;
            sprintf(number, D_80098B68, value * 180);
            strcat(text, number);
        }
    } else {
        value = ((CombTrickLabelView *)entry)->first_trick;
        if (value >= 0) strcpy(text, D_8009A688[value]);
        value = ((CombTrickLabelView *)entry)->second_trick;
        if (value >= 0) {
            strcat(text, D_80098B6C);
            strcat(text, D_8009A688[value]);
        }
    }
    value = strlen(text);
    while (value != 0) {
        if (text[value - 1] != ' ') break;
        text[value - 1] = 0;
        value--;
    }
    return value;
}

// FUNCTION: COMB.BIN:0x8006BB20
void func_8006BB20(GsOT *ot, int *data, int different)
{
    D_80058ACC = func_8007AC70(&ot->org[3], D_80058ACC, data, different);
}

// FUNCTION: COMB.BIN:0x8006BB5C
void func_8006BB5C(GsOT *ot, int value, int *state, int shift, int extra)
{
    D_800A5EA8 = extra - 1;
    D_80058ACC = func_8007CD1C(&ot->org[3], D_80058ACC, value, state, shift);
}

// FUNCTION: COMB.BIN:0x8006BBB4
void func_8006BBB4(GsOT *ot, int mode, int *state, int shift, int value)
{
    D_80058ACC = func_8007D780(&ot->org[3], D_80058ACC, mode, state, shift, value);
}

// FUNCTION: COMB.BIN:0x8006BC04
void func_8006BC04(GsOT *ot, int *state, int shift)
{
    D_80058ACC = func_8008EBB0(ot->org, D_80058ACC, state, shift);
}

// FUNCTION: COMB.BIN:0x8006BC40
void func_8006BC40(GsOT *ot, int *state, int shift, int *value)
{
    CombSettings *settings = D_800A5D5C;
    GsOT_TAG *tag = ot->org;
    PACKET *packet = D_80058ACC;
    int active = *(int *)(settings->unknown_16 + 2) != 3;

    D_80058ACC = func_8008D91C(tag, packet, value, state, shift, active);
}

// FUNCTION: COMB.BIN:0x8006BC98
void func_8006BC98(GsOT *ot, int *data, int different)
{
    D_80058ACC = func_8007B3A8(&ot->org[3], D_80058ACC, data, different);
}

// FUNCTION: COMB.BIN:0x8006BCD4
void func_8006BCD4(GsOT *ot, int value, int mode, int first, int second)
{
    func_80077548(&ot->org[3], value, mode, first - 0x84, second + 0x50);
    func_8007623C(ot, first, second);
}

// FUNCTION: COMB.BIN:0x8006BE3C
void func_8006BE3C(GsOT *ot, int *state, int frozen, int duration, int angle)
{
    int scale;

    if (*state >= 0) {
        if (*state < duration) {
            scale = -150;
            if (angle >= 0) {
                scale = 150;
            } else {
                angle = -angle;
            }
            if (angle > 1024) {
                angle = 1024;
            }
            scale = scale * angle;
            func_80076310(ot, scale / 1024);
        }
        if (frozen == 0) {
            ++*state;
            if (*state >= (duration << 1)) {
                *state = 0;
            }
        }
    }
}

// FUNCTION: COMB.BIN:0x8006C2C8
void func_8006C2C8(GsOT *ot, int *state, int frozen, int value)
{
    int frame = *state;

    if (frozen == 0) {
        ++frame;
    }
    frame %= 48;
    func_80078560(ot, -152, 88, value, frame / 6);
    *state = frame;
}

// FUNCTION: COMB.BIN:0x8006C358
void func_8006C358(GsOT *ot, int *counter, int different, int value, int direction)
{
    char text[16];
    int clamped = 0;
    int x;
    int y = -32;

    if (*counter > 0) {
        x = *counter * -8 + 176;
        if (x < 96) {
            x = 96;
            clamped = -1;
        }
        sprintf(text, D_80098B90, value);
        func_8009624C(D_800A5D34, 0, 96, 96, 0);
        func_80095E90(&ot->org[3], D_800A5D34, text, x, y, 0);
        func_8009624C(D_800A5D34, 0, 128, 128, 128);
    } else {
        *counter = 0;
    }

    if (different == 0) {
        if (direction == -1) {
            if (clamped == -1) {
                *counter = 60;
            } else if (*counter < 60) {
                ++*counter;
            }
        } else {
            --*counter;
        }
    }
}

// FUNCTION: COMB.BIN:0x8006C4AC
void func_8006C4AC(GsOT *ot, int first, int second, int unused)
{
    DVECTOR position;
    DVECTOR size;
    DVECTOR marker_size;
    CVECTOR colors[2];
    int values[2];
    DR_MODE mode;
    int course = D_800A5D5C->course;
    TILE *tile = (TILE *)D_80058ACC;
    int width;
    int flash;
    int depth;
    int i;
    int progress;
    int side;

    position.vx = -144;
    position.vy = -104;
    size.vx = 112;
    size.vy = 13;
    marker_size.vx = 4;
    marker_size.vy = 4;
    __builtin_memcpy(colors, D_80098B98, sizeof(colors));
    values[0] = first;
    values[1] = second;
    width = size.vx - marker_size.vx;
    i = VSync(-1) % 30 - 15;
    flash = -(i > 0);
    depth = 3;
    for (i = 0; i < 2; ++i) {
        if (i != 0 || flash != 0) {
            progress = width * func_8007DBB8(course, values[i]) / 255;
            side = D_8005AB5C == 1 ? i : 1 - i;
            ((GsOT_TAG *)tile)->num = 3;
            tile->code = 0x60;
            tile->w = marker_size.vx;
            tile->h = marker_size.vy;
            tile->r0 = colors[side].r;
            tile->g0 = colors[side].g;
            tile->b0 = colors[side].b;
            tile->x0 = position.vx + progress;
            tile->y0 = position.vy + (marker_size.vy + 1) * side + 2;
            ((GsOT_TAG *)tile)->p = ot->org[depth].p;
            ot->org[depth].p = (unsigned long)tile;
            ++depth;
            ++tile;
        }
    }
    ((GsOT_TAG *)tile)->num = 3;
    tile->code = 0x60;
    tile->x0 = position.vx;
    tile->y0 = position.vy;
    tile->w = size.vx;
    tile->h = size.vy;
    tile->r0 = 128;
    tile->g0 = 128;
    tile->b0 = 128;
    tile->code |= 2;
    ((GsOT_TAG *)tile)->p = ot->org[depth].p;
    ot->org[depth].p = (unsigned long)tile;
    ++tile;
    D_80058ACC = (PACKET *)tile;
    SetDrawMode(&mode, 0, 0, GetTPage(0, 1, 0, 0), 0);
    func_80096D78(&ot->org[depth], &mode);
    i = position.vx - 8;
    progress = position.vy + (size.vy - 8) / 2;
    func_8009621C(D_800A5D28, 32, 255, 32);
    func_80095E90(&ot->org[3], D_800A5D28, D_80098BA0, i, progress, 0);
    i = position.vx + size.vx;
    func_8009621C(D_800A5D28, 32, 32, 255);
    func_80095E90(&ot->org[3], D_800A5D28, D_80098BA4, i, progress, 0);
    func_8009621C(D_800A5D28, 128, 128, 128);
}

// FUNCTION: COMB.BIN:0x8006C8F8
void func_8006C8F8(int course, int direction)
{
    int limit;
    int list;
    int point;

    switch (course) {
    case 0:
    case 9:
    case 15:
        limit = 0x7FFF;
        break;
    default:
        limit = 0x200;
        break;
    }
    D_800A4B68 = limit;

    switch (course) {
    case 3:
        D_800A4B70[0] = D_8009A6D8;
        D_800A4B78[0] = 2;
        D_800A4B78[1] = 0;
        D_800A4B6C = 1;
        break;
    case 5:
        D_800A4B70[0] = D_8009A700;
        D_800A4B70[1] = D_8009A764;
        D_800A4B78[1] = 6;
        D_800A4B78[0] = course;
        D_800A4B6C = 2;
        break;
    case 9:
        D_800A4B70[0] = D_8009A7DC;
        D_800A4B78[0] = 3;
        D_800A4B78[1] = 0;
        D_800A4B6C = 1;
        break;
    case 15:
        D_800A4B70[0] = D_8009A818;
        D_800A4B70[1] = D_8009A868;
        D_800A4B78[0] = 4;
        D_800A4B78[1] = 2;
        D_800A4B6C = 2;
        break;
    default:
        D_800A4B78[0] = 0;
        D_800A4B78[1] = 0;
        D_800A4B6C = 0;
        break;
    }

    if (direction == -1) {
        for (list = 0; list < D_800A4B6C; list++) {
            for (point = 0; point < D_800A4B78[list]; point++) {
                BpCoursePoint *entry = (BpCoursePoint *)(
                    (unsigned int)(point * sizeof(BpCoursePoint)) + (unsigned int)D_800A4B70[list]);
                long *x = &entry->position.vx;
                *x = -*x;
            }
        }
    }
}

// FUNCTION: COMB.BIN:0x8006D400
int func_8006D400(CombPlayer *player, BpCoursePoint *points, VECTOR *position,
                  int list, int index)
{
    int unused[2]; /* Preserves the reference stack frame. */
    int matched = -1;
    int segment = 0;

    if (D_800A4B78[list] - 1 > 0) {
        do {
            if (points[segment].index <= index && index <= points[segment + 1].index) {
                matched = func_80070760(&points[segment].position,
                                      &points[segment + 1].position, position, 0);
                if (matched != 0) {
                    break;
                }
            } else {
                matched = 0;
            }
            segment++;
        } while (segment < D_800A4B78[list] - 1);
    }
    if (matched == 0) {
        segment = -1;
    }
    return segment;
}

// FUNCTION: COMB.BIN:0x8006F13C
int func_8006F13C(CombPlayer *player, VECTOR *start, VECTOR *end, int entry_index, int mode)
{
    CombSceneEntry *entry = &D_800A5AE8[entry_index];
    BpCollisionEntries entries = {entry_index, entry->linked_entries[0], entry->linked_entries[1],
                                 entry->linked_entries[2]};
    SVECTOR local_start;
    SVECTOR local_end;
    SVECTOR hit_starts[4];
    SVECTOR hit_ends[4];
    SVECTOR movement;
    SVECTOR edge;
    int surface;
    int best;
    int selected;
    int i;
    int distance;
    BpCollisionModel *model;
    BpCollisionGeometry *geometry;

    best = -1;
    selected = -1;
    surface = 0;
    i = 0;
    do {
        int candidate_index = *(int *)((char *)&entries + i * sizeof(int));

        if (candidate_index >= 0) {
            entry = &D_800A5AE8[candidate_index];
            local_start.vx = start->vx - ((int *)entry->unknown_00)[0];
            local_start.vy = start->vy - ((int *)entry->unknown_00)[1];
            local_start.vz = start->vz - ((int *)entry->unknown_00)[2];
            local_end.vx = end->vx - ((int *)entry->unknown_00)[0];
            local_end.vy = end->vy - ((int *)entry->unknown_00)[1];
            local_end.vz = end->vz - ((int *)entry->unknown_00)[2];
            switch (mode) {
            case 0: {
                SVECTOR *segment_start = &local_start;

                geometry = (*(BpCollisionModel **)(entry->unknown_12 + 0xE))->geometry;
                distance = func_8006F494(geometry, segment_start, &local_end,
                                         &hit_starts[i], &hit_ends[i], 1);
                if (distance >= 0 && best < distance) {
                    best = distance;
                    selected = i;
                }
                surface = *(short *)((char *)player + 0x730);
                break;
            }
            case 1:
                geometry = 0;
                model = *(BpCollisionModel **)(entry->unknown_12 + 0x1A);
                if (model != 0) {
                    geometry = model->geometry;
                }
                if (geometry != 0) {
                    distance = func_8006F494(geometry, &local_start, &local_end,
                                             &hit_starts[i], &hit_ends[i], 0);
                    if (distance >= 0 && (best < 0 || distance < best)) {
                        best = distance;
                        selected = i;
                    }
                    surface = *(short *)((char *)player + 0x732);
                }
                break;
            }
        }
        i++;
    } while (i < 4);
    if (selected >= 0) {
        movement.vx = end->vx - start->vx;
        movement.vy = end->vy - start->vy;
        movement.vz = end->vz - start->vz;
        edge.vx = hit_ends[selected].vx - hit_starts[selected].vx;
        edge.vy = hit_ends[selected].vy - hit_starts[selected].vy;
        edge.vz = hit_ends[selected].vz - hit_starts[selected].vz;
        switch (surface) {
        case 6:
        case 30:
        case 76:
            func_80073254(player, &movement, &edge, surface);
            break;
        default:
            func_800728EC(player, &movement, &edge, surface);
            break;
        }
    } else {
        surface = 0;
    }
    return surface;
}

// FUNCTION: COMB.BIN:0x8006FAAC
int func_8006FAAC(CombSurfacePlayer *player)
{
    CombSurfaceState query;
    int result;
    int surface;

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
    func_8006FB98(&player->position, player->start, &query, 0);
    surface = query.surface;
    if (surface >= 0) {
        player->surface = surface;
        player->location.entry = &D_800A5AE8[surface];
        player->location.related[0] = player->location.entry->linked_entries[0];
        player->location.related[1] = player->location.entry->linked_entries[1];
        player->location.related[2] = player->location.entry->linked_entries[2];
        player->location.x = ((int *)player->location.entry->unknown_00)[0];
        player->location.y = ((int *)player->location.entry->unknown_00)[1];
        result = -1;
        player->location.z = ((int *)player->location.entry->unknown_00)[2];
    } else {
        result = 0;
    }
    return result;
}
