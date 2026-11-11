#include "main.h"

extern void boot_noop(void);
extern void func_8001228C(void);
extern void func_80015814(void);
extern void func_80016EE4(void);
extern void func_8001B9A4(void);
extern CdlFILE *CdSearchFile(CdlFILE *file, char *name);
extern int func_8001C0F4(int *state, int enabled);
extern int func_800123F8(int overlay, int size);
extern void func_800142F0(int mode);
extern void func_80012508(void);
extern void func_8001B59C(void);
extern int func_8005AB6C(int *state);
extern void func_8001B64C(void);
extern void func_8001263C(void);
extern void func_80044938(void); /* SDK exit */
extern char init_filename[];
extern unsigned int D_800580A8;
extern unsigned long D_800580AC;

extern CdlCB CdSyncCallback(CdlCB callback);
extern CdlCB CdReadyCallback(CdlCB callback);
extern unsigned long DrawSyncCallback(void (*callback)(void));
extern void SpuSetKey(long on_off, unsigned long voice_bits);
extern void func_8003F094(void);
extern void func_8001BA78(void);
extern int ResetGraph(int mode);
extern void CdFlush(void);
extern void StopPAD(void);
extern void func_800178AC(void);
extern void PadStop(void);
extern int StopCallback(void);

extern OverlayFileIndices overlay_file_indices;
extern OverlayHeapStarts overlay_heap_starts;
extern char *overlay_load_addresses[15];
extern int func_80013B60(int file_index, char *destination, int offset, int flags);
extern void func_8001F150(char *heap_start, char *heap_end);

extern DRAWENV D_80058BA0;
extern DRAWENV D_80058E68;
extern DISPENV D_80058C00;
extern DISPENV D_80058EC8;
extern RECT D_80058F70;
extern RECT D_8005AA90;
extern short D_80058030;
extern short D_80058032;
extern short D_80058034;
extern short D_80058036;
extern short D_80058ADC;
extern short D_80058ADE;
extern short D_80058B00;
extern short D_80058B02;

extern void GsInit3D(void);
extern int D_80058E10;
extern int D_80058E18;

// FUNCTION: SCUS_943.58:0x800121B8
void boot_main(void)
{
    int mode;
    int state[16];
    CdlFILE file;

    boot_noop();
    mode = 6;
    func_8001228C();
    func_80015814();
    func_80016EE4();
    func_8001B9A4();
    CdSearchFile(&file, init_filename);
    D_800580A8 = *(unsigned int *)&file.pos;
    D_800580AC = file.size;
    func_8001C0F4(state, 1);
    do {
        state[1] = func_800123F8(state[0], state[1]);
        func_800142F0(mode);
        func_80012508();
        func_8001B59C();
        mode = func_8005AB6C(state);
        func_8001B64C();
        func_8001263C();
    } while (mode != -1);
    func_80044938();
}

// FUNCTION: SCUS_943.58:0x80012378
void func_80012378(void)
{
    CdSyncCallback(0);
    CdReadyCallback(0);
    DrawSyncCallback(0);
    SpuSetKey(0, 0xFFFFFF);
    func_8003F094();
    func_8001BA78();
    ResetGraph(3);
    CdFlush();
    StopPAD();
    func_800178AC();
    PadStop();
    StopCallback();
}

// FUNCTION: SCUS_943.58:0x800123F8
int func_800123F8(int overlay, int size)
{
    OverlayFileIndices file_indices;
    OverlayHeapStarts heap_starts;
    char *heap_end;

    file_indices = overlay_file_indices;
    heap_starts = overlay_heap_starts;
    func_80013B60(file_indices.entries[overlay], overlay_load_addresses[0], 0, 1);
    heap_end = (char *)0x801F1E00 - size;
    func_8001F150(heap_starts.entries[overlay], heap_end);
    return (int)heap_end;
}

// FUNCTION: SCUS_943.58:0x80012508
void func_80012508(void)
{
    D_80058E68 = D_80058BA0;
    D_80058EC8 = D_80058C00;
    D_8005AA90 = D_80058F70;
    D_80058ADC = D_80058030;
    D_80058ADE = D_80058032;
    D_80058B00 = D_80058034;
    D_80058B02 = D_80058036;
}

// FUNCTION: SCUS_943.58:0x8001263C
void func_8001263C(void)
{
    D_80058BA0 = D_80058E68;
    D_80058C00 = D_80058EC8;
    D_80058F70 = D_8005AA90;
    D_80058030 = D_80058ADC;
    D_80058032 = D_80058ADE;
    D_80058034 = D_80058B00;
    D_80058036 = D_80058B02;
}

// FUNCTION: SCUS_943.58:0x80012770
void func_80012770(int width, int height)
{
    unsigned char interlaced;

    interlaced = height > 256;
    D_80058C00.screen.h = 239;
    D_80058BA0.ofs[0] = width / 2;
    D_80058BA0.ofs[1] = height / 2;
    D_80058C00.disp.w = width;
    D_80058C00.disp.h = height;
    D_80058C00.screen.y = 0;
    D_80058C00.isinter = interlaced;
    D_80058BA0.clip.w = width;
    D_80058BA0.clip.h = height;
    D_80058BA0.dtd = 1;
    D_80058BA0.dfe = 0;
    D_80058E10 = width;
    D_80058E18 = height;
    D_80058F70.w = width;
    D_80058F70.h = height;
    D_80058032 = 0;
    D_80058030 = 0;
    D_80058034 = 0;
    if (height <= 256) {
        D_80058036 = 256;
    } else {
        D_80058036 = 0;
    }
    GsInit3D();
}
