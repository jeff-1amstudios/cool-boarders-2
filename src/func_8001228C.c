typedef struct {
    short x;
    short y;
    short width;
    short height;
} ScreenRect;

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
