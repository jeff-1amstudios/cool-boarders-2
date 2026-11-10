/* Select an overlay and initialize its remaining heap space. */
typedef struct {
    int entries[14];
} OverlayTable;

extern OverlayTable overlay_file_indices;
extern OverlayTable overlay_heap_starts;
extern int overlay_load_addresses[];
extern int func_80013B60(int file, int address, int offset, int flags);
extern void func_8001F150(int start, int end);

int func_800123F8(int overlay, int reserved_size) {
    OverlayTable files;
    OverlayTable heap_starts;
    int end;

    files = overlay_file_indices;
    heap_starts = overlay_heap_starts;
    func_80013B60(files.entries[overlay], overlay_load_addresses[0], 0, 1);
    end = 0x801F1E00 - reserved_size;
    func_8001F150(heap_starts.entries[overlay], end);
    return end;
}
