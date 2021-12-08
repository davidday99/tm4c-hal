#ifndef _ENC28J60_H_
#define _ENC28J60_H_

#include "ssi_module.h"
#include "gpio_module.h"
#include "ethernet.h"

struct ENC28J60 {
    struct SSI *ssi;
    struct GPIO_PIN *cs;
};

extern struct ENC28J60 ENC28J60;

uint8_t ENC28J60_init(struct ENC28J60 *enc28j60);
uint8_t ENC28J60_enable_receive(struct ENC28J60 *enc28j60);
uint8_t ENC28J60_disable_receive(struct ENC28J60 *enc28j60);
uint16_t ENC28J60_read_frame(struct ENC28J60 *enc28j60, uint8_t *data);
void ENC28J60_write_frame(struct ENC28J60 *enc28j60, uint8_t *data, uint16_t size);
uint8_t get_packet_count(struct ENC28J60 *enc28j60);
uint32_t ENC28J60_get_mac_address_low(struct ENC28J60 *enc28j60);
uint32_t ENC28J60_get_mac_address_high(struct ENC28J60 *enc28j60);
void read_buffer_memory(struct ENC28J60 *enc28j60, uint8_t *data, uint16_t bytes);
void ENC28J60_enable_loopback_mode(struct ENC28J60 *enc28j60);

#endif /* _ENC28J60_H_ */