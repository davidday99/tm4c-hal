#include <stdint.h>
#include "enc.h"
#include "ethernet.h"
#include "enc28j60.h"

uint8_t enc_init(ENC *enc) {
    enc->model = &ENC28J60;
    return ENC28J60_init(enc->model);
}

void enc_write_frame(ENC *enc, struct enet_frame *e) {
    uint8_t *data = (uint8_t*) e;
    ENC28J60_write_frame(enc->model, data, e->dlen + 18);
}

void enc_read_frame(ENC *enc, struct enet_frame *e) {
    uint8_t data[1518];
    uint16_t len = ENC28J60_read_frame(enc->model, data);
    init_frame(e, data, len);
}

uint32_t enc_get_mac_address_low(ENC *enc) {
    return ENC28J60_get_mac_address_low(enc->model);
}

uint32_t enc_get_mac_address_high(ENC *enc) {
    return ENC28J60_get_mac_address_high(enc->model);
}