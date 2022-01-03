#ifndef _STRING_H_
#define _STRING_H_

#include <stdint.h>

void memcpy(void *dest, const void *src, uint32_t n);
int32_t memcmp(void *dest, const void *src, uint32_t n);
void strcpy(char *dest, char *src);

#endif /* _STRING_H_ */
