/* Initialize the two drawing/display environments and their packet buffers. */
typedef struct {
    unsigned char settings[24];
    unsigned char clear_enabled;
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char commands[64];
} DrawingEnvironment;

typedef struct {
    short x;
    short y;
    short width;
    short height;
    short screen_x;
    short screen_y;
    short screen_width;
    short screen_height;
    unsigned char flags[4];
} DisplayEnvironment;

typedef struct {
    DrawingEnvironment drawing;
    DisplayEnvironment display;
    unsigned int unknown;
    void *packets;
    unsigned int remaining;
} GraphicsBuffer;

typedef struct {
    GraphicsBuffer first;
    GraphicsBuffer second;
} GraphicsBuffers;

extern DrawingEnvironment *func_80027E9C(DrawingEnvironment *, int, int, int, int);
extern DisplayEnvironment *func_80027F6C(DisplayEnvironment *, int, int, int, int);
extern void *func_8001F1A0(unsigned int);

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
