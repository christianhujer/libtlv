#include "tlv.h"
#include <assert.h>

ssize_t getVNLength(const uint8_t **ptr, size_t *size) {
    uint8_t c;
    size_t offset = 0;
    const uint8_t *p = *ptr;

    if (offset >= *size)
        return -1;
    c = p[offset++];

    size_t length;
    if ((c & 0x80u) == 0x00)
        length = c;
    else {
        uint_fast8_t nonStartBytes;
        if ((c & 0xE0u) == 0xC0) { length = c & 0x1Fu; nonStartBytes = 1; } else
        if ((c & 0xF0u) == 0xE0) { length = c & 0x0Fu; nonStartBytes = 2; } else
        if ((c & 0xF8u) == 0xF0) { length = c & 0x07u; nonStartBytes = 3; } else
        if ((c & 0xFCu) == 0xF8) { length = c & 0x03u; nonStartBytes = 4; } else
        if ((c & 0xFEu) == 0xFC) { length = c & 0x01u; nonStartBytes = 5; } else
            return -1;
        while (nonStartBytes-- > 0) { // NOLINT(readability-misleading-indentation)
            if (offset >= *size)
                return -1;
            c = p[offset++];
            if ((c & 0xC0u) != 0x80)
                return -1;

            length <<= 6u;
            length += c & 0x3Fu;
        }
    }
    assert(offset <= *size); // GCOVR_EXCL_LINE
    (*ptr) += offset;
    *size -= offset;

    return length;
}
