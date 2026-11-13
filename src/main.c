#include "main.h"

extern void boot_noop(void);
extern void func_8001228C(void);
extern void func_80015814(void);
extern void func_80016EE4(void);
extern void func_8001B9A4(void);

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

extern void func_8003F094(void);
extern void func_8001BA78(void);

extern void func_800178AC(void);

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

extern int D_80058E10;
extern int D_80058E18;

extern void *func_8001F1A0(unsigned int size);

extern int func_8003CB2C(void);
extern int func_8003CA24(int mode);

extern void func_8001B228(void);
extern void func_8001F824(void);
extern void func_8001787C(void);

extern void func_8002ECB0(short x0, short y0, short x1, short y1);
extern void func_8001E8C0(void);
extern void func_8003CB3C(void);

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

extern DiscFileRecord disc_file_records[];

extern int D_80045364;
extern FileReadRequest D_800502E8[20];

extern void func_800140A0(unsigned char status, unsigned char *result);
extern int D_80045354;
extern int D_80045350;

extern volatile int D_80045368;
/* Volatile view of fields shared with the CD callback; layout is unchanged. */
extern volatile FileReadRequest shared_read_requests[20];
extern int func_8001EC78(void *data);
extern int func_8001EEE0(void *data);
extern int func_8001F378(void *allocation);
extern void *func_8001F28C(unsigned int size);
extern void func_80013FB8(int index);

extern void func_80013C24(void);
extern void func_80015508(void);
extern void func_800143E0(int mode, ModeDisplayData *data);
extern void func_80014388(void);
extern ModeDisplayData D_80058090;

extern int func_80013C70(void);
extern unsigned int func_8001B320(int port, unsigned char *state);

extern SPRT *func_80012950(unsigned long *ordering_table, SPRT *sprite,
                            short x, short y, SpriteTexture *texture, unsigned char shade);
extern DR_TPAGE *func_80012B6C(unsigned long *ordering_table, DR_TPAGE *packet,
                              unsigned int tpage);

extern void func_80015EC8(int volume, int effect_volume, int cd_volume);
extern SpuCommonAttr D_80058068;
extern unsigned long D_8005AB2C;
extern unsigned short D_80050568;

extern int D_80058F90;
extern short D_80057FBC;
extern int D_800478CC;
extern unsigned long *volatile D_80058F8C;
extern unsigned long volatile D_80058F84;

extern void *func_8001579C(void);
extern int func_8001EFE4(void *image, int x, int y, int clut_x, int clut_y);

extern short D_8004534C;
extern void *func_8001EA54(char *filename, void *destination, int sector_offset, int sector_count);
extern char D_80010200[17];

extern volatile SoundBankDescriptor D_800478D8[];
extern short volatile D_80058F80;
extern unsigned long volatile D_80058F88;
extern short volatile D_80058F94;
extern unsigned long volatile D_80058F98;
extern unsigned long volatile D_80058F9C;
extern unsigned long *volatile D_80058FA0;
extern int volatile D_80058FA4;
extern unsigned short D_80057FC0;

extern short D_80057FBE;

extern short D_80057FBA;

extern unsigned short D_800478D0;

extern void *D_80047CF4;
/* Volatile view of the sound status shared with the transfer callback. */
extern volatile unsigned short sound_transfer_status;

extern SoundDiscTransfer *sound_disc_transfer;
extern SoundTransferBank sound_transfer_banks[2];
extern void func_80016354(unsigned char status, unsigned char *result);

extern short D_80057FB8;

extern volatile SoundBankFileRecord D_800478D4[];
extern void func_80016278(void);

/* Separate volatile views preserve the initialization store order. */
extern volatile int sound_bank_sample_count;
extern volatile unsigned short sound_bank_pitch;

extern unsigned char D_80058CC8[64];
extern int D_80058B10;
extern int D_8005A7B8;

extern int func_80016A24(unsigned long voice_bit, int sample_index);

extern int D_8005A7AC;
extern int D_80058B0C;
extern SoundCommand sound_commands[16];

extern volatile SoundCommand pending_sound_commands[16];

/* The byte aliases preserve standalone minute stores; both objects are CdlLOC. */
extern volatile CdlLOC cd_audio_current_pos;
extern volatile unsigned char cd_audio_current_minute;
extern volatile CdlLOC cd_audio_pending_pos;
extern volatile unsigned char cd_audio_pending_minute;
extern CdlLOC cd_audio_starts[26];
extern CdlLOC cd_audio_ends[26];
extern int D_80058048;
extern int D_80047D0C;
extern char D_80010214[16];

extern void func_80017470(unsigned char status, unsigned char *result);
extern short D_80047CF8[10];
extern int D_80050640[10];
extern CdlLOC D_80050678;
extern int D_8005067C;
extern int D_80050680;
extern int D_80050684;
extern CdlLOC D_80050690;
extern int D_80050694;

extern int D_80050688;
extern int D_8005068C;

extern void func_80044524(int mode);
extern void func_80044578(void);
extern void func_8003CA7C(void); /* SDK _bu_init */

extern void func_800445B0(void);

extern long CloseEvent(long event);

extern long TestEvent(long event);

extern long OpenEvent(unsigned long event_class, long specification, long mode,
                      long (*callback)(void));
extern long EnableEvent(long event);

extern CardResultTable card_result_table;
extern long _card_info(long channel);
extern long _card_clear(long channel);

extern CardLoadResultTable card_load_result_table;
extern long _card_load(long channel);

extern unsigned short D_80047D10;
extern int D_80047D14;
extern int func_80018808(void);

extern int format(char *device);
extern char card_format_device[6];

extern unsigned char *func_80012C30(unsigned long *ordering_table, unsigned char *packet, RECT *texture,
                                      unsigned short clut, int x, int y, unsigned char *string,
                                      CVECTOR *color, unsigned short mode);
extern TILE *func_80018604(unsigned long *ordering_table, TILE *tile);

extern RECT card_message_texture;
extern CardMessageColors card_message_colors;
extern unsigned char card_saving_text[15];
extern unsigned char card_missing_text[16];
extern unsigned char card_changed_text[33];
extern unsigned char card_write_error_text[13];
extern unsigned char card_not_saved_text[14];
extern unsigned char card_save_off_text[15];
extern unsigned char card_saved_text[17];

#include "card_format.h"

extern void func_800178CC(long *events);
extern int func_80017B64(long *events);
extern int func_80017CFC(long *events);
extern void func_80017A00(long *events);
extern int open(char *filename, int mode);
extern int lseek(int fd, int offset, int origin);
extern int read(int fd, void *buffer, int size);
extern int close(int fd);
extern int func_80018768(unsigned char *data);
extern char card_save_filename[23];

extern unsigned int func_800181D4(unsigned char *data, unsigned int length);
extern unsigned char card_save_signature[9];

extern int func_80018968(char *filename, CardSaveBlock *block);
extern void func_80018CC0(unsigned char *data);
extern int func_80017A58(long *events);
extern int write(int fd, void *buffer, int size);

extern CardSaveTitle card_save_title;
extern CardSavePalette card_save_palette;
extern CardSaveFrames card_save_frames;
extern int func_80018044(unsigned char *destination, int mode, int frame_count,
                          unsigned char *palette, unsigned char *frames, char *title);

extern CardGameSettings card_game_settings;
extern CardAudioSettings card_audio_settings;
extern CardGameSettings card_game_options;
extern CardGameRecord card_game_records[10];
extern CardGameResult card_game_results[3];
extern CardGameProgress card_game_progress;
extern CardGameEntry card_game_entries[3];

extern char card_load_filename[23];
extern int func_80019058(CardSaveHeader *header);
extern int func_80019154(CardSaveHeader *header, CardGamePayload *payload);

extern unsigned char card_load_signature[9];
extern int D_80045370;

extern char card_extra_load_filename[23];
extern int func_80019530(CardSaveHeader *header);
extern int func_800195F8(CardSaveHeader *header, CardExtraPayload *payload);

extern unsigned char card_extra_load_signature[9];

extern volatile int card_extra_selected_record;
extern volatile unsigned short card_extra_setting_0;
extern volatile unsigned short card_extra_setting_1;
extern volatile unsigned short card_extra_setting_2;
extern volatile unsigned short card_extra_setting_3;
extern volatile unsigned short card_extra_setting_4;
extern volatile short card_extra_setting_5;
extern volatile unsigned short card_extra_setting_6;
extern short card_extra_mode;
extern unsigned short card_extra_record_value;

extern CardGhostFilename card_ghost_filename;
extern int func_80019920(char *filename, CardGhostSaveBlock *block,
                          unsigned int slot, int title_index);
extern void func_8001A3DC(void *destination, void *source, unsigned int length);


extern unsigned char card_ghost_signature[9];
extern CardSaveTitle card_ghost_title;
extern CardSavePalette card_ghost_palette_0;
extern CardGhostFrame card_ghost_frame_0;
extern CardSavePalette card_ghost_palette_1;
extern CardGhostFrame card_ghost_frame_1;
extern CardSavePalette card_ghost_palette_2;
extern CardGhostFrame card_ghost_frame_2;
extern CardSavePalette card_ghost_palette_3;
extern CardGhostFrame card_ghost_frame_3;
extern CardSavePalette card_ghost_palette_4;
extern CardGhostFrame card_ghost_frame_4;
extern CardSavePalette card_ghost_palette_5;
extern CardGhostFrame card_ghost_frame_5;
extern CardSavePalette card_ghost_palette_6;
extern CardGhostFrame card_ghost_frame_6;

extern int func_8001A4D4(int slot, unsigned int *data);

extern CardGhostFilename card_ghost_read_filename;
extern int func_8001A608(CardGhostHeader *header, int check_payload);

extern unsigned char card_ghost_read_signature[9];

#include "card_ghost_read.h"

extern CardGhostFilename card_board_filename;
extern int func_8001AAD0(char *filename, CardBoardSaveBlock *block, unsigned int slot);
extern void func_8001ADC8(CardBoardRecord *record, unsigned short *values, unsigned char *data);

extern CardSaveTitle card_board_title;
extern CardSavePalette card_board_palette;
extern CardSaveFrames card_board_frames;
extern unsigned char card_board_signature[9];

extern int func_8001AEE0(int slot);

extern CardGhostFilename card_board_read_filename;
extern int func_8001B1AC(CardBoardRecord *record);

extern unsigned char card_board_read_signature[9];

extern unsigned char pad_receive_buffers[2][34];
extern unsigned char pad_send_buffers[2][34];
extern int InitPAD(unsigned char *buffer0, int length0, unsigned char *buffer1, int length1);
extern int SendPAD(unsigned char *buffer0, int length0, unsigned char *buffer1, int length1);
extern int StartPAD(void);


extern int D_80047D18;

extern int D_80050698;
extern int D_8005069C;
extern int D_800506B0;
extern int D_800506B4;
extern char sio_filename[5];
extern void func_8003D0D4(void);
/* Command-dependent third argument: serial settings or a callback. */
extern int func_8003D714(int command, int selector, ...);
extern int func_8001C068(int event);
extern void func_8001BF08(void);

extern int D_8005AB5C;
extern unsigned char D_800506A0[7];
extern unsigned char D_800506A7;
extern unsigned char D_800506A8[7];
extern unsigned char D_800506AF;

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

// FUNCTION: SCUS_943.58:0x80013654
POLY_FT4 *func_80013654(unsigned long *ordering_table, POLY_FT4 *packet,
                      int spacing, int x, int y, SpriteTexture *texture)
{
    int index = 0;
    short base_x = x;
    POLY_FT4 *primitive;

    for (; index < 3; index++) {
        primitive = packet++;
        ((PrimitiveTag *)primitive)->length = 9;
        primitive->code = 0x2C;
        primitive->x2 = base_x;
        primitive->x0 = x + (index + 1) * spacing;
        primitive->x2 += index * spacing;
        primitive->x3 = primitive->x2 + texture->w;
        primitive->x1 = primitive->x0 + texture->w;
        primitive->y0 = primitive->y1 = y + (index < 2 ? texture->h / (index + 2) : 0);
        primitive->y2 = primitive->y3 = y + texture->h / (index + 1);
        primitive->u0 = primitive->u2 = texture->u;
        primitive->u1 = primitive->u3 = texture->u + (unsigned char)texture->w;
        primitive->v1 = texture->v + (index < 2 ? texture->h / (index + 2) : 0);
        primitive->v0 = primitive->v1 = primitive->v1;
        primitive->v2 = primitive->v3 = texture->v + texture->h / (index + 1);
        primitive->clut = texture->clut;
        primitive->tpage = texture->tpage;
        primitive->r0 = primitive->g0 = primitive->b0 = 0x80;
        ((PrimitiveTag *)primitive)->address = ((PrimitiveTag *)ordering_table)->address;
        ((PrimitiveTag *)ordering_table)->address = (unsigned long)primitive;
    }
    return packet;
}

// FUNCTION: SCUS_943.58:0x80013C70
int func_80013C70(void)
{
    int current;
    int flags;
    int count;
    int index;
    char *buffer;
    char *allocation;

    current = D_80045368;
    flags = D_800502E8[current].flags;
    if ((flags & 1) == 0) {
        if (flags & 2) {
            func_8001EC78(shared_read_requests[current].destination);
            DrawSync(0);
            current = D_80045368;
            if (D_800502E8[current].flags & 0x100) {
                func_8001F378(shared_read_requests[current].destination);
            }
            D_800502E8[D_80045368].flags = 0;
        }
        current = D_80045368;
        if (D_800502E8[current].flags & 4) {
            func_8001EEE0(shared_read_requests[current].destination);
            DrawSync(0);
            current = D_80045368;
            if (D_800502E8[current].flags & 0x100) {
                func_8001F378(shared_read_requests[current].destination);
            }
            D_800502E8[D_80045368].flags = 0;
        }
        index = (D_80045368 + 1) % 20;
        if (D_800502E8[index].flags == 0) {
            return 0;
        }
        buffer = shared_read_requests[index].destination;
        D_80045368 = index;
        if (buffer == 0) {
            if (D_800502E8[index].flags & 0x106) {
                int slot;
                allocation = func_8001F28C(disc_file_records[D_800502E8[index].file_index].size);
                slot = D_80045368;
                shared_read_requests[slot].cursor = allocation;
                shared_read_requests[slot].destination = allocation;
            } else {
                int slot;
                allocation = func_8001F1A0(disc_file_records[D_800502E8[index].file_index].size);
                slot = D_80045368;
                shared_read_requests[slot].cursor = allocation;
                shared_read_requests[slot].destination = allocation;
            }
            if (shared_read_requests[D_80045368].destination == 0) {
                return -2;
            }
        }
        current = D_80045368;
        if (D_800502E8[current].completion != 0) {
            *D_800502E8[current].completion = shared_read_requests[current].destination;
        }
    } else {
        index = VSync(-1) - D_80045350;
        if (D_80045354 == 0 || index < 181) {
            goto count_requests;
        }
    }
    func_80013FB8(D_80045368);
count_requests:
    index = 0;
    count = 0;
    do {
        if (D_800502E8[count].flags != 0) {
            index++;
        }
        count++;
    } while (count < 20);
    return index;
}
// FUNCTION: SCUS_943.58:0x800139C4
void func_800139C4(unsigned char *buffer)
{
    int index, next, block, end;
    unsigned char *front, *back;
    unsigned char first;
    block = 0;
    end = 0x1000;
    do {
        index = 0;
        do {
            front = buffer + (block * 0x1000 + index);
            next = index + 1;
            back = buffer + (end - next);
            first = *front;
            *front = *back;
            *back = first;
            index = next;
        } while (index < 0x800);
        block++;
        end += 0x1000;
    } while (block < 2);
}

// FUNCTION: SCUS_943.58:0x800142F0
void func_800142F0(int mode)
{
    func_80013C24();
    VSync(3);
    switch (mode) {
    case 3:
        func_80015508();
        break;
    case 7:
        func_800143E0(0, &D_80058090);
        break;
    case 6:
    default:
        func_80014388();
        break;
    }
}

// FUNCTION: SCUS_943.58:0x80014388
void func_80014388(void)
{
    int pending;

    for (;;) {
        pending = func_80013C70() - 1;
        func_8001B320(0, 0);
        if (pending < 0) {
            break;
        }
        rand();
        VSync(0);
    }
}

// FUNCTION: SCUS_943.58:0x8001507C
void func_8001507C(DRAWENV *draw, DISPENV *disp)
{
    SetDefDrawEnv(draw, 0, 256, 320, 240);
    SetDefDrawEnv(draw + 1, 0, 0, 320, 240);
    draw[0].b0 = 0;
    draw[0].g0 = 0;
    draw[0].r0 = 0;
    draw[1].b0 = 0;
    draw[1].g0 = 0;
    draw[1].r0 = 0;
    draw[0].isbg = 1;
    draw[1].isbg = 1;
    SetDefDispEnv(disp, 0, 0, 320, 240);
    SetDefDispEnv(disp + 1, 0, 256, 320, 240);
    disp[0].screen.h = 240;
    disp[1].screen.h = 240;
    disp[0].screen.y = 0;
    disp[1].screen.y = 0;
}

// FUNCTION: SCUS_943.58:0x8001514C
unsigned char *func_8001514C(unsigned long *ordering_table, unsigned char *cursor,
                    SpriteTexture *texture, int step, int duration, int x, int y,
                    unsigned char *glyphs, int count, int time)
{
    SpriteTexture glyph_texture;
    int elapsed = 0;
    int index;
    int layers = 6;
    int progress;
    unsigned char glyph;
    unsigned int glyph_index;
    int offset;
    int glyph_x;
    int expansion;
    int layer;
    int shade;
    POLY_FT4 *poly;

    glyph_texture = *texture;
    for (index = 0; index < count; elapsed += step, index++) {
        progress = time - elapsed;
        if (progress < 0)
            return cursor;
        glyph_x = x;
        glyph = *glyphs++;
        offset = *glyphs++;
        glyph_index = glyph;
        glyph_texture.u = texture->u + (glyph_index & 15) * texture->w;
        glyph_texture.v = texture->v + (glyph_index >> 4) * texture->h;
        progress = duration - progress;
        glyph_x += offset;
        if (progress < 0) {
            /* The legacy call passes promoted coordinates without short narrowing. */
            cursor = (unsigned char *)((SPRT *(*)(unsigned long *, SPRT *, int, int, SpriteTexture *, int))
                      func_80012950)(ordering_table, (SPRT *)cursor, glyph_x, y,
                                   &glyph_texture, 128);
            cursor = (unsigned char *)func_80012B6C(ordering_table, (DR_TPAGE *)cursor,
                                          texture->tpage);
        } else {
            for (layer = 0; layer < layers; layer++) {
                poly = (POLY_FT4 *)cursor;
                cursor += sizeof(POLY_FT4);
                ((PrimitiveTag *)poly)->length = 9;
                poly->code = 0x2c;
                expansion = texture->w * layer * progress / duration;
                poly->x0 = poly->x2 = glyph_x - (expansion + 1) / 2;
                poly->x1 = poly->x3 = poly->x0 + texture->w + expansion;
                poly->u0 = poly->u2 = glyph_texture.u;
                poly->u1 = poly->u3 = glyph_texture.u + (unsigned char)texture->w - 1;
                expansion = texture->h * layer * progress / duration;
                shade = 128 / layers + layers / 2;
                poly->y0 = poly->y1 = y - (expansion + 1) / 2;
                poly->y2 = poly->y3 = poly->y0 + texture->h + expansion;
                poly->v0 = poly->v1 = glyph_texture.v;
                poly->v2 = poly->v3 = glyph_texture.v + (unsigned char)texture->h - 1;
                poly->tpage = texture->tpage;
                poly->clut = texture->clut;
                poly->r0 = poly->g0 = poly->b0 = shade;
                poly->code |= 2;
                ((PrimitiveTag *)poly)->address = ((PrimitiveTag *)(ordering_table + 1))->address;
                ((PrimitiveTag *)(ordering_table + 1))->address = (unsigned long)poly;
            }
        }
    }
    return cursor;
}

// FUNCTION: SCUS_943.58:0x80015814
void func_80015814(void)
{
    SpuVoiceAttr voice;
    /* Unreferenced stack storage; the original local type is unknown. */
    unsigned char unused_stack[24];

    func_8003F094();
    SpuInit();
    SpuSetMute(0);
    SpuSetTransferMode(0);
    D_80058068.mask = 0x23CF;
    D_80058068.mvol.left = 0x3FFF;
    D_80058068.mvol.right = 0x3FFF;
    D_80058068.mvolmode.left = 0;
    D_80058068.mvolmode.right = 0;
    D_80058068.cd.volume.left = 0x7FFF;
    D_80058068.cd.volume.right = 0x7FFF;
    D_80058068.cd.reverb = 0;
    D_80058068.cd.mix = 1;
    D_80058068.ext.mix = 0;
    SpuSetCommonAttr(&D_80058068);
    voice.mask = 0xFF1F;
    voice.volume.left = 0x2FFF;
    voice.volume.right = 0x2FFF;
    voice.pitch = 0x1000;
    voice.r_mode = 3;
    voice.voice = 0xFFFFFF;
    voice.volmode.left = 0;
    voice.volmode.right = 0;
    voice.a_mode = 1;
    voice.s_mode = 1;
    voice.ar = 0;
    voice.dr = 0;
    voice.sr = 0;
    voice.rr = 0;
    voice.sl = 0xF;
    SpuSetVoiceAttr(&voice);
    SpuSetNoiseVoice(0, 0xFFFFFF);
    func_80015EC8(0xC0, 0xC0, 0xC0);
    D_8005AB2C = 0;
    D_80050568 = 0;
}

// FUNCTION: SCUS_943.58:0x80015B70
int func_80015B70(int sample, int volume)
{
    SpuVoiceAttr voice;
    short level;

    if (D_80050568 == 0 && sample < D_80058F90) {
        voice.mask = 0x93;
        voice.pitch = 0x5CE;
        voice.voice = 1 << D_800478CC;
        level = ((volume * D_80057FBC) >> 2) - 1;
        if (level > 0) {
            voice.volume.left = level < 0x4000 ? level : 0x3FFF;
        } else {
            voice.volume.left = 0;
        }
        voice.volume.right = voice.volume.left = voice.volume.left;
        voice.addr = D_80058F84 + D_80058F8C[sample];
        SpuSetKeyOnWithAttr(&voice);
        D_800478CC = (D_800478CC + 1) % 8;
    }
    return D_800478CC;
}
// FUNCTION: SCUS_943.58:0x80015508
void func_80015508(void)
{
    void *image;
    int pending;
    DRAWENV draw;
    DISPENV disp;
    RECT rect;
    DR_TPAGE page0;
    DR_TPAGE page1;
    SPRT sprites[2];

    image = func_8001579C();
    func_8001EFE4(image, 0, 511, 0, 256);
    DrawSync(0);
    func_8001F378(image);
    SetDefDrawEnv(&draw, 0, 0, 320, 240);
    draw.dtd = 0;
    draw.r0 = draw.g0 = draw.b0 = 0;
    draw.isbg = 1;
    draw.r0 = draw.g0 = draw.b0 = 0;
    SetDefDispEnv(&disp, 0, 0, 320, 240);
    disp.screen.h = 239;
    disp.screen.y = 0;
    VSync(0);
    PutDrawEnv(&draw);
    PutDispEnv(&disp);
    ((PrimitiveTag *)&page0)->length = 1;
    page0.code[0] = (GetGraphType() == 1 || GetGraphType() == 2) ?
                    0xe1001090 : 0xe1000490;
    DrawPrim(&page0);
    ((PrimitiveTag *)&sprites[0])->length = 4;
    sprites[0].code = 0x64;
    sprites[0].w = 256;
    sprites[0].h = 240;
    sprites[0].clut = 0x7fc0;
    sprites[0].x0 = 0;
    sprites[0].y0 = 0;
    sprites[0].u0 = sprites[0].v0 = 0;
    sprites[0].r0 = sprites[0].g0 = sprites[0].b0 = 128;
    DrawPrim(&sprites[0]);
    ((PrimitiveTag *)&page1)->length = 1;
    page1.code[0] = (GetGraphType() == 1 || GetGraphType() == 2) ?
                    0xe1001092 : 0xe1000492;
    DrawPrim(&page1);
    ((PrimitiveTag *)&sprites[1])->length = 4;
    sprites[1].code = 0x64;
    sprites[1].x0 = 256;
    sprites[1].w = 64;
    sprites[1].h = 240;
    sprites[1].clut = 0x7fc0;
    sprites[1].y0 = 0;
    sprites[1].u0 = sprites[1].v0 = 0;
    sprites[1].r0 = sprites[1].g0 = sprites[1].b0 = 128;
    DrawPrim(&sprites[1]);
    DrawSync(0);
    SetDispMask(1);
    for (;;) {
        pending = func_80013C70() - 1;
        func_8001B320(0, 0);
        if (pending < 0)
            break;
        rand();
        DrawSync(0);
        VSync(0);
    }
    D_8004534C = 1;
    rect.w = 320;
    rect.x = 0;
    rect.y = 0;
    rect.h = 240;
    MoveImage(&rect, 0, 256);
    DrawSync(0);
}
// FUNCTION: SCUS_943.58:0x8001579C
void *func_8001579C(void)
{
    int selection;

    selection = rand() % 11;
    return func_8001EA54(D_80010200, 0, selection * 38, 38);
}
// FUNCTION: SCUS_943.58:0x80015948
void func_80015948(unsigned char *first_data, int first_bank,
                   unsigned char *second_data, int second_bank)
{
    unsigned long addr;
    SpuVoiceAttr voice;

    SpuSetKey(0, 0xFFFFFF);
    func_8003F094();
    SpuStart();
    voice.mask = 0xFF1C;
    voice.pitch = 0x1000;
    voice.a_mode = 1;
    voice.s_mode = 1;
    voice.r_mode = 3;
    voice.voice = 0xFFFFFF;
    voice.volmode.left = 0;
    voice.volmode.right = 0;
    voice.ar = 0;
    voice.dr = 0;
    voice.sr = 0;
    voice.rr = 0;
    voice.sl = 0xF;
    SpuSetVoiceAttr(&voice);
    addr = 0x1010;
    if (first_bank >= 0) {
        D_80058F80 = first_bank;
        D_80058F84 = addr;
        D_80058F88 = D_800478D8[first_bank].offsets[D_800478D8[first_bank].sample_count];
        D_80058F8C = D_800478D8[first_bank].offsets;
        D_80058F90 = D_800478D8[first_bank].sample_count;
        SpuSetTransferStartAddr(0x1010);
        SpuRead(first_data, D_80058F88);
        VSync(0);
        SpuIsTransferCompleted(1);
        addr = ((D_80058F88 + 0xF) & ~0xFUL) + 0x1010;
    }
    if (second_bank >= 0) {
        D_80058F94 = second_bank;
        D_80058F98 = addr;
        D_80058F9C = D_800478D8[second_bank].offsets[D_800478D8[second_bank].sample_count];
        D_80058FA0 = D_800478D8[second_bank].offsets;
        D_80058FA4 = D_800478D8[second_bank].sample_count;
        D_80057FC0 = D_800478D8[second_bank].pitch;
        SpuSetTransferStartAddr(addr);
        SpuRead(second_data, D_80058F9C);
        VSync(0);
        SpuIsTransferCompleted(1);
    }
    D_8005AB2C = 0;
    D_80050568 = 0;
}

// FUNCTION: SCUS_943.58:0x80015C8C
void func_80015C8C(int pitch, unsigned long voices, int sample, int volume)
{
    SpuVoiceAttr voice;
    short level;

    if (D_80050568 == 0 && sample < D_80058F90) {
        voice.voice = voices;
        voice.mask = 0x93;
        if (pitch != 0) {
            voice.pitch = pitch;
        } else {
            voice.pitch = D_80057FC0;
        }
        if (voices == 0x800000) {
            level = ((volume * D_80057FBE) >> 2) - 1;
        } else {
            level = ((volume * D_80057FBC) >> 2) - 1;
        }
        if (level > 0) {
            voice.volume.left = level < 0x4000 ? level : 0x3FFF;
        } else {
            voice.volume.left = 0;
        }
        voice.volume.right = voice.volume.left = voice.volume.left;
        voice.addr = D_80058F84 + D_80058F8C[sample];
        SpuSetKeyOnWithAttr(&voice);
    }
}

// FUNCTION: SCUS_943.58:0x80015D84
void func_80015D84(unsigned long voices, int volume)
{
    SpuVoiceAttr voice;
    short level;

    voice.mask = 3;
    voice.voice = voices;
    if (voices == 0x800000) {
        level = ((volume * D_80057FBE) >> 2) - 1;
    } else {
        level = ((volume * D_80057FBC) >> 2) - 1;
    }
    if (level > 0) {
        voice.volume.left = level < 0x4000 ? level : 0x3FFF;
    } else {
        voice.volume.left = 0;
    }
    voice.volume.right = voice.volume.left = voice.volume.left;
    SpuSetVoiceAttr(&voice);
}

// FUNCTION: SCUS_943.58:0x80015E18
void func_80015E18(unsigned long voices, int sample)
{
    SpuVoiceAttr voice;
    short level;

    if (D_80050568 == 0) {
        voice.voice = voices;
        voice.mask = 0x93;
        voice.pitch = D_80057FC0;
        level = D_80057FBA * 0x20 - 1;
        if (level > 0) {
            voice.volume.left = level < 0x4000 ? level : 0x3FFF;
        } else {
            voice.volume.left = 0;
        }
        voice.volume.right = voice.volume.left = voice.volume.left;
        voice.addr = D_80058F98 + D_80058FA0[sample];
        SpuSetKeyOnWithAttr(&voice);
    }
}

// FUNCTION: SCUS_943.58:0x80015EC8
void func_80015EC8(int volume, int effect_volume, int cd_volume)
{
    CdlATV mix;
    short music = volume;
    short effects = effect_volume;
    short cd = cd_volume;

    if ((unsigned short)volume < 0x100) {
        D_80057FBC = music;
    }
    if ((unsigned short)effect_volume < 0x100) {
        D_80057FBA = effects;
    }
    if ((unsigned short)cd_volume < 0x100) {
        D_80057FB8 = D_80057FBE = cd;
    }
    mix.val3 = 0;
    mix.val1 = 0;
    mix.val0 = mix.val2 = D_80057FB8 / 2;
    CdMix(&mix);
}

// FUNCTION: SCUS_943.58:0x80015F68
int func_80015F68(void)
{
    int result;

    if (D_800478D0 == 0) {
        D_800478D0 = 1;
        result = 0;
    } else {
        result = -1;
    }
    return result;
}

// FUNCTION: SCUS_943.58:0x80015F98
int func_80015F98(void)
{
    int result;

    if (D_800478D0 != 0) {
        D_800478D0 = 0;
        result = 0;
    } else {
        result = -1;
    }
    return result;
}

// FUNCTION: SCUS_943.58:0x80015FC8
void func_80015FC8(void)
{
    unsigned char mode[8];

    SpuSetKey(0, 0xFFFFFF);
    mode[0] = 0xA0;
    CdControl(0x0E, mode, 0);
    D_80050568 = 0;
}

// FUNCTION: SCUS_943.58:0x80016010
void func_80016010(int first_bank, int second_bank, unsigned char mode)
{
    SoundDiscTransfer *request;
    SoundDiscTransfer *active;
    unsigned long second_addr;
    int bank;
    int file_index;

    request = func_8001F1A0(0x81C);
    D_80058F80 = first_bank;
    D_80058F84 = 0x1010;
    D_80058F88 = D_800478D8[first_bank].offsets[D_800478D8[first_bank].sample_count];
    D_80058F8C = D_800478D8[first_bank].offsets;
    sound_bank_sample_count = D_800478D8[first_bank].sample_count;
    file_index = D_800478D4[first_bank].file_index;
    D_80047CF4 = request;
    /* Copy the aligned CdlLOC representation: direct SDK struct assignment
       emits unaligned load/store pairs with this compiler. */
    *(unsigned long *)&request->positions[0] = *(unsigned long *)&disc_file_records[file_index].pos;
    request->sizes[0] = disc_file_records[D_800478D4[first_bank].file_index].size;
    if (second_bank >= 0) {
        second_addr = ((D_80058F88 + 0x800) & ~0x7FFUL) + 0x1010;
        D_80058F94 = second_bank;
        D_80058F98 = second_addr;
        D_80058F9C = D_800478D8[second_bank].offsets[D_800478D8[second_bank].sample_count];
        D_80058FA0 = D_800478D8[second_bank].offsets;
        D_80058FA4 = D_800478D8[second_bank].sample_count;
        sound_bank_pitch = D_800478D8[second_bank].pitch;
        *(unsigned long *)&request->positions[1] = *(unsigned long *)&disc_file_records[D_800478D4[second_bank].file_index].pos;
        request->sizes[1] = disc_file_records[D_800478D4[second_bank].file_index].size;
        bank = 1;
    } else {
        bank = 0;
    }
    active = D_80047CF4;
    active->bank_index = bank;
    active->voice = mode;
    func_80016278();
}

// FUNCTION: SCUS_943.58:0x80016220
int func_80016220(void)
{
    if (D_80050568 == 0 && D_80047CF4 != 0) {
        func_8001F378(D_80047CF4);
        D_80047CF4 = 0;
    }
    return (short)sound_transfer_status;
}

// FUNCTION: SCUS_943.58:0x80016278
void func_80016278(void)
{
    int sector;
    unsigned long size;
    SoundDiscTransfer *transfer;
    SoundDiscTransfer *current;

    sector = CdPosToInt(&sound_disc_transfer->positions[sound_disc_transfer->bank_index]);
    transfer = sound_disc_transfer;
    transfer->sector = sector - 1;
    size = transfer->sizes[transfer->bank_index];
    transfer->transfer_pending = 0;
    current = sound_disc_transfer;
    transfer->remaining = (size + 0x7FFU) >> 11;
    sound_transfer_status = 2;
    SpuSetTransferStartAddr(sound_transfer_banks[current->bank_index].transfer_start);
    CdReadyCallback(func_80016354);
    CdControl(6, (unsigned char *)&sound_disc_transfer->positions[sound_disc_transfer->bank_index], 0);
}

// FUNCTION: SCUS_943.58:0x800166A4
void func_800166A4(void)
{
    unsigned char *slot;
    int remaining;
    unsigned char empty;

    empty = 0xFF;
    remaining = 63;
    slot = &D_80058CC8[63];
    D_80058B10 = 0;
    D_8005A7B8 = 0;
    do {
        *slot = empty;
        remaining--;
        slot--;
    } while (remaining >= 0);
}

// FUNCTION: SCUS_943.58:0x800166DC
int func_800166DC(unsigned char command)
{
    if (D_80058CC8[D_8005A7B8] != 0xFF) {
        return -1;
    }
    D_80058CC8[D_8005A7B8] = command;
    D_8005A7B8 = (D_8005A7B8 + 1) % 64;
    return 0;
}

// FUNCTION: SCUS_943.58:0x80016748
void func_80016748(void)
{
    if (D_80058CC8[D_80058B10] != 0xFF &&
        SpuGetKeyStatus(0x20000) != 1) {
        func_80016A24(0x20000, D_80058CC8[D_80058B10]);
        D_80058CC8[D_80058B10] = 0xFF;
        D_80058B10 = (D_80058B10 + 1) % 64;
    }
}

// FUNCTION: SCUS_943.58:0x80016804
void func_80016804(void)
{
    int index;

    index = 0;
    D_8005A7AC = 0;
    D_80058B0C = 0;
    do {
        sound_commands[index].command = 0xFF;
        sound_commands[index].sample_index = 0xFF;
        sound_commands[index].volume = 0;
        index++;
    } while (index < 16);
    D_8005AB2C = 0;
}

// FUNCTION: SCUS_943.58:0x80016860
int func_80016860(int sample_index, unsigned char volume)
{
    int index;

    if (sound_transfer_status != 0) {
        return -1;
    }
    if (sample_index >= sound_transfer_banks[0].sample_count) {
        return -1;
    }
    index = D_8005A7AC;
    if (sound_commands[index].command != 0xFF) {
        return -1;
    }
    sound_commands[index].sample_index = sample_index;
    sound_commands[index].command = 0;
    sound_commands[D_8005A7AC].volume = volume;
    D_8005A7AC = (D_8005A7AC + 1) % 16;
    return 0;
}

// FUNCTION: SCUS_943.58:0x80016A24
int func_80016A24(unsigned long voice_bit, int sample_index)
{
    int index;

    if (sound_transfer_status != 0) {
        return -1;
    }
    if (sample_index >= sound_transfer_banks[1].sample_count) {
        return -1;
    }
    index = D_8005A7AC;
    if (sound_commands[index].command != 0xFF) {
        return -1;
    }
    sound_commands[index].sample_index = sample_index;
    sound_commands[index].command = voice_bit;
    sound_commands[D_8005A7AC].volume = 0x80;
    D_8005A7AC = (D_8005A7AC + 1) % 16;
    return 0;
}

// FUNCTION: SCUS_943.58:0x80016AEC
void func_80016AEC(void)
{
    int index;
    int next_voice;
    unsigned long command;
    unsigned long voices;
    int level;
    SpuVoiceAttr voice;

    voices = 0;
    index = D_80058B0C;
    next_voice = D_800478CC;
    while (index != D_8005A7AC) {
        if (sound_commands[index].command != 0xFF) {
            if (sound_commands[index].command >= 16) {
                /* The reference explicitly rereads the queued voice mask. */
                voices |= pending_sound_commands[index].command;
                D_8005AB2C |= sound_commands[index].command;
            } else {
                switch (sound_commands[index].command) {
                case 0:
                    next_voice = (next_voice + 1) % 8;
                    voices |= 1 << next_voice;
                    D_8005AB2C |= 1 << next_voice;
                    break;
                case 1:
                    voices |= 0x8000;
                    D_8005AB2C |= 0x8000;
                    break;
                }
            }
        }
        index = (index + 1) % 16;
    }
    SpuSetKey(0, D_8005AB2C);
    while (D_80058B0C != D_8005A7AC) {
        command = sound_commands[D_80058B0C].command;
        if (command != 0xFF) {
            if (command >= 16) {
                voice.voice = command;
                voice.mask = 0x93;
                voice.pitch = D_80057FC0;
                level = D_80057FBA * 0x20 - 1;
                if (level > 0) {
                    if (level < 0x4000) {
                        voice.volume.left = level;
                    } else {
                        voice.volume.left = 0x3FFF;
                    }
                } else {
                    voice.volume.left = 0;
                }
                voice.volume.right = voice.volume.left = voice.volume.left;
                voice.addr = sound_transfer_banks[1].transfer_start +
                    sound_transfer_banks[1].offsets[sound_commands[D_80058B0C].sample_index];
                SpuSetVoiceAttr(&voice);
            } else {
                switch (command) {
                case 0:
                    D_800478CC = (D_800478CC + 1) % 8;
                    voice.voice = 1 << D_800478CC;
                    voice.mask = 0x93;
                    voice.pitch = 0x5CE;
                    level = ((sound_commands[D_80058B0C].volume * D_80057FBC) >> 2) - 1;
                    if (level > 0) {
                        if (level < 0x4000) {
                            voice.volume.left = level;
                        } else {
                            voice.volume.left = 0x3FFF;
                        }
                    } else {
                        voice.volume.left = 0;
                    }
                    voice.volume.right = voice.volume.left = voice.volume.left;
                    voice.addr = sound_transfer_banks[0].transfer_start +
                        sound_transfer_banks[0].offsets[sound_commands[D_80058B0C].sample_index];
                    SpuSetVoiceAttr(&voice);
                    break;
                case 1:
                    voice.voice = 0x8000;
                    voice.mask = 0x93;
                    voice.pitch = sound_commands[D_80058B0C].pitch;
                    level = ((sound_commands[D_80058B0C].volume * D_80057FBC) >> 2) - 1;
                    if (level > 0) {
                        if (level < 0x4000) {
                            voice.volume.left = level;
                        } else {
                            voice.volume.left = 0x3FFF;
                        }
                    } else {
                        voice.volume.left = 0;
                    }
                    voice.volume.right = voice.volume.left = voice.volume.left;
                    voice.addr = sound_transfer_banks[0].transfer_start +
                        sound_transfer_banks[0].offsets[sound_commands[D_80058B0C].sample_index];
                    SpuSetVoiceAttr(&voice);
                    break;
                }
            }
            voices |= voice.voice;
            sound_commands[D_80058B0C].command = 0xFF;
        }
        D_80058B0C = (D_80058B0C + 1) % 16;
    }
    SpuSetKey(1, voices);
    D_8005AB2C = 0;
}

// FUNCTION: SCUS_943.58:0x80016EE4
void func_80016EE4(void)
{
    char *filename = D_80010214;
    int index;
    int track;
    int current_track;
    CdlLOC *position;
    CdlFILE file;

    cd_audio_current_pos.sector = 0;
    cd_audio_current_pos.second = 0;
    cd_audio_current_minute = 0;
    cd_audio_current_pos.track = 0;
    cd_audio_pending_pos.sector = 0;
    cd_audio_pending_pos.second = 0;
    cd_audio_pending_minute = 0;
    cd_audio_pending_pos.track = 0;
    for (index = 1; index < 26; index++) {
        cd_audio_starts[index].track = 255;
    }
    position = &cd_audio_starts[2];
    D_80058048 = 1;
    D_80047D0C = 0;
    for (track = 2; track < 26; track++) {
        filename[9] = track + 63;
        current_track = track;
        if (CdSearchFile(&file, filename) == 0) {
            return;
        }
        D_80058048++;
        *position = file.pos;
        cd_audio_starts[track].track = current_track;
        CdIntToPos(CdPosToInt(&file.pos) + ((file.size + 2047) >> 11),
                   &cd_audio_ends[track]);
        position++;
    }
}

// FUNCTION: SCUS_943.58:0x80017044
void func_80017044(int track)
{
    CdControlF(0x16, &cd_audio_starts[track].minute);
}

// FUNCTION: SCUS_943.58:0x80017074
void func_80017074(int track, int mode)
{
    int index;
    int first;
    int second;
    int temporary;
    short *source;
    int *destination;
    unsigned char parameter[8];

    if (track <= D_80058048) {
        CdReadyCallback(func_80017470);
        parameter[0] = 4;
        CdControl(14, parameter, 0);
        D_80050684 = 0;
        if (track == 0) {
            mode = 5;
            index = 0;
            destination = D_80050640;
            source = D_80047CF8;
            do {
                *destination++ = *source++;
                index++;
            } while (index < 10);
            index = 0;
            do {
                index++;
                first = rand() % 10;
                second = rand() % 10;
                temporary = D_80050640[first];
                D_80050640[first] = D_80050640[second];
                D_80050640[second] = temporary;
            } while (index < 100);
            track = D_80050640[0];
        }
        D_80050678 = cd_audio_ends[track];
        D_80050678.track = track;
        D_8005067C = CdPosToInt(&D_80050678);
        if (D_80047D0C != 0) {
            D_8005067C -= 75;
        }
        D_80050680 = mode;
        D_80050690 = cd_audio_starts[track];
        D_80050694 = mode;
        CdControlF(3, &cd_audio_starts[track].minute);
        func_80015EC8(-1, -1, -1);
    }
}

// FUNCTION: SCUS_943.58:0x8001728C
void func_8001728C(void)
{
    CdControlF(9, 0);
    D_80050680 = 0;
}

// FUNCTION: SCUS_943.58:0x800172B8
void func_800172B8(void)
{
    int status;
    unsigned char result[8];

    status = CdControl(1, 0, result);
    if (status == 1 && (result[0] & 0x80)) {
        status = CdControl(0x11, 0, result);
        if (status == 1) {
            D_80050690.minute = result[5];
            D_80050690.second = result[6];
            D_80050690.sector = result[7];
            D_80050690.track = result[0];
        } else {
            D_80050690 = cd_audio_starts[D_80050678.track];
        }
        CdControlF(9, 0);
    }
    if (D_80050680 != 6) {
        D_80050694 = D_80050680;
        D_80050680 = 6;
    }
}

// FUNCTION: SCUS_943.58:0x800173C0
void func_800173C0(void)
{
    unsigned char result[8];

    if (D_80050680 == 6) {
        CdControl(1, 0, result);
        if ((result[0] & 0x80) == 0) {
            D_80050680 = D_80050694;
            CdControlF(3, &D_80050690.minute);
        }
    }
}

// FUNCTION: SCUS_943.58:0x8001742C
void func_8001742C(int fade_step)
{
    if (D_80050680 - 1U < 2) {
        D_8005068C = D_80057FB8;
        D_80050688 = fade_step;
        D_80050680 = 3;
    }
}

// FUNCTION: SCUS_943.58:0x8001787C
void func_8001787C(void)
{
    func_80044524(1);
    func_80044578();
    func_8003CA7C();
}

// FUNCTION: SCUS_943.58:0x800178AC
void func_800178AC(void)
{
    func_800445B0();
}

// FUNCTION: SCUS_943.58:0x80017A00
void func_80017A00(long *events)
{
    int i;

    func_8003CB2C();
    for (i = 0; i < 8; i++) {
        CloseEvent(events[i]);
    }
    func_8003CB3C();
}

// FUNCTION: SCUS_943.58:0x80017A58
int func_80017A58(long *events)
{
    int index;
    long *event;

    do {
        index = 0;
        event = events;
        do {
            if (TestEvent(*event) != 0) {
                return index;
            }
            index++;
            event++;
        } while (index < 4);
    } while (1);
}

// FUNCTION: SCUS_943.58:0x80017AC0
int func_80017AC0(long *events)
{
    int index;
    long *event;

    index = 0;
    event = events;
    do {
        if (TestEvent(*event) != 0) {
            return index;
        }
        index++;
        event++;
    } while (index < 4);
    return -1;
}

// FUNCTION: SCUS_943.58:0x800178CC
void func_800178CC(long *events)
{
    int i;

    func_8003CB2C();
    events[0] = OpenEvent(0xF4000001, 4, 0x2000, 0);
    events[1] = OpenEvent(0xF4000001, 0x8000, 0x2000, 0);
    events[2] = OpenEvent(0xF4000001, 0x100, 0x2000, 0);
    events[3] = OpenEvent(0xF4000001, 0x2000, 0x2000, 0);
    events[4] = OpenEvent(0xF0000011, 4, 0x2000, 0);
    events[5] = OpenEvent(0xF0000011, 0x8000, 0x2000, 0);
    events[6] = OpenEvent(0xF0000011, 0x100, 0x2000, 0);
    events[7] = OpenEvent(0xF0000011, 0x2000, 0x2000, 0);
    func_8003CB3C();
    for (i = 0; i < 8; i++) {
        EnableEvent(events[i]);
    }
}

// FUNCTION: SCUS_943.58:0x80017B18
void func_80017B18(long *events)
{
    long event;
    int i;
    long *cursor;

    i = 0;
    cursor = events;
    do {
        event = *cursor;
        cursor++;
        i++;
        TestEvent(event);
    } while (i < 4);
}

// FUNCTION: SCUS_943.58:0x80017B64
int func_80017B64(long *events)
{
    CardResultTable results;
    long event;
    int i;
    long *cursor;
    int index;
    int retries;
    long *flush_cursor;
    int clear_index;
    long *clear_cursor;
    int poll_index;
    long *poll_cursor;

    results = card_result_table;
    i = 0;
    cursor = events;
    do {
        event = *cursor;
        cursor++;
        i++;
        TestEvent(event);
    } while (i < 4);
    retries = 0;
    do {
        _card_info(0);
        do {
            poll_index = 0;
            poll_cursor = events;
            do {
                if (TestEvent(*poll_cursor) != 0) {
                    index = poll_index;
                    goto info_event;
                }
                poll_index++;
                poll_cursor++;
            } while (poll_index < 4);
        } while (1);
info_event:
        if (index == 0) {
            break;
        }
        clear_index = 0;
        if (index == 3) {
            goto clear_flush;
        }
        retries++;
    } while (retries < 16);
    if (index == 3) {
        clear_index = 0;
clear_flush:
        flush_cursor = events + 4;
        do {
            TestEvent(*flush_cursor++);
            clear_index++;
        } while (clear_index < 4);
        retries = 0;
        events += 4;
        do {
            _card_clear(0);
            do {
                clear_index = 0;
                clear_cursor = events;
                do {
                    if (TestEvent(*clear_cursor) != 0) {
                        goto clear_event;
                    }
                    clear_index++;
                    clear_cursor++;
                } while (clear_index < 4);
            } while (1);
clear_event:
            if (clear_index == 0) {
                break;
            }
            retries++;
        } while (retries < 16);
        index = 3;
        if (clear_index != 0) {
            index = 4;
        }
    }
    return results.entries[index];
}

// FUNCTION: SCUS_943.58:0x80017CFC
int func_80017CFC(long *events)
{
    CardLoadResultTable results;
    long event;
    int i;
    long *cursor;
    int index;
    int retries;
    int result_offset;
    long status;

    results = card_load_result_table;
    i = 0;
    cursor = events;
    do {
        event = *cursor;
        cursor++;
        i++;
        TestEvent(event);
    } while (i < 4);
    retries = 0;
    do {
        _card_load(0);
        do {
            index = 0;
            do {
                status = TestEvent(events[index]);
                result_offset = index * sizeof(int);
                if (status != 0) {
                    goto load_event;
                }
                index++;
            } while (index < 4);
        } while (1);
load_event:
        if (index == 0) {
            break;
        }
        retries++;
    } while (retries < 16);
    return *(int *)((char *)&results + result_offset);
}


// FUNCTION: SCUS_943.58:0x80018044
int func_80018044(unsigned char *destination, int mode, int frame_count,
                  unsigned char *palette, unsigned char *frames, char *title)
{
    int i;
    int size;
    int result;
    unsigned char character;
    char *title_cursor;

    result = frame_count + 1;
    size = result * 128;
    for (i = 0; i < size; i++) {
        destination[i] = 0;
    }
    destination[0] = 0x53;
    destination[1] = 0x43;
    destination[2] = frame_count % 4 + 0x10;
    destination[3] = mode;
    title_cursor = title;
    for (i = 0; i < 64; i += 2) {
        character = *title_cursor;
        if (character == 0) {
            break;
        }
        (destination + i)[4] = character;
        (destination + i)[5] = title_cursor[1];
        title_cursor += 2;
    }
    for (i = 0; i < 32; i++) {
        (destination + i)[0x60] = *palette;
        palette++;
    }
    for (i = 0; i < frame_count; i++, destination += 128) {
        mode = 0;
        size = i * 128;
        palette = destination + 128;
        do {
            *palette = frames[size + mode];
            mode++;
            palette++;
        } while (mode < 128);
    }
    return result;
}


// FUNCTION: SCUS_943.58:0x800181D4
unsigned int func_800181D4(unsigned char *data, unsigned int length)
{
    unsigned int value;
    unsigned int left_shift;
    unsigned int right_shift;
    unsigned int index;
    unsigned int checksum;
    /* Preserve the original unused eight-byte stack allocation. */
    unsigned char stack_space[8];

    checksum = 0x6B383658;
    index = 0;
    if (length != 0) {
        do {
            value = *data;
            data++;
            left_shift = index & 0x1F;
            right_shift = 32 - index;
            index++;
            checksum ^= (value << left_shift) |
                        (value << (right_shift & 0x1F));
        } while (index < length);
    }
    return checksum;
}

// FUNCTION: SCUS_943.58:0x80018230
int func_80018230(void)
{
    D_80047D10 = 1;
    D_80047D14 = 0;
    return func_80018808();
}

// FUNCTION: SCUS_943.58:0x80018264
void func_80018264(void)
{
    D_80047D10 = 0;
    D_80047D14 = 0;
}


// FUNCTION: SCUS_943.58:0x80018604
TILE *func_80018604(unsigned long *ordering_table, TILE *tile)
{
    ((PrimitiveTag *)tile)->length = 3;
    tile->code = 0x60;
    tile->w = 320;
    tile->r0 = 0;
    tile->g0 = 0;
    tile->b0 = 0;
    tile->x0 = 0;
    tile->y0 = 0;
    tile->h = 240;
    ((PrimitiveTag *)tile)->address = ((PrimitiveTag *)ordering_table)->address;
    ((PrimitiveTag *)ordering_table)->address = (unsigned long)tile;
    return tile + 1;
}

// FUNCTION: SCUS_943.58:0x80017DE8
int func_80017DE8(void)
{
    long events[8];
    int result;

    card_format_open_events(events);
    result = card_format_load_result(events);
    card_format_close_events(events);
    if (result == 1) {
        result = format(card_format_device) - 1;
    } else {
        result = -1;
    }
    return result;
}

// FUNCTION: SCUS_943.58:0x8001827C
void func_8001827C(void)
{
    DRAWENV draw;
    DISPENV disp;
    unsigned char packets[4096];
    RECT texture = card_message_texture;
    CardMessageColors colors = card_message_colors;
    unsigned long ordering_table[2];
    unsigned char *packet;
    int result;
    int frames;
    unsigned int previous;
    unsigned int buttons;

    if (D_80047D10 == 0 || D_80047D14 == 0)
        return;
    SetDefDrawEnv(&draw, 0, 0, 320, 240);
    draw.dtd = 0;
    draw.r0 = draw.g0 = draw.b0 = 0;
    draw.isbg = 1;
    draw.r0 = draw.g0 = draw.b0 = 0;
    SetDefDispEnv(&disp, 0, 0, 320, 240);
    PutDrawEnv(&draw);
    PutDispEnv(&disp);
    ClearOTag(ordering_table, 1);
    packet = packets;
    packet = func_80012C30(ordering_table, packet, &texture, 0x733E,
                           176, 192, card_saving_text, &colors.normal, 0);
    func_80018604(ordering_table, (TILE *)packet);
    DrawOTag(ordering_table);
    DrawSync(0);
    SetDispMask(1);
    result = func_80018808();
    ClearOTag(ordering_table, 1);
    packet = packets;
    if (result < 0) {
        switch (result) {
        case -1:
            packet = func_80012C30(ordering_table, packet, &texture, 0x733E,
                                   100, 114, card_missing_text, &colors.error, 0);
            break;
        case -2:
        case -3:
        case -7:
            packet = func_80012C30(ordering_table, packet, &texture, 0x733E,
                                   32, 114, card_changed_text, &colors.error, 0);
            break;
        default:
            packet = func_80012C30(ordering_table, packet, &texture, 0x733E,
                                   112, 114, card_write_error_text, &colors.error, 0);
            break;
        }
        packet = func_80012C30(ordering_table, packet, &texture, 0x733E,
                               108, 138, card_not_saved_text, &colors.error, 0);
        packet = func_80012C30(ordering_table, packet, &texture, 0x733E,
                               104, 162, card_save_off_text, &colors.error, 0);
        frames = 180;
    } else {
        packet = func_80012C30(ordering_table, packet, &texture, 0x733E,
                               160, 192, card_saved_text, &colors.normal, 0);
        frames = 120;
    }
    func_80018604(ordering_table, (TILE *)packet);
    DrawOTag(ordering_table);
    DrawSync(0);
    SetDispMask(1);
    buttons = ~0U;
    while (--frames > 0) {
        previous = buttons;
        buttons = func_8001B320(0, 0);
        if (buttons != (previous & buttons))
            break;
        VSync(0);
    }
}

// FUNCTION: SCUS_943.58:0x80018678
int func_80018678(void)
{
    unsigned char data[128];
    long events[8];
    int result;
    int status;
    int fd;

    result = 0;
    func_800178CC(events);
    status = func_80017B64(events);
    if (status >= 0) {
        status = func_80017CFC(events);
        if (status == 0) {
            fd = open(card_save_filename, 1);
            if (fd >= 0) {
                lseek(fd, 0x200, 0);
                status = read(fd, data, 128);
                if (status == 128) {
                    status = func_80018768(data);
                    result = -4;
                    if (status >= 0) {
                        result = status & 1;
                    }
                } else {
                    result = -6;
                }
                close(fd);
            }
        } else {
            result = -6;
            if (status == 1) {
                result = -2;
            }
        }
    } else {
        result = -1;
    }
    func_80017A00(events);
    return result;
}

// FUNCTION: SCUS_943.58:0x80018768
int func_80018768(unsigned char *data)
{
    int result = 0;
    unsigned char *expected = card_save_signature;
    CardSaveHeader *header = (CardSaveHeader *)data;
    unsigned char *cursor = data;

    do {
        if (*cursor != *expected++) {
            return -1;
        }
        cursor++;
    } while ((int)expected < (int)(card_save_signature + 7));
    if (func_800181D4((unsigned char *)&header->data_offset, 16) != header->checksum) {
        return -2;
    }
    if (header->data_offset != 0) {
        result = 1;
    }
    return result;
}

// FUNCTION: SCUS_943.58:0x80018808
int func_80018808(void)
{
    CardSaveBlock block;
    long events[8];
    int result = 0;
    int status;
    int fd;

    if (D_80047D10 == 0) {
        return 0;
    }
    D_80047D10 = 0;
    func_800178CC(events);
    status = func_80017B64(events);
    if (status == 0) {
        status = func_80017CFC(events);
        if (status == 0) {
            fd = func_80018968(card_save_filename, &block);
            if (fd >= 0) {
                block.header.data_offset = 28;
                func_80018CC0(block.payload);
                ((CardSaveHeaderTail *)block.header.unknown_10)->data_checksum =
                    func_800181D4(block.payload, 0x998);
                block.header.checksum = func_800181D4(
                    (unsigned char *)&block.header.data_offset, 16);
                lseek(fd, 0, 0);
                write(fd, &block, 0x2000);
                if (func_80017A58(events) > 0) {
                    result = -6;
                } else {
                    D_80047D10 = 1;
                    D_80047D14 = 0;
                }
                close(fd);
            } else {
                result = -3;
            }
        } else {
            result = -6;
            if (status == 1) {
                result = -2;
            }
        }
    } else {
        result = -1;
        if (status == 1) {
            result = -7;
        }
    }
    func_80017A00(events);
    return result;
}

// FUNCTION: SCUS_943.58:0x80018968
int func_80018968(char *filename, CardSaveBlock *block)
{
    CardSaveTitle title;
    CardSavePalette palette;
    CardSaveFrames frames;
    int fd;
    CardSaveHeader *header;
    int initialize = 0;
    unsigned char *signature = card_save_signature;
    unsigned char *expected;
    unsigned char *output;
    int i;

    title = card_save_title;
    palette = card_save_palette;
    frames = card_save_frames;
    fd = open(filename, 1);
    header = &block->header;
    if (fd >= 0) {
        read(fd, block, 0x2000);
        close(fd);
        for (i = 0; i < 8; i++) {
            if (header->signature[i] != signature[i]) {
                initialize = 1;
            }
        }
    } else {
        fd = open(filename, 0x10200);
        if (fd >= 0) {
            close(fd);
            initialize = 1;
        }
    }
    if (initialize) {
        if (fd < 0) {
            return fd;
        }
        for (i = 0x1fff; i >= 0; i--) {
            ((unsigned char *)block)[i] = 0;
        }
        func_80018044(block->card_header, 1, 3, palette.bytes, frames.bytes, title.text);
        output = header->signature;
        expected = signature;
        do {
            *output = *expected;
            output++;
            expected++;
        } while ((int)output < (int)(header->signature + 8));
        header->data_offset = 0;
        ((CardSaveHeaderTail *)header->unknown_10)->unknown_10 = 0;
        ((CardSaveHeaderTail *)header->unknown_10)->data_checksum = 0;
        ((CardSaveHeaderTail *)header->unknown_10)->unknown_18 = 0;
    }
    if (fd >= 0) {
        fd = open(filename, 0x8002);
    }
    return fd;
}

// FUNCTION: SCUS_943.58:0x80018CC0
void func_80018CC0(unsigned char *data)
{
    CardGamePayload *save = (CardGamePayload *)data;
    int i;

    save->size = 0x998;
    save->settings = card_game_settings;
    save->audio = card_audio_settings;
    save->options = card_game_options;
    for (i = 0; i < 10; i++) {
        /* Preserve the shared base used to address the resident record table. */
        save->records[i] = ((CardGameRecord *)((unsigned char *)&card_game_settings + 0x570))[i];
    }
    for (i = 0; i < 3; i++) {
        save->results[i] = card_game_results[i];
    }
    save->progress = card_game_progress;
    for (i = 0; i < 3; i++) {
        save->entries[i] = card_game_entries[i];
    }
}

// FUNCTION: SCUS_943.58:0x80018F44
int func_80018F44(void)
{
    CardLoadBlock block;
    long events[8];
    int result = 0;
    int status;
    int fd;

    func_800178CC(events);
    status = func_80017B64(events);
    if (status >= 0) {
        status = func_80017CFC(events);
        if (status == 0) {
            fd = open(card_load_filename, 1);
            if (fd >= 0) {
                lseek(fd, 0x200, 0);
                if (read(fd, &block, 0xA80) == 0xA80) {
                    status = func_80019058(&block.header);
                    if (status >= 0 && (status & 1) != 0) {
                        result = func_80019154(&block.header, &block.payload) == 0;
                    } else {
                        result = -4;
                    }
                } else {
                    result = -6;
                }
                close(fd);
            }
        } else {
            result = -6;
            if (status == 1) {
                result = -2;
            }
        }
    } else {
        result = -1;
    }
    func_80017A00(events);
    D_80047D10 = result > 0;
    return result;
}

// FUNCTION: SCUS_943.58:0x80019058
int func_80019058(CardSaveHeader *input)
{
    int result = 0;
    unsigned char *signature = card_load_signature;
    unsigned char *expected = signature;
    CardSaveHeader *header = input;
    CardGamePayload *payload = (CardGamePayload *)((unsigned char *)header + header->data_offset);
    int version;

    do {
        if (*(unsigned char *)input != *expected++) {
            return -1;
        }
        /* Walk the serialized signature one byte at a time. */
        input = (CardSaveHeader *)((unsigned char *)input + 1);
    } while ((int)expected < (int)(signature + 7));
    version = header->signature[7];
    if (version != signature[7]) {
        D_80045370 = version;
        if (version <= 0) {
            D_80045370 = 1;
        }
    }
    if (func_800181D4((unsigned char *)&header->data_offset, 16) != header->checksum) {
        return -2;
    }
    if (header->data_offset != 0) {
        result = 0x10000;
        if (func_800181D4((unsigned char *)payload, 0x998) ==
            ((CardSaveHeaderTail *)header->unknown_10)->data_checksum) {
            result = 1;
        }
    }
    return result;
}

// FUNCTION: SCUS_943.58:0x80019154
int func_80019154(CardSaveHeader *header, CardGamePayload *payload)
{
    int i;

    if (payload->size != 0x998) {
        return -1;
    }
    if (func_800181D4((unsigned char *)payload, 0x998) !=
        ((CardSaveHeaderTail *)header->unknown_10)->data_checksum) {
        return -2;
    }
    card_game_settings = payload->settings;
    card_audio_settings = payload->audio;
    card_game_options = payload->options;
    for (i = 0; i < 10; i++) {
        ((CardGameRecord *)((unsigned char *)&card_game_settings + 0x570))[i] = payload->records[i];
    }
    for (i = 0; i < 3; i++) {
        card_game_results[i] = payload->results[i];
    }
    card_game_progress = payload->progress;
    for (i = 0; i < 3; i++) {
        card_game_entries[i] = payload->entries[i];
    }
    return 0;
}

// FUNCTION: SCUS_943.58:0x80019428
int func_80019428(void)
{
    CardExtraLoadBlock block;
    long events[8];
    int result = 0;
    int status;
    int fd;

    func_800178CC(events);
    status = func_80017B64(events);
    if (status >= 0) {
        status = func_80017CFC(events);
        if (status == 0) {
            fd = open(card_extra_load_filename, 1);
            if (fd >= 0) {
                lseek(fd, 0x200, 0);
                if (read(fd, &block, 0xA80) == 0xA80) {
                    status = func_80019530(&block.header);
                    if (status >= 0 && (status & 2) != 0) {
                        result = func_800195F8(&block.header, &block.extra) == 0;
                    } else {
                        result = -4;
                    }
                } else {
                    result = -6;
                }
                close(fd);
            }
        } else {
            result = -6;
            if (status == 1) {
                result = -2;
            }
        }
    } else {
        result = -1;
    }
    func_80017A00(events);
    return result;
}

// FUNCTION: SCUS_943.58:0x80019530
int func_80019530(CardSaveHeader *input)
{
    int result = 0;
    unsigned char *signature = card_extra_load_signature;
    unsigned char *expected = signature;
    CardSaveHeader *header = input;
    CardExtraPayload *payload = (CardExtraPayload *)((unsigned char *)header +
        ((CardSaveHeaderTail *)header->unknown_10)->unknown_10);

    do {
        if (*(unsigned char *)input != *expected++) {
            return -1;
        }
        input = (CardSaveHeader *)((unsigned char *)input + 1);
    } while ((int)expected < (int)(signature + 8));
    if (func_800181D4((unsigned char *)&header->data_offset, 16) != header->checksum) {
        return -2;
    }
    if (((CardSaveHeaderTail *)header->unknown_10)->unknown_10 != 0) {
        result = 0x20000;
        if (func_800181D4((unsigned char *)payload, 0xC4) ==
            ((CardSaveHeaderTail *)header->unknown_10)->unknown_18) {
            result = 2;
        }
    }
    return result;
}

// FUNCTION: SCUS_943.58:0x800195F8
int func_800195F8(CardSaveHeader *header, CardExtraPayload *payload)
{
    volatile int *selected;
    int row;
    int column;
    int offset;
    int mode;
    unsigned short *destination;
    unsigned short *output;
    volatile unsigned short *source;
    volatile unsigned short *values;
    volatile unsigned short *cursor;

    if (payload->size != 0xC4) {
        return -1;
    }
    if (func_800181D4((unsigned char *)payload, 0xC4) !=
        ((CardSaveHeaderTail *)header->unknown_10)->unknown_18) {
        return -2;
    }
    selected = &card_extra_selected_record;
    *selected = payload->selected_record;
    card_extra_setting_5 = ((volatile unsigned short *)payload)[3 + 5];
    card_extra_setting_6 = ((volatile unsigned short *)payload)[3 + 6];
    card_extra_setting_0 = ((volatile unsigned short *)payload)[3 + 0];
    row = 0;
    card_extra_setting_1 = ((volatile unsigned short *)payload)[3 + 1];
    cursor = (volatile unsigned short *)payload;
    card_extra_setting_2 = cursor[3 + 2];
    offset = 0;
    card_extra_setting_3 = cursor[3 + 3];
    card_extra_setting_4 = cursor[3 + 4];
    values = cursor;
    destination = (unsigned short *)((unsigned char *)selected + 0xA0);
    do {
        column = 0;
        output = destination;
        *(unsigned short *)((unsigned char *)&card_extra_record_value + offset) = values[10];
        source = cursor + 18;
        do {
            *output = *source++;
            column++;
            output++;
        } while (column < 10);
        cursor += 10;
        destination += 84;
        offset += 168;
        row++;
        values++;
    } while (row < 8);
    mode = *(short *)&card_extra_setting_5;
    if (mode == 0) {
        mode = 9;
    } else {
        mode = 10;
    }
    card_extra_mode = mode;
    return 0;
}

// FUNCTION: SCUS_943.58:0x80019748
int func_80019748(unsigned int slot, CardGhostPayload *data, unsigned int length)
{
    long events[8];
    CardGhostFilename filename;
    CardGhostSaveBlock *block;
    unsigned char *payload;
    int result = 0;
    int status;
    int fd;

    filename = card_ghost_filename;
    if (slot >= 2) {
        return -6;
    }
    filename.text[22] = slot + '1';
    block = func_8001F1A0(0x4000);
    func_800178CC(events);
    status = func_80017B64(events);
    if (status >= 0) {
        status = func_80017CFC(events);
        if (status == 0) {
            fd = func_80019920(filename.text, block, slot, data->title_index);
            if (fd >= 0) {
                payload = block->payload;
                block->data_offset = 24;
                block->data_size = length;
                func_8001A3DC(payload, data, length);
                block->data_checksum = func_800181D4(payload, length);
                block->checksum = func_800181D4((unsigned char *)&block->data_offset, 12);
                lseek(fd, 0, 0);
                write(fd, block, 0x4000);
                func_80017A58(events);
                close(fd);
            } else {
                result = -3;
            }
        } else {
            result = -6;
            if (status == 1) {
                result = -2;
            }
        }
    } else {
        result = -1;
    }
    func_80017A00(events);
    func_8001F378(block);
    return result;
}

// FUNCTION: SCUS_943.58:0x80019920
int func_80019920(char *filename, CardGhostSaveBlock *block,
                  unsigned int slot, int title_index)
{
    CardSaveTitle title;
    CardSavePalette palette_0;
    CardGhostFrame frame_0;
    CardSavePalette palette_1;
    CardGhostFrame frame_1;
    CardSavePalette palette_2;
    CardGhostFrame frame_2;
    CardSavePalette palette_3;
    CardGhostFrame frame_3;
    CardSavePalette palette_4;
    CardGhostFrame frame_4;
    CardSavePalette palette_5;
    CardGhostFrame frame_5;
    CardSavePalette palette_6;
    CardGhostFrame frame_6;
    CardGhostAssetPointers frames;
    CardGhostAssetPointers palettes;
    CardGhostAssetPointers temporary;
    int fd;
    int available = 1;
    unsigned char *output;
    int i;
    unsigned char *expected;
    CardGhostHeader *header;

    title = card_ghost_title;
    palette_0 = card_ghost_palette_0;
    frame_0 = card_ghost_frame_0;
    palette_1 = card_ghost_palette_1;
    frame_1 = card_ghost_frame_1;
    palette_2 = card_ghost_palette_2;
    frame_2 = card_ghost_frame_2;
    palette_3 = card_ghost_palette_3;
    frame_3 = card_ghost_frame_3;
    palette_4 = card_ghost_palette_4;
    frame_4 = card_ghost_frame_4;
    palette_5 = card_ghost_palette_5;
    frame_5 = card_ghost_frame_5;
    palette_6 = card_ghost_palette_6;
    frame_6 = card_ghost_frame_6;
    palettes.entries[0] = frame_0.bytes;
    palettes.entries[1] = frame_1.bytes;
    palettes.entries[2] = frame_2.bytes;
    palettes.entries[3] = frame_3.bytes;
    palettes.entries[4] = frame_4.bytes;
    palettes.entries[5] = frame_5.bytes;
    palettes.entries[6] = frame_6.bytes;
    frames = palettes;
    temporary.entries[0] = palette_0.bytes;
    temporary.entries[1] = palette_1.bytes;
    temporary.entries[2] = palette_2.bytes;
    temporary.entries[3] = palette_3.bytes;
    temporary.entries[4] = palette_4.bytes;
    temporary.entries[5] = palette_5.bytes;
    temporary.entries[6] = palette_6.bytes;
    palettes = temporary;
    title.text[59] = slot + 'P';
    fd = open(filename, 1);
    if (fd < 0) {
        fd = open(filename, 0x20200);
        if (fd < 0) {
            goto unavailable;
        }
    }
    close(fd);
    goto opened;
unavailable:
    available = 0;
opened:
    if (available) {
        fd = open(filename, 0x8002);
    }
    for (i = 0x1FFF; i >= 0; i--) {
        ((unsigned char *)block)[i] = 0;
    }
    func_80018044((unsigned char *)block, 2, 1, palettes.entries[title_index],
                  frames.entries[title_index], title.text);
    header = (CardGhostHeader *)block->signature;
    output = header->signature;
    expected = card_ghost_signature;
    do {
        *output = *expected;
        output++;
        expected++;
    } while ((int)output < (int)(header->signature + 8));
    header->data_offset = 0;
    header->data_size = 0;
    header->data_checksum = 0;
    return fd;
}

// FUNCTION: SCUS_943.58:0x8001A3DC
void func_8001A3DC(void *destination, void *source, unsigned int length)
{
    unsigned char scratch[8];
    unsigned char *output = destination;
    unsigned char *input = source;
    unsigned int copied = 0;
    unsigned char value;

    if (length != 0) {
        do {
            value = *input;
            input++;
            copied++;
            *output = value;
            output++;
        } while (copied < length);
    }
}

// FUNCTION: SCUS_943.58:0x8001A410
unsigned short func_8001A410(unsigned int *data)
{
    int status;
    int slot;
    unsigned short result;
    long events[8];

    result = 0;

    func_800178CC(events);
    status = func_80017B64(events);
    if (status >= 0) {
        status = func_80017CFC(events);
        if (status == 0) {
            for (slot = 0; slot < 2; slot++) {
                if (func_8001A4D4(slot, data + slot * 4) == 0) {
                    result |= 1 << slot;
                }
            }
        } else {
            result = 0xFFFA;
            if (status == 1) {
                result = 0xFFFE;
            }
        }
    } else {
        result = 0xFFFF;
    }
    func_80017A00(events);
    return result & 0xFFFF;
}

// FUNCTION: SCUS_943.58:0x8001A4D4
int func_8001A4D4(int slot, unsigned int *data)
{
    CardGhostReadWindow block;
    CardGhostFilename filename;
    int result;
    int fd;

    filename = card_ghost_read_filename;
    result = -1;
    filename.text[22] = slot + '1';
    fd = open(filename.text, 1);
    if (fd >= 0) {
        lseek(fd, 0x100, 0);
        if (read(fd, &block, 0x80) == 0x80) {
            if (func_8001A608(&block.header, 0) == 0) {
                result = 0;
                *(CardGhostSummary *)data = *(CardGhostSummary *)
                    ((unsigned char *)&block + block.header.data_offset);
            }
        }
        close(fd);
    }
    return result;
}

// FUNCTION: SCUS_943.58:0x8001A6B8
int func_8001A6B8(int slot, unsigned char *destination)
{
    CardGhostFilename filename;
    long events[8];
    CardGhostHeader *block;
    int status;
    int result;
    int fd;

    filename = card_ghost_read_filename;
    block = func_8001F1A0(0x3F00);
    func_800178CC(events);
    filename.text[22] = slot + '1';
    status = func_80017B64(events);
    if (status >= 0) {
        status = func_80017CFC(events);
        if (status == 0) {
            fd = open(filename.text, 1);
            if (fd >= 0) {
                lseek(fd, 0x100, 0);
                if (read(fd, block, 0x3F00) == 0x3F00) {
                    status = card_ghost_read_validate(block);
                    if (status == 0) {
                        unsigned int i = 0;
                        CardGhostHeader *header = block;
                        unsigned char *source = (unsigned char *)header + header->data_offset;
                        while (i < header->data_size) {
                            *destination = *source;
                            source++;
                            i++;
                            destination++;
                        }
                        result = 1;
                    } else {
                        result = -4;
                    }
                } else {
                    result = -6;
                }
                close(fd);
            } else {
                result = -6;
            }
        } else {
            result = -6;
            if (status == 1) {
                result = -2;
            }
        }
    } else {
        result = -1;
    }
    func_80017A00(events);
    return result;
}

// FUNCTION: SCUS_943.58:0x8001A900
int func_8001A900(unsigned int slot, short selected_record,
                  unsigned short *values, unsigned char *data)
{
    long events[8];
    CardGhostFilename filename;
    CardBoardSaveBlock *block;
    int result;
    int status;
    int fd;

    filename = card_board_filename;
    if (slot >= 7) {
        return -6;
    }
    filename.text[22] = slot + '1';
    block = func_8001F1A0(0x4000);
    func_800178CC(events);
    status = func_80017B64(events);
    if (status >= 0) {
        status = func_80017CFC(events);
        if (status == 0) {
            fd = func_8001AAD0(filename.text, block, slot);
            if (fd >= 0) {
                block->record.selected_record = selected_record;
                func_8001ADC8(&block->record, values, data);
                block->record.checksum = func_800181D4(
                    (unsigned char *)block->record.values, 0x2200);
                lseek(fd, 0, 0);
                write(fd, block, 0x4000);
                result = -(unsigned int)(func_80017A58(events) > 0) & -6;
                close(fd);
            } else {
                result = -3;
            }
        } else {
            result = -6;
            if (status == 1) {
                result = -2;
            }
        }
    } else {
        result = -1;
    }
    func_80017A00(events);
    func_8001F378(block);
    return result;
}

// FUNCTION: SCUS_943.58:0x8001AAD0
int func_8001AAD0(char *filename, CardBoardSaveBlock *block, unsigned int slot)
{
    CardSaveTitle title;
    CardSavePalette palette;
    CardSaveFrames frames;
    int fd;
    int available = 1;
    unsigned char *output;
    unsigned char *expected;
    CardBoardRecord *record;
    unsigned char *end;
    unsigned char *clear_cursor;

    title = card_board_title;
    palette = card_board_palette;
    frames = card_board_frames;
    if ((int)slot > 0) {
        title.text[62] = 0x82;
        title.text[63] = slot + 'P';
    }
    fd = open(filename, 1);
    if (fd < 0) {
        fd = open(filename, 0x20200);
        if (fd < 0) {
            goto unavailable;
        }
    }
    close(fd);
    goto opened;
unavailable:
    available = 0;
opened:
    if (available) {
        fd = open(filename, 0x8002);
    }
    clear_cursor = (unsigned char *)block;
    end = (unsigned char *)block + 0x2280;
    do {
        *clear_cursor = 0;
        clear_cursor++;
    } while (clear_cursor < end);
    func_80018044((unsigned char *)block, 2, 1, palette.bytes, frames.bytes, title.text);
    record = &block->record;
    output = record->signature;
    expected = card_board_signature;
    do {
        *output = *expected;
        output++;
        expected++;
    } while ((int)output < (int)(record->signature + 8));
    record->selected_record = 0;
    record->checksum = 0;
    return fd;
}

// FUNCTION: SCUS_943.58:0x8001ADC8
void func_8001ADC8(CardBoardRecord *record, unsigned short *values, unsigned char *data)
{
    int i;

    for (i = 0; i < 256; i++) {
        record->values[i] = values[i];
    }
    for (i = 0; i < 8192; i++) {
        record->data[i] = data[i];
    }
}

// FUNCTION: SCUS_943.58:0x8001AE14
unsigned int func_8001AE14(short *results)
{
    int value;
    int status;
    unsigned int slot;
    int result;
    long events[8];

    result = 0;
    func_800178CC(events);
    status = func_80017B64(events);
    if (status >= 0) {
        status = func_80017CFC(events);
        if (status == 0) {
            for (slot = 0; (int)slot < 7; slot++) {
                value = (short)func_8001AEE0(slot);
                if (value >= 0) {
                    result |= 1 << slot;
                }
                results[slot] = value;
            }
        } else {
            result = -6;
            if (status == 1) {
                result = -2;
            }
        }
    } else {
        result = -1;
    }
    func_80017A00(events);
    return result;
}

// FUNCTION: SCUS_943.58:0x8001AFD0
int func_8001AFD0(int slot, unsigned short *values, unsigned char *data)
{
    long events[8];
    CardGhostFilename filename;
    CardBoardRecord *record;
    int status;
    int result;
    int fd;
    int i;

    filename = card_board_read_filename;
    record = func_8001F1A0(0x4000);
    filename.text[22] = slot + '1';
    func_800178CC(events);
    status = func_80017B64(events);
    if (status >= 0) {
        status = func_80017CFC(events);
        if (status == 0) {
            fd = open(filename.text, 1);
            if (fd >= 0) {
                lseek(fd, 0x200, 0);
                if (read(fd, record, 0x2280) == 0x2280) {
                    if (func_8001B1AC(record) == 0) {
                        CardBoardRecord *source = record;
                        result = source->selected_record;
                        for (i = 0; i < 256; i++) {
                            values[i] = source->values[i];
                        }
                        for (i = 0; i < 8192; i++) {
                            data[i] = source->data[i];
                        }
                    } else {
                        result = -4;
                    }
                } else {
                    result = -6;
                }
                close(fd);
            } else {
                result = -5;
            }
        } else {
            result = -6;
            if (status == 1) {
                result = -2;
            }
        }
    } else {
        result = -1;
    }
    func_80017A00(events);
    return result;
}

// FUNCTION: SCUS_943.58:0x8001B1AC
int func_8001B1AC(CardBoardRecord *record)
{
    unsigned char *expected = card_board_read_signature;
    CardBoardRecord *header = record;

    do {
        if (*(unsigned char *)record != *expected) {
            return -1;
        }
        expected++;
        record = (CardBoardRecord *)((unsigned char *)record + 1);
    } while ((int)expected < (int)(card_board_read_signature + 8));
    return -(unsigned int)(func_800181D4((unsigned char *)header->values, 0x2200) !=
                            header->checksum) & -2;
}

// FUNCTION: SCUS_943.58:0x8001B228
void func_8001B228(void)
{
    int i = 0;

    do {
        pad_send_buffers[0][i] = 0;
        pad_send_buffers[1][i] = 0;
        i++;
    } while (i < 34);
    card_game_options.values[0] = card_game_options.values[1] = 128;
    card_game_options.values[2] = card_game_options.values[6] = 24;
    card_game_options.values[3] = card_game_options.values[7] = 96;
    card_game_options.values[8] = card_game_options.values[4] = card_game_options.values[5] = 128;
    card_game_options.values[9] = 8;
    card_game_options.values[10] = 100;
    card_game_options.values[11] = 8;
    card_game_options.values[12] = 200;
    InitPAD(pad_receive_buffers[0], 34, pad_receive_buffers[1], 34);
    SendPAD(pad_send_buffers[0], 34, pad_send_buffers[1], 34);
    StartPAD();
}

// FUNCTION: SCUS_943.58:0x8001B320
unsigned int func_8001B320(int port, unsigned char *state)
{
    unsigned int buttons;
    unsigned int original;

    if (pad_receive_buffers[port][0] == 0) {
        if (state != 0)
            state[4] = pad_receive_buffers[port][1];
        if (pad_receive_buffers[port][1] == 0x41 ||
            pad_receive_buffers[port][1] == 0x53 ||
            pad_receive_buffers[port][1] == 0x73) {
            buttons = ~(pad_receive_buffers[port][2] << 8 |
                        pad_receive_buffers[port][3]) & 0xffff;
        } else {
            buttons = 0;
        }
        switch (pad_receive_buffers[port][1]) {
        case 0x53:
            original = buttons;
            if (state != 0) {
                state[0] = pad_receive_buffers[port][4];
                state[1] = pad_receive_buffers[port][5];
                state[2] = pad_receive_buffers[port][6];
                state[3] = pad_receive_buffers[port][7];
            }
            buttons &= 0xff61;
            buttons |= (original & 4) << 5;
            buttons |= (original & 10) << 1;
            buttons |= (original & 0x10) >> 1;
            original &= 0x80;
            original >>= 6;
            buttons |= original;
            break;
        case 0x73:
            if (state != 0) {
                state[0] = pad_receive_buffers[port][4];
                state[1] = pad_receive_buffers[port][5];
                state[2] = pad_receive_buffers[port][6];
                state[3] = pad_receive_buffers[port][7];
            }
            break;
        case 0x12:
            if (state != 0) {
                state[0] = pad_receive_buffers[port][4];
                state[1] = pad_receive_buffers[port][5];
                state[2] = ~pad_receive_buffers[port][2];
                state[3] = ~pad_receive_buffers[port][3];
                buttons &= ~3;
            }
            break;
        case 0x41:
        default:
            if (state != 0) {
                state[0] = 0;
                state[1] = 0;
                state[2] = 0;
                state[3] = 0;
            }
            break;
        }
    } else {
        buttons = 0xffff0000;
        if (state != 0) {
            state[0] = 0;
            state[1] = 0;
            state[2] = 0;
            state[3] = 0;
            state[4] = 0;
        }
    }
    return buttons;
}

// FUNCTION: SCUS_943.58:0x8001B5B4
int func_8001B5B4(int port, int countdown)
{
    int previous;

    previous = (&D_80047D18)[port];
    (&D_80047D18)[port] = countdown;
    return previous;
}

// FUNCTION: SCUS_943.58:0x8001B9A4
void func_8001B9A4(void)
{
    D_80050698 = -1;
    D_8005069C = -1;
    D_800506B0 = 500000;
    D_800506B4 = 500000;
    func_8003D0D4();
    func_8003CB2C();
    D_80050698 = open(sio_filename, 1);
    D_8005069C = open(sio_filename, 2);
    func_8003CB3C();
    func_8003D714(1, 3, 0x1FA400);
    func_8003D714(1, 4, 8);
    func_8003D714(4, 0, func_8001C068);
    func_8001BF08();
}

// FUNCTION: SCUS_943.58:0x8001BAFC
unsigned char func_8001BAFC(unsigned char length, unsigned char *data, unsigned char status)
{
    unsigned char result = 0;
    unsigned char *send;
    unsigned char *destination;
    unsigned char *input;
    unsigned char *source;
    unsigned char *status_byte;
    int i;

    if (D_8005AB5C >= 3)
        goto inactive;
    if (D_8005AB5C <= 0)
        goto inactive;
    {
        input = data;
        send = D_800506A0;
        do {
            if ((int)send < (int)(D_800506A0 + length))
                *send = *input;
            else
                *send = 0;
            send++;
            input++;
        } while ((int)send < (int)(D_800506A0 + 8));
        status_byte = &D_800506A7;
        *status_byte = status;
        D_800506B0 = 500000;
        D_800506B4 = 500000;
        if (D_8005AB5C == 1) {
            write(D_8005069C, status_byte - 7, 8);
            if (D_8005AB5C != -1)
                read(D_80050698, D_800506A8, 8);
        } else {
            read(D_80050698, D_800506A8, 8);
            if (D_8005AB5C != -1)
                write(D_8005069C, status_byte - 7, 8);
        }
        for (i = 0; i < length; i++)
            data[i] = D_800506A8[i];
        result = D_800506AF;
        goto done;
    }
inactive:
    {
        if (length != 0) {
            source = data;
            destination = (unsigned char *)(length + (int)data);
            do {
                *source++ = 0;
            } while ((int)source < (int)destination);
        }
    }
done:
    return result;
}

// FUNCTION: SCUS_943.58:0x8001BCB4
unsigned char func_8001BCB4(int countdown, unsigned char value)
{
    int i;
    unsigned char *cursor;
    unsigned char *status_byte;

    if (countdown < 0)
        countdown = 5000000;
    i = 7;
    cursor = &D_800506A7;
    do {
        *cursor = value;
        i--;
        cursor--;
    } while (i >= 0);
    status_byte = &D_800506A7;
    *status_byte = 0xdd;
    D_800506B0 = countdown;
    D_800506B4 = countdown;
    if (D_8005AB5C >= 3)
        goto inactive;
    if (D_8005AB5C <= 0)
        goto inactive;
    if (D_8005AB5C == 1) {
        write(D_8005069C, status_byte - 7, 8);
        if (D_8005AB5C != -1)
            read(D_80050698, D_800506A8, 8);
    } else {
        read(D_80050698, D_800506A8, 8);
        if (D_8005AB5C != -1)
            write(D_8005069C, status_byte - 7, 8);
    }
    value = D_800506A8[0];
    goto done;
inactive:
    value = 0;
done:
    return value;
}

// FUNCTION: SCUS_943.58:0x8001BF08
void func_8001BF08(void)
{
    D_8005AB5C = 0;
    func_8003D714(2, 0, 0);
    func_8003D714(1, 1, 3);
}

// FUNCTION: SCUS_943.58:0x8001BF48
int func_8001BF48(int countdown)
{
    int result = -1;
    int remaining = countdown;

    while (remaining > 0) {
        if ((func_8003D714(0, 0, 0) & 0x180) == 0x80) {
            func_8003D714(1, 1, 0);
            func_8003D714(1, 1, 0);
            func_8003D714(5, 0, 0);
            SetDispMask(1);
            VSync(0);
            D_8005AB5C = 1;
            result = 0;
            break;
        }
        remaining--;
    }
    return result;
}

// FUNCTION: SCUS_943.58:0x8001BFFC
int func_8001BFFC(void)
{
    unsigned int status;

    do {
        status = func_8003D714(0, 0, 0);
    } while ((status & 0x100) != 0);
    func_8003D714(1, 1, 0);
    func_8003D714(5, 0, 0);
    SetDispMask(1);
    VSync(0);
    D_8005AB5C = 2;
    return 0;
}
