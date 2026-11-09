/* Build a colored textured sprite and prepend it to the ordering table. */
typedef struct {
    unsigned int address : 24;
    unsigned int length : 8;
    unsigned char r, g, b, code;
    short x, y;
    unsigned char u, v;
    unsigned short clut;
    unsigned short width, height;
} SpritePacket;

typedef struct {
    unsigned char u, v;
    unsigned short width, height, clut;
} SpriteTexture;

typedef struct {
    unsigned char r, g, b;
} SpriteColor;

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
