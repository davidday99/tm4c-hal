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

void memset(void *dest, uint8_t val, uint32_t n) {
    for (uint32_t i = 0; i < n; i++)
        ((uint8_t *) dest)[i] = val;
}

void strcpy(char *dest, char *src) {
    while ((*dest++ = *src++) != '\0')
        ;
}

int32_t strcmp(const char *s1, const char *s2) {
    for (; *s1 == *s2; s1++, s2++) {
        if (*s1 == '\0')
            return 0;
    }
    return *s1 - *s2;
}

uint32_t strlen(const char *s) {
    uint32_t len = 0;
    while (*s != '\0')
        len++;
    return len;
}