#ifndef _SSI_MODULE_H_
#define _SSI_MODULE_H_

#include "gpio.h"
#include <stdint.h>

struct SSI {
    enum SSI_MODULE module;
    struct GPIO_PIN *tx;
    struct GPIO_PIN *rx;
    struct GPIO_PIN *fss;
    struct GPIO_PIN *clk;
};

extern struct SSI SSI_0;
extern struct SSI SSI_1;
extern struct SSI SSI_2;
extern struct SSI SSI_3;

void init_ssi_mode(struct SSI *ssi, uint8_t clock_rate, uint8_t clock_phase,
                    uint8_t clock_polarity, uint8_t protocol, uint8_t data_size);
void init_ssi_clock_prescale_divider(struct SSI *ssi, uint8_t prescale_div);
void init_ssi(struct SSI *ssi);
void enable_ssi(struct SSI *ssi);
void disable_ssi(struct SSI *ssi);
void enable_ssi_interrupts(struct SSI *ssi);
void disable_ssi_interrupts(struct SSI *ssi);
void enable_ssi_tx_dma(struct SSI *ssi);
void disable_ssi_tx_dma(struct SSI *ssi);
void enable_ssi_rx_dma(struct SSI *ssi);
void disable_ssi_rx_dma(struct SSI *ssi);
void enable_ssi_tx_interrupt(struct SSI *ssi);
void disable_ssi_tx_interrupt(struct SSI *ssi);
void enable_ssi_rx_interrupt(struct SSI *ssi);
void disable_ssi_rx_interrupt(struct SSI *ssi);
void enable_ssi_rx_timeout_interrupt(struct SSI *ssi);
void disable_ssi_rx_timeout_interrupt(struct SSI *ssi);
void enable_ssi_rx_overrun_interrupt(struct SSI *ssi);
void disable_ssi_rx_overrun_interrupt(struct SSI *ssi);
void clear_ssi_rx_timeout_interrupt(struct SSI *ssi);
void clear_ssi_rx_overrun_interrupt(struct SSI *ssi);
void read_ssi(struct SSI *ssi, uint8_t *data, uint32_t size, uint8_t nop);
void write_ssi(struct SSI *ssi, uint8_t *data, uint32_t size);
void dump_rx_fifo(struct SSI *ssi);
void enable_ssi_loopback(struct SSI *ssi);
void disable_ssi_loopback(struct SSI *ssi);
uint8_t ssi_is_busy(struct SSI *ssi);
uint8_t ssi_tx_ready(struct SSI *ssi);
uint8_t ssi_rx_ready(struct SSI *ssi);
uint8_t ssi_tx_empty(struct SSI *ssi);
uint8_t ssi_rx_empty(struct SSI *ssi);

#endif /* _SSI_MODULE_H_*/