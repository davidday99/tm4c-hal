#ifndef _ENC_H_
#define _ENC_H_

#include <stdint.h>

#define ENC_RX_BUF_LEN 1518
#define ENC_TX_BUF_LEN 1518

typedef struct ENC {
    void *model;
    uint8_t mac[6];
} ENC;

extern uint8_t enc_rx_buffer[ENC_RX_BUF_LEN];
extern uint8_t enc_tx_buffer[ENC_TX_BUF_LEN];

extern uint8_t enc_frame_waiting;
extern uint8_t enc_frame_ack;


uint8_t enc_init(ENC *enc);
uint8_t enc_enable(ENC *enc);
void enc_write_frame(ENC *enc, uint8_t *data, uint16_t len);
uint16_t enc_read_frame(ENC *enc);
uint32_t enc_get_mac_address_low(ENC *enc);
uint32_t enc_get_mac_address_high(ENC *enc);
void enc_clear_interrupt_flag();
void enc_acknowledge_frame();

#endif /* _ENC_H_ */