#ifndef _SSI_H_
#define _SSI_H_

#include <stdint.h>
#include "common.h"

void init_SSICR0(enum SSI_MODULE module, uint8_t scr,
                    uint8_t sph, uint8_t spo,
                    uint8_t frf, uint8_t dss);
void init_SSICR1(enum SSI_MODULE module, uint8_t mode);
void set_SSICR1_loopback(enum SSI_MODULE module, uint8_t lpbk);
void set_SSICR1SSE_high(enum SSI_MODULE module);
void set_SSICR1SSE_low(enum SSI_MODULE module);
void init_SSICC(enum SSI_MODULE module, uint8_t cs);
void init_SSICPSR(enum SSI_MODULE module, uint8_t cs);
uint8_t read_SSIDR(enum SSI_MODULE module);
void write_SSIDR(enum SSI_MODULE module, uint8_t data);
void read_n_bytes_from_SSIDR(enum SSI_MODULE module, uint8_t *data, uint32_t n, uint8_t nop);
void write_n_bytes_to_SSIDR(enum SSI_MODULE module, uint8_t *data, uint32_t n);
void set_SSIIM(enum SSI_MODULE module, uint8_t val);
void clear_SSIIM(enum SSI_MODULE module, uint8_t val);
void set_SSIICR(enum SSI_MODULE module, uint8_t val);
void set_SSIDMACTL_tx(enum SSI_MODULE module, uint8_t txdma_en);
void set_SSIDMACTL_rx(enum SSI_MODULE module, uint8_t rxdma_en);
void enable_SSI_NVI(enum SSI_MODULE module);
void disable_SSI_NVI(enum SSI_MODULE module);
uint8_t SSI_bsy(enum SSI_MODULE module);
uint8_t SSI_tx_full(enum SSI_MODULE module);
uint8_t SSI_tx_empty(enum SSI_MODULE module);
uint8_t SSI_rx_full(enum SSI_MODULE module);
uint8_t SSI_rx_empty(enum SSI_MODULE module);

#endif /* _SSI_H_ */