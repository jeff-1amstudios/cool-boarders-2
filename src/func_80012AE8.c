/* Build a 16-by-16 textured sprite and prepend it to the ordering table. */
typedef struct {
    unsigned int address : 24;
    unsigned int length : 8;
    unsigned char r, g, b, code;
    short x, y;
    unsigned char u, v;
    unsigned short clut;
} Sprite16Packet;

typedef struct {
    unsigned char u, v;
    unsigned short clut;
} Sprite16Texture;

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
