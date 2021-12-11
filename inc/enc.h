#ifndef _ENC_H_
#define _ENC_H_

#include <stdint.h>
#include "ethernet.h"

#define ENC_RX_BUF_LEN 1518
#define ENC_TX_BUF_LEN 1518

typedef struct ENC {
    void *model;
} ENC;

extern uint8_t enc_rx_buffer[ENC_RX_BUF_LEN];
extern uint8_t enc_tx_buffer[ENC_TX_BUF_LEN];

extern uint8_t enc_frame_waiting;

uint8_t enc_init(ENC *enc);
void enc_write_frame(ENC *enc, uint8_t *data, uint16_t len);
uint16_t enc_read_frame(ENC *enc);
uint32_t enc_get_mac_address_low(ENC *enc);
uint32_t enc_get_mac_address_high(ENC *enc);
#endif /* _ENC_H_ */