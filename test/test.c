#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tlv.h"

void badCaseTest(const uint8_t data[], size_t dataLength) {
    uint8_t *ptr = malloc(dataLength);
    assert(ptr != NULL);
    memcpy(ptr, data, dataLength);

    const uint8_t *p = ptr;
    size_t length = dataLength;
    assert(-1 == getVNLength(&p, &length));
    assert(length == dataLength);
    assert(p == ptr);

    free(ptr);
}

void testBadCases() {
#define badCase(...) \
    do { \
        const uint8_t data[] = { __VA_ARGS__ }; \
        badCaseTest(data, sizeof(data)); \
    } while (false)

    badCase(0x80);
    badCase(0xC0);
    badCase(0xC0, 0x00);
    badCase(0xC0, 0xC0);
    badCase(0xC0, 0x40);
    badCase(0xE0, 0x80);
    badCase(0xF0, 0x80, 0x80);
    badCase(0xF8, 0x80, 0x80, 0x80);
    badCase(0xFC, 0x80, 0x80, 0x80, 0x80);
    badCase(0xFE, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);

#undef badCase
}

void goodCaseBeforeEndOfBufferTest(const uint8_t data[], size_t dataLength, ssize_t expected) {
    uint8_t *ptr = malloc(dataLength);
    assert(ptr != NULL);
    memcpy(ptr, data, dataLength);

    const uint8_t *p = ptr;
    size_t length = dataLength;
    assert(expected == getVNLength(&p, &length));
    assert(0 == length);
    assert(p == ptr + dataLength);

    assert(-1 == getVNLength(&p, &length));
    assert(0 == length);
    assert(p == ptr + dataLength);

    free(ptr);
}

void testGoodCases() {
#define goodCase(expected, ...) \
    do { \
        const uint8_t data[] = { __VA_ARGS__ }; \
        goodCaseBeforeEndOfBufferTest(data, sizeof(data), expected); \
    } while (false)

    goodCase(         0, 0x00);
    goodCase(       127, 0x7F);
    goodCase(         0, 0xC0, 0x80);
    goodCase(      2047, 0xDF, 0xBF);
    goodCase(         0, 0xE0, 0x80, 0x80);
    goodCase(     65535, 0xEF, 0xBF, 0xBF);
    goodCase(         0, 0xF0, 0x80, 0x80, 0x80);
    goodCase(   2097151, 0xF7, 0xBF, 0xBF, 0xBF);
    goodCase(         0, 0xF8, 0x80, 0x80, 0x80, 0x80);
    goodCase(  67108863, 0xFB, 0xBF, 0xBF, 0xBF, 0xBF);
    goodCase(         0, 0xFC, 0x80, 0x80, 0x80, 0x80, 0x80);
    goodCase(2147483647, 0xFD, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF);

#undef goodCase
}

int main(void) {
    testGoodCases();
    testBadCases();
}
