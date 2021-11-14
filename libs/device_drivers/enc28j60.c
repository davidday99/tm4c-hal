#include <stdint.h>
#include "enc28j60.h"

struct ENC28J60 ENC28J60 = {
    &SSI_1,
    &PORTB_PIN0
};

void ENC28J60_init(struct ENC28J60 *enc28j60) {
    return;
}

void ENC28J60_read_data(struct ENC28J60 *enc28j60, uint8_t data[], uint32_t size) {
    return;
}

void ENC28J60_write_data(struct ENC28J60 *enc28j60, uint8_t *data, uint32_t size) {
    return;
}