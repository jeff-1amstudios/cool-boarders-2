/* Boot initialization and dispatch through the currently loaded overlay. */
typedef struct {
    int location;
    int size;
    char name[16];
} BootFile;

extern char init_filename[];
extern int D_800580A8;
extern int D_800580AC;
extern void boot_noop(void);
extern void func_8001228C(void);
extern void func_80015814(void);
extern void func_80016EE4(void);
extern void func_8001B9A4(void);
extern void *func_800424DC(BootFile *file, const char *name);
extern int func_8001C0F4(int *state, int flag);
extern int func_800123F8(int overlay, int address);
extern void func_800142F0(int status);
extern void func_80012508(void);
extern void func_8001B59C(void);
extern int func_8005AB6C(int *state);
extern void func_8001B64C(void);
extern void func_8001263C(void);
extern void func_80044938(void);

void boot_main(void) {
    int state[16];
    BootFile file;
    int status;

    boot_noop();
    status = 6;
    func_8001228C();
    func_80015814();
    func_80016EE4();
    func_8001B9A4();
    func_800424DC(&file, init_filename);
    D_800580A8 = file.location;
    D_800580AC = file.size;
    func_8001C0F4(state, 1);
    do {
        state[1] = func_800123F8(state[0], state[1]);
        func_800142F0(status);
        func_80012508();
        func_8001B59C();
        status = func_8005AB6C(state);
        func_8001B64C();
        func_8001263C();
    } while (status != -1);
    func_80044938();
}
