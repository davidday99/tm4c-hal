#include <stdint.h>
#include "enc.h"
#include "ethernet.h"
#include "enc28j60.h"

void enc_init(ENC *enc) {
    enc->model = &ENC28J60;
    ENC28J60_init(enc->model);
}

void enc_write_frame(struct enet_frame *e) {
    return;
}

void enc_read_frame(struct enet_frame *e) {
    return;
}