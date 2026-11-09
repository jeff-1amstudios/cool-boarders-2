/* Build a scaled, rotated textured quad and prepend it to the ordering table. */
typedef struct {
    unsigned int address : 24;
    unsigned int length : 8;
    unsigned char r, g, b, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad2;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad3;
} RotatedQuadPacket;

typedef struct {
    unsigned char u, v;
    unsigned short width, height, clut, tpage;
} RotatedQuadTexture;

typedef struct {
    unsigned char r, g, b, semitransparent;
} RotatedQuadColor;

extern int func_8001F5DC(int angle);
extern int func_8001F6A8(int angle);

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
