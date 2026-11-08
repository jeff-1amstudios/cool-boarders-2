#include "record_search.h"

int func_8001DA98(const SearchRecord *records, unsigned int threshold) {
    int result = -1;
    int i;
    for (i = 0; i < 3; i++, records++) {
        if (records->value > threshold) {
            result = i;
            break;
        }
    }
    return result;
}
