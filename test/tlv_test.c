#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "tlv.h"
#include "tlv_test.h"

void testGetVNLengthBadCase(const uint8_t data[], size_t dataLength) {
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

void testGetVNLengthBadCases() {
#define badCase(...) \
    do { \
        const uint8_t data[] = { __VA_ARGS__ }; \
        testGetVNLengthBadCase(data, sizeof(data)); \
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

void testGetVNLengthGoodCase(const uint8_t data[], size_t dataLength, ssize_t expected) {
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

void testGetVNLengthGoodCases() {
#define goodCase(expected, ...) \
    do { \
        const uint8_t data[] = { __VA_ARGS__ }; \
        testGetVNLengthGoodCase(data, sizeof(data), expected); \
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

void testGetVNLength() {
    testGetVNLengthGoodCases();
    testGetVNLengthBadCases();
}

void testPutVNLengthGoodCase(const uint8_t *expected, size_t dataLength, uint32_t value) {
    uint8_t *ptr = malloc(dataLength);
    assert(ptr != NULL);
    uint8_t *p = ptr;
    size_t length = dataLength;
    size_t result;

    result = putVNLength(&p, &length, value);

    assert(dataLength == result);
    assert(0 == length);
    assert(0 == memcmp(expected, ptr, dataLength));
    assert(p == &ptr[dataLength]);

    result = putVNLength(&p, &length, value);
    assert(0 == result);
    assert(0 == length);
    assert(0 == memcmp(expected, ptr, dataLength));
    assert(p == &ptr[dataLength]);

    free(ptr);
}

void testPutVNLengthGoodCases() {
#define goodCase(value, ...) \
    do { \
        const uint8_t expected[] = { __VA_ARGS__ }; \
        testPutVNLengthGoodCase(expected, sizeof(expected), value); \
    } while (false)

    goodCase(         0, 0x00);
    goodCase(       127, 0x7F);
    goodCase(       128, 0xC2, 0x80);
    goodCase(      2047, 0xDF, 0xBF);
    goodCase(      2048, 0xE0, 0xA0, 0x80);
    goodCase(     65535, 0xEF, 0xBF, 0xBF);
    goodCase(     65536, 0xF0, 0x90, 0x80, 0x80);
    goodCase(   2097151, 0xF7, 0xBF, 0xBF, 0xBF);
    goodCase(   2097152, 0xF8, 0x88, 0x80, 0x80, 0x80);
    goodCase(  67108863, 0xFB, 0xBF, 0xBF, 0xBF, 0xBF);
    goodCase(  67108864, 0xFC, 0x84, 0x80, 0x80, 0x80, 0x80);
    goodCase(2147483647, 0xFD, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF);
#undef goodCase
}

void testPutVNLengthBadCase(size_t dataLength, uint32_t value) {
    const uint8_t ptr[dataLength];
    assert(ptr != NULL);
    const uint8_t *p = ptr;
    size_t length = dataLength;
    size_t result;

    result = putVNLength((uint8_t **) &p, &length, value);

    assert(0 == result);
    assert(dataLength == length);
    assert(p == &ptr[0]);
}

void testPutVNLengthBadCases() {
    testPutVNLengthBadCase(0, 0);
    testPutVNLengthBadCase(0, 127);
    testPutVNLengthBadCase(1, 128);
    testPutVNLengthBadCase(1, 2047);
    testPutVNLengthBadCase(2, 2048);
    testPutVNLengthBadCase(2, 65535);
    testPutVNLengthBadCase(3, 65536);
    testPutVNLengthBadCase(3, 2097151);
    testPutVNLengthBadCase(4, 2097152);
    testPutVNLengthBadCase(4, 67108863);
    testPutVNLengthBadCase(5, 67108864);
    testPutVNLengthBadCase(5, 2147483647);
    testPutVNLengthBadCase(6, UINT32_C(2147483648));
}

void testPutVNLength() {
    testPutVNLengthGoodCases();
    testPutVNLengthBadCases();
}

void tlv_test() {
    testGetVNLength();
    testPutVNLength();
}
