/* Forward byte copy; no overlap guarantee is inferred. */
void func_8001A3DC(unsigned char *destination, unsigned char *source, unsigned int count) {
    unsigned int i;
    for (i = 0; i < count; i++) *destination++ = *source++;
}
