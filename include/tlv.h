#ifndef TLV_H
#define TLV_H

#include <sys/types.h>
#include <stdint.h>

/**
 * Reads the UTF-8-style encoded length from the specified pointer which has at max size bytes length remaining.
 * @param ptr Pointer to the pointer from which to read the UTF-8-style encoded length.
 * @param size Number of bytes available at `*ptr`.
 * @return The length decoded from `*ptr`.
 * @retval >=0 as a valid length. In this case, `**ptr` and `*size` are updated by the number of bytes consumed from `*ptr`.
 * @retval -1 if the encoding is invalid. In this case, `**ptr` and `*size` remain unmodified.
 */
extern ssize_t getVNLength(const uint8_t **ptr, size_t *size);

#endif
