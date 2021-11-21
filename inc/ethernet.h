#ifndef _ETHERNET_H_
#define _ETHERNET_H_

#include <stdint.h>

#define DEST_LEN 6
#define SRC_LEN 6
#define TYPE_LEN 1
#define FCS_LEN 4

#define DEST_O 0
#define SRC_O DEST_LEN
#define TYPE_O SRC_LEN + DEST_LEN

struct enet_frame {
    uint8_t dest[6];
    uint8_t src[6];
    uint16_t type;
    uint8_t data[1500];
    uint8_t fcs[4];
} __attribute__((packed));

uint8_t write_rx_frame(uint16_t *data, uint16_t len);
uint8_t read_rx_frame(struct enet_frame *frame);

#endif /* _ETHERNET_H_ */