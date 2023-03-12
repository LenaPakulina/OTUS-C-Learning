#ifndef CRC32_UTILS_H
#define CRC32_UTILS_H

#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include "stddef.h"

uint32_t getCrc32(const void *buf, size_t size);

#endif // CRC32_UTILS_H
