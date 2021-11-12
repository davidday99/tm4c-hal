#ifndef _SSI_H_
#define _SSI_H_

#include <stdint.h>
#include "common.h"

void init_SSICR0(enum SSI_MODULE module, uint8_t scr,
                    uint8_t sph, uint8_t spo,
                    uint8_t frf, uint8_t dss);
void init_SSICR1(enum SSI_MODULE module, uint8_t mode);
void set_SSICR1SSE_high(enum SSI_MODULE module);
void set_SSICR1SSE_low(enum SSI_MODULE module);
void init_SSICC(enum SSI_MODULE module, uint8_t cs);
void init_SSICPSR(enum SSI_MODULE module, uint8_t cs);
uint16_t read_SSIDR(enum SSI_MODULE module);
void write_SSIDR(enum SSI_MODULE module, uint16_t data);
uint8_t SSI_isbusy(enum SSI_MODULE module);

#endif /* _SSI_H_ */