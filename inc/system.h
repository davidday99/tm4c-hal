#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>
#include "common.h"

void PLL_init(void);
void set_RCGCGPIO(enum PORT port, uint8_t value);
void set_RCGCSSI(enum SSI_MODULE module, uint8_t value);
void set_RCGCDMA(enum SSI_MODULE module, uint8_t value);
void set_NVIC_EN0_bit(uint32_t bit);
void clear_NVIC_EN0_bit(uint32_t bit);

#endif /* SYSTEM_H */