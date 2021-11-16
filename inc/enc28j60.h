#ifndef _ENC28J60_H_
#define _ENC28J60_H_

#include "ssi_module.h"
#include "gpio_module.h"

struct ENC28J60 {
    struct SSI *ssi;
    struct GPIO_PIN *intr;
};

extern struct ENC28J60 ENC28J60;

uint8_t ENC28J60_init(struct ENC28J60 *enc28j60);
void ENC28J60_read_data(struct ENC28J60 *enc28j60, uint8_t data[], uint32_t size);
void ENC28J60_write_data(struct ENC28J60 *enc28j60, uint8_t *data, uint32_t size);

#endif /* _ENC28J60_H_ */