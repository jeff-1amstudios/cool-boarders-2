/* Restore the saved state after the overlay returns. */
typedef struct {
    unsigned int words[23];
} StateRecord;

typedef struct {
    unsigned char bytes[20];
} DisplayRecord;

typedef struct {
    unsigned char bytes[8];
} RectangleRecord;

extern StateRecord D_80058BA0;
extern StateRecord D_80058E68;
extern DisplayRecord D_80058C00;
extern DisplayRecord D_80058EC8;
extern RectangleRecord D_80058F70;
extern RectangleRecord D_8005AA90;
extern unsigned short D_80058030;
extern unsigned short D_80058032;
extern unsigned short D_80058034;
extern unsigned short D_80058036;
extern unsigned short D_80058ADC;
extern unsigned short D_80058ADE;
extern unsigned short D_80058B00;
extern unsigned short D_80058B02;

void func_8001263C(void) {
    D_80058BA0 = D_80058E68;
    D_80058C00 = D_80058EC8;
    D_80058030 = D_80058ADC;
    D_80058032 = D_80058ADE;
    D_80058034 = D_80058B00;
    D_80058036 = D_80058B02;
    D_80058F70 = D_8005AA90;
}
