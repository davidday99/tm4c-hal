#include <stdint.h>
#include "string.h"

void memcpy(void *dest, const void *src, uint32_t n) {
    for (uint32_t i = 0; i < n; i++)
        ((uint8_t *) dest)[i] = ((uint8_t *) src)[i];
}

int32_t memcmp(void *dest, const void *src, uint32_t n) {
    for (uint32_t i = 0; i < n; i++) {
        if (((uint8_t *) dest)[i] < ((uint8_t *) src)[i])
            return -1;
        else if (((uint8_t *) dest)[i] > ((uint8_t *) src)[i])
            return 1;
    }
    return 0;
}

void strcpy(char *dest, char *src) {
    while ((*dest++ = *src++) != '\0')
        ;
}