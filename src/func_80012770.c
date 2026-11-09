/* Set the display dimensions and reset the viewport state. */
extern short D_80058C04;
extern short D_80058C06;
extern short D_80058C0A;
extern short D_80058C0E;
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
extern unsigned short D_80058030;
extern unsigned short D_80058032;
extern unsigned short D_80058034;
extern unsigned short D_80058036;
extern void func_8002ED50(void);

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
