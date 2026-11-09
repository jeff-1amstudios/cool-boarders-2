/* Only the observed 16-byte prefix and two payload extents are modeled. */
struct Payload8001ADC8 {
    unsigned char unknown[16];
    unsigned short halfwords[256];
    unsigned char bytes[8192];
};
void func_8001ADC8(struct Payload8001ADC8 *destination, unsigned short *halfwords, unsigned char *bytes) {
    int i;
    for (i = 0; i < 256; i++) destination->halfwords[i] = *halfwords++;
    for (i = 0; i < 8192; i++) destination->bytes[i] = bytes[i];
}
