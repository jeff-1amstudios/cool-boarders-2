#ifndef CB2_BP_DIGITS_H
#define CB2_BP_DIGITS_H

static inline void BpDrawDigits(GsOT_TAG *tag, SPRT *sprite, int x, int y,
                                int input, int width, int length, int alignment, int absolute)
{
    short digits[16];
    int value = input;
    int places = length;
    int blank = -1;
    int padding = alignment;
    int count;
    unsigned char saved_u;

    if (absolute && value < 0) {
        value = -value;
    }
    for (count = 0; value > 0; value /= 10) {
        if (count >= places) {
            break;
        }
        digits[count++] = value % 10;
    }
    if (count == 0) {
        count = 1;
        digits[0] = 0;
    }
    while (count < places) {
        digits[count++] = blank;
    }
    sprite->x0 = x;
    sprite->y0 = y;
    for (count = places - 1; count >= 0; count--) {
        if (digits[count] >= 0) {
            saved_u = sprite->u0;
            sprite->u0 += sprite->w * digits[count];
            func_800944E8(tag, sprite);
            sprite->u0 = saved_u;
            sprite->x0 += width;
        } else if (padding >= 0) {
            sprite->x0 += width;
        }
    }
}

#endif
