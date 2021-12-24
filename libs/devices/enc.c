#include <stdint.h>
#include "enc.h"
#include "ethernet.h"
#include "enc28j60.h"
#include "stdlib.h"

uint8_t enc_rx_buffer[ENC_RX_BUF_LEN];
uint8_t enc_tx_buffer[ENC_TX_BUF_LEN];

uint8_t enc_frame_waiting;
uint8_t enc_frame_len;

uint8_t enc_frame_ack = 1;

uint8_t enc_init(ENC *enc) {
    enc->model = &ENC28J60;
    uint8_t success = ENC28J60_init(enc->model);
    ENC28J60_get_mac_address(enc->model, enc->mac);
}

void enc_write_frame(ENC *enc, uint8_t *data, uint16_t len) {
    ENC28J60_write_frame(enc->model, data, len);
}

uint16_t enc_read_frame(ENC *enc) {
    uint8_t data[1518];
    uint16_t len = ENC28J60_read_frame(enc->model, enc_rx_buffer);
    enc_frame_waiting = 1;
    enc_frame_ack = 0;
    return len;
}

void enc_clear_interrupt_flag() {
    ENC28J60_decrement_packet_count(&ENC28J60);
}

void enc_acknowledge_frame() {
    enc_frame_ack = 1;
}