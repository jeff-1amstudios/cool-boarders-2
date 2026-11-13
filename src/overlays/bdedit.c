#include "bdedit.h"

extern void func_80015948(unsigned char *first_data, int first_bank,
                          unsigned char *second_data, int second_bank);
extern void func_8005AC10(void);
extern unsigned int func_8001B320(int port, unsigned char *state);
extern int func_8001C130(FileLoadState *state, short mode);
extern int D_80045370;
extern int D_8004536C;

extern CVECTOR D_80062E10;
extern CVECTOR D_80062E14;
extern unsigned char *func_8005DE88(unsigned long *ot, unsigned char *packet, int selection);
extern unsigned char *func_8005C374(unsigned long *ot, unsigned char *packet,
                                  int selection, short *cursor, unsigned char **planes,
                                  CVECTOR *palette, int mode, unsigned char *mask, int value9, int flags);
extern unsigned char *func_8005D200(unsigned long *ot, unsigned char *packet,
                                  int x, int y, CVECTOR *palette, int selection);
extern unsigned char *func_8005F4AC(unsigned long *ot, unsigned char *packet,
                                  int x, int y, int w, int h,
                                  CVECTOR *border, CVECTOR *fill, int flags);

extern void func_8005C328(unsigned char **planes, int plane);
extern int D_800631FC;

// FUNCTION: BDEDIT.BIN:0x8005AB6C
int func_8005AB6C(FileLoadState *state)
{
    func_80015948((unsigned char *)state->file_5c, 0x41, 0, -1);
    func_8005AC10();
    if (D_80045370 != 0 && (func_8001B320(0, 0) & 0x401F) == 0x401F) {
        D_8004536C = rand() % 2 + 10;
    }
    func_8001C130(state, 1);
    return 3;
}

// FUNCTION: BDEDIT.BIN:0x8005BE94
unsigned char *func_8005BE94(unsigned long *ot, unsigned char *packet,
                          int selection, unsigned char **planes, CVECTOR *palette, short *cursor,
                          int mode, unsigned char *mask, int value9, int flags)
{
    CVECTOR fill = D_80062E10;
    CVECTOR border = D_80062E14;
    int item;

    packet = func_8005DE88(ot, packet, selection);
    packet = func_8005C374(ot, packet, selection, cursor, planes, palette,
                           mode, mask, value9, flags);
    if (mode != 2) {
        if (mode == 3) {
            item = (cursor[1] - 26) / 20 + 2;
        } else {
            item = 0;
        }
    } else {
        item = 1;
    }
    packet = func_8005D200(ot, packet, 16, 64, palette, item);
    return func_8005F4AC(ot, packet, 16, 201, 285, 16, &border, &fill, 0);
}

// FUNCTION: BDEDIT.BIN:0x8005C020
void func_8005C020(unsigned char **planes)
{
    int plane;
    int pixel;
    short fill;
    unsigned char **current;

    plane = 0;
    fill = 0x101;
    current = planes;
    do {
        pixel = 0;
        do {
            ((unsigned short *)*current)[pixel] = fill;
            pixel++;
        } while (pixel < 0x800);
        fill += 0x101;
        plane++;
        current++;
    } while (plane < 2);
    func_8005C328(planes, 0);
    func_8005C328(planes, 1);
    D_800631FC = 0;
}
