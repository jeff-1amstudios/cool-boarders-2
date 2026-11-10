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
