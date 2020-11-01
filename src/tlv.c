#include "tlv.h"
#include <assert.h>

ssize_t getVNLength(const uint8_t **ptr, size_t *size) {
    uint8_t c;
    size_t offset = 0;
    const uint8_t *p = *ptr;

    if (offset >= *size)
        return -1;
    c = p[offset++];

    size_t value;
    if ((c & 0x80u) == 0x00)
        value = c;
    else {
        uint_fast8_t nonStartBytes;
        if ((c & 0xE0u) == 0xC0) { value = c & 0x1Fu; nonStartBytes = 1; } else
        if ((c & 0xF0u) == 0xE0) { value = c & 0x0Fu; nonStartBytes = 2; } else
        if ((c & 0xF8u) == 0xF0) { value = c & 0x07u; nonStartBytes = 3; } else
        if ((c & 0xFCu) == 0xF8) { value = c & 0x03u; nonStartBytes = 4; } else
        if ((c & 0xFEu) == 0xFC) { value = c & 0x01u; nonStartBytes = 5; } else
            return -1;
        while (nonStartBytes-- > 0) { // NOLINT(readability-misleading-indentation)
            if (offset >= *size)
                return -1;
            c = p[offset++];
            if ((c & 0xC0u) != 0x80)
                return -1;

            value <<= 6u;
            value += c & 0x3Fu;
        }
    }
    assert(offset <= *size); // GCOVR_EXCL_LINE
    (*ptr) += offset;
    *size -= offset;

    return value;
}

size_t putVNLength(uint8_t **ptr, size_t *size, uint32_t value) {
    size_t offset = 0;
    uint8_t *p = *ptr;
    if (value <= 127) {
        if (*size < 1)
            return 0;
        p[offset++] = value;
    } else if (value <= 2047) {
        if (*size < 2)
            return 0;
        p[offset++] = 0xC0u | (value >> 6u);
        p[offset++] = 0x80u | (value & 0x3Fu);
    } else if (value <= 65535) {
        if (*size < 3)
            return 0;
        p[offset++] = 0xE0u | (value >> 12u);
        p[offset++] = 0x80u | (value >> 6u & 0x3Fu);
        p[offset++] = 0x80u | (value & 0x3Fu);
    } else if (value <= 2097151) {
        if (*size < 4)
            return 0;
        p[offset++] = 0xF0u | (value >> 18u);
        p[offset++] = 0x80u | (value >> 12u & 0x3Fu);
        p[offset++] = 0x80u | (value >> 6u & 0x3Fu);
        p[offset++] = 0x80u | (value & 0x3Fu);
    } else if (value <= 67108863) {
        if (*size < 5)
            return 0;
        p[offset++] = 0xF8u | (value >> 24u);
        p[offset++] = 0x80u | (value >> 18u & 0x3Fu);
        p[offset++] = 0x80u | (value >> 12u & 0x3Fu);
        p[offset++] = 0x80u | (value >> 6u & 0x3Fu);
        p[offset++] = 0x80u | (value & 0x3Fu);
    } else if (value <= 2147483647) {
        if (*size < 6)
            return 0;
        p[offset++] = 0xFCu | (value >> 30u);
        p[offset++] = 0x80u | (value >> 24u & 0x3Fu);
        p[offset++] = 0x80u | (value >> 18u & 0x3Fu);
        p[offset++] = 0x80u | (value >> 12u & 0x3Fu);
        p[offset++] = 0x80u | (value >> 6u & 0x3Fu);
        p[offset++] = 0x80u | (value & 0x3Fu);
    } else {
        return 0;
    }

    (*ptr) += offset;
    *size -= offset;

    return offset;
}
