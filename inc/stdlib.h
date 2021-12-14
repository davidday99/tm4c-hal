#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <stdint.h>

void memcopy(void *dest, const void *src, uint32_t bytes);
void rmemcopy(void *dest, const void *src, uint32_t bytes);

#endif /* _STDLIB_H_ */