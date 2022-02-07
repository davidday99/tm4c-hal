#ifndef _STRING_H_
#define _STRING_H_

#include <stdint.h>

void memcpy(void *dest, const void *src, uint32_t n);
int32_t memcmp(void *dest, const void *src, uint32_t n);
void memset(void *dest, uint8_t val, uint32_t n);
void strcpy(char *dest, char *src);
int32_t strcmp(const char *s1, const char *s2);
uint32_t strlen(const char *s);

#endif /* _STRING_H_ */
