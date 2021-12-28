#include <stdint.h>
#include "string.h"

void memcpy(void *dest, const void *src, uint32_t n) {
    for (uint32_t i = 0; i < n; i++)
        ((uint8_t *) dest)[i] = ((uint8_t *) src)[i];
}