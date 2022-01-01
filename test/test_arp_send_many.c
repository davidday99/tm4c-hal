#include <stdint.h>

#include "enc.h"
#include "enc28j60.h"

static ENC _enc;
static ENC *enc = &_enc;

static uint8_t arp[] = {
    0xDE,
    0xAD,
    0xBE,
    0XEF,
    0xCC,
    0XCC,
    0XAB,
    0xCD,
    0xEF,
    0x12,
    0x34,
    0x56,
    0x08,
    0x06,
    0x00,
    0x01,
    0x08,
    0x00,
    0x06,
    0x04,
    0x00,
    0x01,
    0xDE,
    0xAD,
    0xBE,
    0XEF,
    0xCC,
    0XCC,
    0xC0,
    0xA8,
    0x00,
    0x6E,
    0XAB,
    0xCD,
    0xEF,
    0x12,
    0x34,
    0x56,
    0xC0,
    0xA8,
    0x00,
    0x01,
};

static void Delay(uint32_t d) {
    uint32_t volatile delay = d * 80000;
    while (delay > 0)
        delay--;
}

int test_arp_send_many() {
    enc_init(enc);
    enc_enable(enc);

    uint8_t expected_pktcnt = 20;

    for (unsigned int i = 0; i < expected_pktcnt; i++) {
        Delay(10);
        enc_write_frame(enc, arp, sizeof(arp));
    }

    uint8_t pktcnt = ENC28J60_get_packet_count(&ENC28J60);

    return pktcnt == expected_pktcnt;
}