/* Build a textured sprite and prepend it to the ordering table. */
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
