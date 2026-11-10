/* Boot executable reconstructions, ordered by original address. */
#include "main.h"

extern char init_filename[];
extern int D_800580AC;
extern void boot_noop(void);
extern void func_8001228C(void);
extern void func_80015814(void);
extern void func_80016EE4(void);
extern void func_8001B9A4(void);
extern void *func_800424DC(BootFile *file, const char *name);
extern int func_8001C0F4(int *state, int flag);
extern int func_800123F8(int overlay, int address);
extern void func_800142F0(int status);
extern void func_80012508(void);
extern void func_8001B59C(void);
extern int func_8005AB6C(int *state);
extern void func_8001B64C(void);
extern void func_8001263C(void);
extern void func_80044938(void);
extern short D_80058C0A;
extern short D_80058C0E;
extern void func_8003CB2C(void);
extern void func_8003CA24(int mode);
extern void func_8003BF44(void);
extern void func_8001B228(void);
extern void func_8003CBAC(int mode);
extern void func_80029328(int mode);
extern void func_8001F824(void);
extern void func_8001787C(void);
extern void func_8002997C(ScreenRect *rect, int red, int green, int blue);
extern void func_800297E8(int mode);
extern void func_8002DB58(int width, int height, int mode, int buffers, int flags);
extern void func_8002ECB0(int x, int y, int top, int bottom);
extern void func_8002ED50(void);
extern void func_8001E8C0(void);
extern void func_8003CB3C(void);
extern void func_8002974C(int mask);
extern int func_80040814(int value);
extern int func_8004082C(int value);
extern int func_800296F0(int value);
extern void func_8003F3F4(int mode, unsigned int mask);
extern void func_8003F094(void);
extern void func_8001BA78(void);
extern void func_80040734(void);
extern void func_8003CD34(void);
extern void func_800178AC(void);
extern void func_8003BD40(void);
extern void func_8003C038(void);
extern OverlayTable overlay_file_indices;
extern OverlayTable overlay_heap_starts;
extern int overlay_load_addresses[];
extern int func_80013B60(int file, int address, int offset, int flags);
extern void func_8001F150(int start, int end);
extern StateRecord D_80058BA0;
extern StateRecord D_80058E68;
extern DisplayRecord D_80058C00;
extern DisplayRecord D_80058EC8;
extern RectangleRecord D_80058F70;
extern RectangleRecord D_8005AA90;
extern unsigned short D_80058030;
extern unsigned short D_80058032;
extern unsigned short D_80058034;
extern unsigned short D_80058036;
extern unsigned short D_80058ADC;
extern unsigned short D_80058ADE;
extern unsigned short D_80058B00;
extern unsigned short D_80058B02;
extern short D_80058C04;
extern short D_80058C06;
extern unsigned char D_80058C10;
extern short D_80058BA4;
extern short D_80058BA6;
extern short D_80058BA8;
extern short D_80058BAA;
extern unsigned char D_80058BB6;
extern unsigned char D_80058BB7;
extern int D_80058E10;
extern int D_80058E18;
extern short D_80058F74;
extern short D_80058F76;
extern DrawingEnvironment *func_80027E9C(DrawingEnvironment *, int, int, int, int);
extern DisplayEnvironment *func_80027F6C(DisplayEnvironment *, int, int, int, int);
extern void *func_8001F1A0(unsigned int);
extern int func_800296D0(void);
extern int func_8001F5DC(int angle);
extern int func_8001F6A8(int angle);
extern WordPair D_800580A8[];
extern void func_8004097C(int command, void *argument);
extern int func_80013C70(void);
extern void func_8001B320(int, int);
extern int func_800449F8(void);
extern int func_8003BD60(int);
extern unsigned short D_800478D0;
extern int D_80058B10;
extern int D_8005A7B8;
extern unsigned char D_80058CC8[];
extern unsigned int D_80050570[];
extern int D_80050680;
extern int D_80050688;
extern int D_8005068C;
extern short D_80057FB8;
extern void func_80044524(int);
extern void func_80044578(void);
extern void func_8003CA7C(void);
extern void func_800445B0(void);
extern void func_8003CABC(unsigned int word);
extern int func_8003CACC(unsigned int);
extern int D_80047D1C;
extern int D_80047D18[];
extern int D_8005AB5C;
extern int func_8003D714(int, int, int);
extern unsigned char D_80058D10[];
extern int D_80058C40;
extern int D_8005AB54;
extern int D_8005802C;
extern unsigned char D_8004E050;
extern unsigned char D_8004E052;
extern unsigned int *D_80054C08;
extern short D_8005AB3C;
extern int D_8005AB24;

// FUNCTION: SCUS_943.58:0x800121B8
/* Boot initialization and dispatch through the currently loaded overlay. */

void boot_main(void) {
    int state[16];
    BootFile file;
    int status;

    boot_noop();
    status = 6;
    func_8001228C();
    func_80015814();
    func_80016EE4();
    func_8001B9A4();
    func_800424DC(&file, init_filename);
    D_800580A8[0].word0 = file.location;
    D_800580AC = file.size;
    func_8001C0F4(state, 1);
    do {
        state[1] = func_800123F8(state[0], state[1]);
        func_800142F0(status);
        func_80012508();
        func_8001B59C();
        status = func_8005AB6C(state);
        func_8001B64C();
        func_8001263C();
    } while (status != -1);
    func_80044938();
}

// FUNCTION: SCUS_943.58:0x8001228C

void func_8001228C(void) {
    ScreenRect rect;

    func_8003CB2C();
    func_8003CA24(0);
    func_8003BF44();
    func_8001B228();
    func_8003CBAC(0);
    func_80029328(0);
    func_8001F824();
    func_8001787C();
    rect.x = 0;
    rect.y = 0;
    rect.width = 1024;
    rect.height = 512;
    func_8002997C(&rect, 0, 0, 0);
    func_800297E8(0);
    func_8002DB58(320, 240, 4, 1, 0);
    D_80058C0A = 0;
    D_80058C0E = 239;
    func_8002ECB0(0, 0, 0, 240);
    func_8002ED50();
    func_8001E8C0();
    func_8003CB3C();
    func_8002974C(1);
}

// FUNCTION: SCUS_943.58:0x80012378

void func_80012378(void) {
    func_80040814(0);
    func_8004082C(0);
    func_800296F0(0);
    func_8003F3F4(0, 0xFFFFFF);
    func_8003F094();
    func_8001BA78();
    func_80029328(3);
    func_80040734();
    func_8003CD34();
    func_800178AC();
    func_8003BD40();
    func_8003C038();
}

// FUNCTION: SCUS_943.58:0x800123F8
/* Select an overlay and initialize its remaining heap space. */

int func_800123F8(int overlay, int reserved_size) {
    OverlayTable files;
    OverlayTable heap_starts;
    int end;

    files = overlay_file_indices;
    heap_starts = overlay_heap_starts;
    func_80013B60(files.entries[overlay], overlay_load_addresses[0], 0, 1);
    end = 0x801F1E00 - reserved_size;
    func_8001F150(heap_starts.entries[overlay], end);
    return end;
}

// FUNCTION: SCUS_943.58:0x80012508
/* Save the state restored after the overlay returns. */

void func_80012508(void) {
    D_80058E68 = D_80058BA0;
    D_80058EC8 = D_80058C00;
    D_80058ADC = D_80058030;
    D_80058ADE = D_80058032;
    D_80058B00 = D_80058034;
    D_80058B02 = D_80058036;
    D_8005AA90 = D_80058F70;
}

// FUNCTION: SCUS_943.58:0x8001263C
/* Restore the saved state after the overlay returns. */

void func_8001263C(void) {
    D_80058BA0 = D_80058E68;
    D_80058C00 = D_80058EC8;
    D_80058030 = D_80058ADC;
    D_80058032 = D_80058ADE;
    D_80058034 = D_80058B00;
    D_80058036 = D_80058B02;
    D_80058F70 = D_8005AA90;
}

// FUNCTION: SCUS_943.58:0x80012770
/* Set the display dimensions and reset the viewport state. */

void func_80012770(int width, int height) {
    D_80058C04 = width;
    D_80058C06 = height;
    D_80058C0A = 0;
    D_80058C0E = 239;
    D_80058C10 = height > 256;
    D_80058BA4 = width;
    D_80058BA6 = height;
    D_80058BA8 = width / 2;
    D_80058BAA = height / 2;
    D_80058BB6 = 1;
    D_80058BB7 = 0;
    D_80058E10 = width;
    D_80058E18 = height;
    D_80058F74 = width;
    D_80058F76 = height;
    D_80058032 = 0;
    D_80058030 = 0;
    D_80058034 = 0;
    if (height <= 256) {
        D_80058036 = 256;
    } else {
        D_80058036 = 0;
    }
    func_8002ED50();
}

// FUNCTION: SCUS_943.58:0x80012868
/* Initialize the two drawing/display environments and their packet buffers. */

void func_80012868(GraphicsBuffers *buffers, unsigned int packet_bytes) {
    func_80027E9C(&buffers->first.drawing, 0, 256, 320, 240);
    func_80027E9C(&buffers->second.drawing, 0, 0, 320, 240);
    buffers->first.drawing.blue = 0;
    buffers->first.drawing.green = 0;
    buffers->first.drawing.red = 0;
    buffers->second.drawing.blue = 0;
    buffers->second.drawing.green = 0;
    buffers->second.drawing.red = 0;
    buffers->first.drawing.clear_enabled = 1;
    buffers->second.drawing.clear_enabled = 1;
    func_80027F6C(&buffers->first.display, 0, 0, 320, 240);
    func_80027F6C(&buffers->second.display, 0, 256, 320, 240);
    buffers->first.display.screen_height = 239;
    buffers->second.display.screen_height = 239;
    buffers->first.display.screen_y = 0;
    buffers->second.display.screen_y = 0;
    buffers->first.packets = func_8001F1A0(packet_bytes);
    buffers->second.packets = func_8001F1A0(packet_bytes);
}

// FUNCTION: SCUS_943.58:0x80012950
/* Build a textured sprite and prepend it to the ordering table. */

SpritePacket *func_80012950(unsigned int *ordering_table, SpritePacket *packet,
                          int x, int y, SpriteTexture *texture,
                          unsigned char brightness) {
    packet->length = 4;
    packet->code = 0x64;
    packet->width = texture->width;
    packet->height = texture->height;
    packet->r = packet->g = packet->b = brightness;
    packet->x = x;
    packet->y = y;
    packet->u = texture->u;
    packet->v = texture->v;
    packet->clut = texture->clut;
    packet->address = *ordering_table & 0x00FFFFFF;
    *ordering_table = (*ordering_table & 0xFF000000) |
                      ((unsigned int)packet & 0x00FFFFFF);
    return packet + 1;
}

// FUNCTION: SCUS_943.58:0x800129E8
/* Build a colored textured sprite and prepend it to the ordering table. */

unsigned char *func_800129E8(unsigned int *ordering_table, unsigned char *buffer,
                            int x, int y, SpriteTexture *texture,
                            SpriteColor *color, int semitransparent) {
    SpritePacket *packet = (SpritePacket *)buffer;
    buffer += sizeof(SpritePacket);
    packet->length = 4;
    packet->code = 0x64;
    packet->width = texture->width;
    packet->height = texture->height;
    if (color) {
        packet->r = color->r;
        packet->g = color->g;
        packet->b = color->b;
    } else {
        packet->r = packet->g = packet->b = 0x80;
    }
    packet->x = x;
    packet->y = y;
    packet->u = texture->u;
    packet->v = texture->v;
    packet->clut = texture->clut;
    if (semitransparent) {
        packet->code |= 2;
    } else {
        packet->code &= ~2;
    }
    packet->address = *ordering_table & 0x00FFFFFF;
    *ordering_table = (*ordering_table & 0xFF000000) |
                      ((unsigned int)packet & 0x00FFFFFF);
    return buffer;
}

// FUNCTION: SCUS_943.58:0x80012AE8
/* Build a 16-by-16 textured sprite and prepend it to the ordering table. */

Sprite16Packet *func_80012AE8(unsigned int *ordering_table, Sprite16Packet *packet,
                            int x, int y, Sprite16Texture *texture,
                            unsigned char brightness) {
    packet->length = 3;
    packet->code = 0x7C;
    packet->x = x;
    packet->y = y;
    packet->r = packet->g = packet->b = brightness;
    packet->u = texture->u;
    packet->v = texture->v;
    packet->clut = texture->clut;
    packet->address = *ordering_table & 0x00FFFFFF;
    *ordering_table = (*ordering_table & 0xFF000000) |
                      ((unsigned int)packet & 0x00FFFFFF);
    return packet + 1;
}

// FUNCTION: SCUS_943.58:0x80012B6C
/* Build a draw-mode packet and prepend it to the ordering table. */

unsigned char *func_80012B6C(unsigned int *ordering_table, unsigned char *buffer,
                            unsigned int mode) {
    DrawModePacket *packet = (DrawModePacket *)buffer;
    buffer += sizeof(DrawModePacket);
    packet->length = 1;
    packet->command = 0xE1000000 |
        ((func_800296D0() == 1 || func_800296D0() == 2)
            ? mode & 0x27FF : mode & 0x09FF);
    packet->address = *ordering_table & 0x00FFFFFF;
    *ordering_table = (*ordering_table & 0xFF000000) |
                      ((unsigned int)packet & 0x00FFFFFF);
    return buffer;
}

// FUNCTION: SCUS_943.58:0x80013340
/* Build a scaled, rotated textured quad and prepend it to the ordering table. */

unsigned char *func_80013340(unsigned int *ordering_table, unsigned char *buffer,
                            RotatedQuadTexture *texture, int x, int y,
                            int angle, int scale, RotatedQuadColor *color) {
    int left, right, top, bottom;
    int sine, cosine;
    int left_cos, top_sin, right_cos, bottom_sin;
    int left_sin, top_cos, right_sin, bottom_cos;
    RotatedQuadPacket *packet;

    left = -(((short)texture->width / 2) * scale) >> 8;
    right = ((((short)texture->width + 1) / 2) * scale) >> 8;
    top = -(((short)texture->height / 2) * scale) >> 8;
    bottom = ((((short)texture->height + 1) / 2) * scale) >> 8;
    sine = func_8001F5DC(angle);
    cosine = func_8001F6A8(angle);
    packet = (RotatedQuadPacket *)buffer;
    buffer += sizeof(RotatedQuadPacket);
    packet->length = 9;
    packet->code = 0x2C;
    packet->clut = texture->clut;
    packet->tpage = texture->tpage;
    if (!color) {
        packet->r = packet->g = packet->b = 0x80;
    } else {
        packet->r = color->r;
        packet->g = color->g;
        packet->b = color->b;
        if (color->semitransparent) {
            packet->code |= 2;
        }
    }
    left_cos = left * cosine;
    top_sin = top * sine;
    right_cos = right * cosine;
    bottom_sin = bottom * sine;
    left_sin = left * sine;
    top_cos = top * cosine;
    right_sin = right * sine;
    bottom_cos = bottom * cosine;
    packet->u0 = packet->u2 = texture->u;
    packet->u1 = packet->u3 = texture->u + texture->width;
    packet->v0 = packet->v1 = texture->v;
    packet->v2 = packet->v3 = texture->v + texture->height;
    packet->x0 = x + ((left_cos - top_sin) >> 12);
    packet->x1 = x + ((right_cos - top_sin) >> 12);
    packet->x2 = x + ((left_cos - bottom_sin) >> 12);
    packet->x3 = x + ((right_cos - bottom_sin) >> 12);
    packet->y0 = y + ((left_sin + top_cos) >> 12);
    packet->y1 = y + ((right_sin + top_cos) >> 12);
    packet->y2 = y + ((left_sin + bottom_cos) >> 12);
    packet->y3 = y + ((right_sin + bottom_cos) >> 12);
    packet->address = *ordering_table & 0x00FFFFFF;
    *ordering_table = (*ordering_table & 0xFF000000) |
                      ((unsigned int)packet & 0x00FFFFFF);
    return buffer;
}

// FUNCTION: SCUS_943.58:0x80013B30
/* Initial matching reconstruction; symbol and layout types remain provisional. */

void func_80013B30(int index) {
    func_8004097C(21, &D_800580A8[index]);
}

// FUNCTION: SCUS_943.58:0x80014388
/* Signed result, preserved across a call, followed by a conditional loop. */

void func_80014388(void) {
    int pending;
    for (;;) {
        pending = func_80013C70() - 1;
        func_8001B320(0, 0);
        if (pending < 0) break;
        func_800449F8();
        func_8003BD60(0);
    }
}

// FUNCTION: SCUS_943.58:0x80015F68
/* Halfword flag access; ownership and subsystem meaning remain provisional. */

int func_80015F68(void) {
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
/* Paired halfword flag access; no atomicity or subsystem role is inferred. */

int func_80015F98(void) {
    int result;
    if (D_800478D0 != 0) {
        D_800478D0 = 0;
        result = 0;
    } else {
        result = -1;
    }
    return result;
}

// FUNCTION: SCUS_943.58:0x800166A4
/* Clear two words and fill the 64-byte slot table. */

void func_800166A4(void) {
    int i;
    unsigned char *slots;
    unsigned char value;
    D_80058B10 = 0;
    D_8005A7B8 = 0;
    value = 255;
    slots = D_80058CC8;
    for (i = 63; i >= 0; i--) {
        slots[i] = value;
    }
}

// FUNCTION: SCUS_943.58:0x80017044
/* Four-byte entry stride is observed; entry contents remain provisional. */

void func_80017044(int index) {
    func_8004097C(22, &D_80050570[index]);
}

// FUNCTION: SCUS_943.58:0x8001728C
/* Command/state interpretation remains provisional. */

void func_8001728C(void) {
    func_8004097C(9, (void *)0);
    D_80050680 = 0;
}

// FUNCTION: SCUS_943.58:0x8001742C
/* Transition states 1/2 to 3, preserving a signed halfword as a word. */
void func_8001742C(int value) {
    if (D_80050680 == 1 || D_80050680 == 2) {
        D_80050688 = value;
        D_80050680 = 3;
        D_8005068C = D_80057FB8;
    }
}

// FUNCTION: SCUS_943.58:0x8001787C

void func_8001787C(void) {
    func_80044524(1);
    func_80044578();
    func_8003CA7C();
}

// FUNCTION: SCUS_943.58:0x800178AC
/* Wrapper only; the called SDK routine remains retained assembly. */

void func_800178AC(void) {
    func_800445B0();
}

// FUNCTION: SCUS_943.58:0x80017A00
/* Eight indexed words are passed between two calls; API names are provisional. */

void func_80017A00(unsigned int *words) {
    int i;
    func_8003CB2C();
    for (i = 0; i < 8; i++) {
        func_8003CABC(words[i]);
    }
    func_8003CB3C();
}

// FUNCTION: SCUS_943.58:0x80017AC0
/* Return the index of the first nonzero callee result among four words. */
int func_80017AC0(unsigned int *words) {
    int i;
    for (i = 0; i < 4; i++) {
        if (func_8003CACC(words[i])) return i;
    }
    return -1;
}

// FUNCTION: SCUS_943.58:0x80017B18
/* Visit all four words; callee results are discarded. */
void func_80017B18(unsigned int *words) {
    int i;
    for (i = 0; i < 4; i++) func_8003CACC(words[i]);
}

// FUNCTION: SCUS_943.58:0x800181D4
/* Shift/XOR byte fold. Both shifts are left shifts, as observed. */
unsigned int func_800181D4(unsigned char *bytes, unsigned int count) {
    unsigned int result = 0x6B383658;
    unsigned int i;
    for (i = 0; i < count; i++) {
        unsigned int value = *bytes++;
        result ^= (value << (i & 31)) | (value << ((32 - i) & 31));
    }
    return result;
}

// FUNCTION: SCUS_943.58:0x8001A3DC
/* Forward byte copy; no overlap guarantee is inferred. */
void func_8001A3DC(unsigned char *destination, unsigned char *source, unsigned int count) {
    unsigned int i;
    for (i = 0; i < count; i++) *destination++ = *source++;
}

// FUNCTION: SCUS_943.58:0x8001ADC8
/* Only the observed 16-byte prefix and two payload extents are modeled. */

void func_8001ADC8(struct Payload8001ADC8 *destination, unsigned short *halfwords, unsigned char *bytes) {
    int i;
    for (i = 0; i < 256; i++) destination->halfwords[i] = *halfwords++;
    for (i = 0; i < 8192; i++) destination->bytes[i] = bytes[i];
}

// FUNCTION: SCUS_943.58:0x8001B59C

void func_8001B59C(void) {
    D_80047D18[0] = 0;
    D_80047D1C = 0;
}

// FUNCTION: SCUS_943.58:0x8001B5B4

int func_8001B5B4(int index, int value) {
    int previous = D_80047D18[index];
    D_80047D18[index] = value;
    return previous;
}

// FUNCTION: SCUS_943.58:0x8001BF08
/* State word and command meanings remain provisional. */

void func_8001BF08(void) {
    D_8005AB5C = 0;
    func_8003D714(2, 0, 0);
    func_8003D714(1, 1, 3);
}

// FUNCTION: SCUS_943.58:0x8001C0D4
/* The caller supplies storage for sixteen 32-bit words. */
void func_8001C0D4(unsigned int *words) {
    int i;
    for (i = 15; i >= 0; i--) {
        words[i] = 0;
    }
}

// FUNCTION: SCUS_943.58:0x8001C0F4
/* Clear sixteen words, then initialize the first three fields. */
int func_8001C0F4(int *words, int flag) {
    int i;
    for (i = 15; i >= 0; i--) words[i] = 0;
    words[0] = 0;
    words[1] = 0;
    if (flag) words[2] = -1;
    else words[2] = 0;
    return 3;
}

// FUNCTION: SCUS_943.58:0x8001DA98

int func_8001DA98(const SearchRecord *records, unsigned int threshold) {
    int result = -1;
    int i;
    for (i = 0; i < 3; i++, records++) {
        if (records->value > threshold) {
            result = i;
            break;
        }
    }
    return result;
}

// FUNCTION: SCUS_943.58:0x8001DAD4

int func_8001DAD4(const SearchRecord *records, unsigned int threshold) {
    int result = -1;
    int i;
    for (i = 0; i < 3; i++, records++) {
        if (records->value < threshold) {
            result = i;
            break;
        }
    }
    return result;
}

// FUNCTION: SCUS_943.58:0x8001E560
/* Copy at most 256 bytes, including a terminator if encountered. */
void func_8001E560(unsigned char *source, int value) {
    int i;
    for (i = 0; i < 256; i++) {
        D_80058D10[i] = *source;
        if (*source == 0) break;
        source++;
    }
    D_80058C40 = value;
    D_8005AB54 = 0;
}

// FUNCTION: SCUS_943.58:0x8001EC68

void func_8001EC68(int value) {
    D_8005802C = value;
}

// FUNCTION: SCUS_943.58:0x800296D0

int func_800296D0(void) {
    return D_8004E050;
}

// FUNCTION: SCUS_943.58:0x800296E0

int func_800296E0(void) {
    return D_8004E052;
}

// FUNCTION: SCUS_943.58:0x8002C3BC

int func_8002C3BC(unsigned int *data) {
    D_80054C08 = data;
    return 0;
}

// FUNCTION: SCUS_943.58:0x8002E158

int func_8002E158(void) {
    return D_8005AB3C;
}

// FUNCTION: SCUS_943.58:0x8002ECA0

void func_8002ECA0(int value) {
    D_8005AB24 = value;
}

// FUNCTION: SCUS_943.58:0x8002FCEC
/* Count arithmetic right shifts until a positive signed input reaches zero. */
int func_8002FCEC(int value) {
    int count = 0;
    while (value > 0) {
        value >>= 1;
        count++;
    }
    return count;
}

// FUNCTION: SCUS_943.58:0x8003D14C
void func_8003D14C(void) {
}

// FUNCTION: SCUS_943.58:0x8003E15C
int func_8003E15C(void) {
    return 0;
}

// FUNCTION: SCUS_943.58:0x8003E164
int func_8003E164(void) {
    return 0;
}

// FUNCTION: SCUS_943.58:0x8003E2F8
int func_8003E2F8(void) {
    return 0;
}
