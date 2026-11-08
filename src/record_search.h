#ifndef CB2_RECORD_SEARCH_H
#define CB2_RECORD_SEARCH_H

/* Only the first word and twelve-byte stride are established. */
typedef struct {
    unsigned int value;
    unsigned int unknown[2];
} SearchRecord;

int func_8001DA98(const SearchRecord *records, unsigned int threshold);
int func_8001DAD4(const SearchRecord *records, unsigned int threshold);

#endif
