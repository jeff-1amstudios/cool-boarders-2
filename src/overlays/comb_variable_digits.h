#ifndef CB2_COMB_VARIABLE_DIGITS_H
#define CB2_COMB_VARIABLE_DIGITS_H

static inline void CombDrawVariableDigits(GsOT_TAG *tag, SPRT *sprite,
                                          int x, int y, int input,
                                          int width, int length)
{
    short digits[16];
    int value = input;
    int places = length;
    int blank = input;
    int padding;
    int count;
    unsigned char saved_u;

    if (value < 0) value = -value;
    if (length < 0) {
        padding = -1;
        places = 16;
        blank = -1;
    } else {
        padding = places;
    }
    for (count = 0; value > 0; value /= 10) {
        if (count >= places) break;
        digits[count++] = value % 10;
    }
    if (count == 0) {
        count = 1;
        digits[0] = 0;
    }
    while (count < places) digits[count++] = blank;
    sprite->x0 = x;
    sprite->y0 = y;
    for (count = places - 1; count >= 0; count--) {
        if (digits[count] >= 0) {
            saved_u = sprite->u0;
            sprite->u0 += sprite->w * digits[count];
            func_80096CF4(tag, sprite);
            sprite->u0 = saved_u;
            sprite->x0 += width;
        } else if (padding >= 0) {
            sprite->x0 += width;
        }
    }
}

#endif
