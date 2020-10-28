#include "tlv.h"
#include <assert.h>

ssize_t getVNLength(const uint8_t **ptr, size_t *size) {
    size_t offset = 0;
    const uint8_t *p = *ptr;
    size_t length = 0;

    if (offset >= *size)
        return -1;
    uint8_t c = p[offset++];
    if ((c & 0x80) == 0x00)
        length = c;
    else {
        uint_fast8_t nonStartBytes;
        if ((c & 0xE0) == 0xC0) { length = c & 0x1F; nonStartBytes = 1; } else
        if ((c & 0xF0) == 0xE0) { length = c & 0x0F; nonStartBytes = 2; } else
        if ((c & 0xF8) == 0xF0) { length = c & 0x07; nonStartBytes = 3; } else
        if ((c & 0xFC) == 0xF8) { length = c & 0x03; nonStartBytes = 4; } else
        if ((c & 0xFE) == 0xFC) { length = c & 0x01; nonStartBytes = 5; } else
        { return -1; }
        while (nonStartBytes-- > 0) {
            if (offset >= *size)
                return -1;
            c = p[offset++];
            if ((c & 0xC0) != 0x80)
                return -1;

            length <<= 6;
            length += c & 0x3F;
        }
    }
    assert(offset <= *size); // GCOVR_EXCL_LINE
    (*ptr) += offset;
    *size -= offset;

    return length;
}
