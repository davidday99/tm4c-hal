#include <stdint.h>
#include "enc.h"
#include "ethernet.h"
#include "enc28j60.h"

void enc_init(ENC *enc) {
    enc->model = &ENC28J60;
    ENC28J60_init(enc->model);
}

void enc_write_frame(ENC *enc, struct enet_frame *e) {
    uint8_t *data = (uint8_t*) e;
    ENC28J60_write_frame(enc->model, data, 0);
}

void enc_read_frame(ENC *enc, struct enet_frame *e) {
    return;
}