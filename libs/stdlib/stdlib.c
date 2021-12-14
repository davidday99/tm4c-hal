#include <stdint.h>

void memcopy(void *dest, const void *src, uint32_t bytes) {
    for (uint32_t i = 0; i < bytes; i++)
        *((uint8_t *) dest++) = *((uint8_t *) src++);
}

void rmemcopy(void *dest, const void *src, uint32_t bytes) {
    for (uint32_t i = 0; i < bytes; i++)
        *((uint8_t *) dest++) = *((uint8_t *) src--);
}