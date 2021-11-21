#include <stdint.h>
#include "ethernet.h"

#define ENET_RX_BUFFER_SIZE 8
#define ENET_TX_BUFFER_SIZE 8

static struct enet_frame rx_frame_buffer[ENET_RX_BUFFER_SIZE];

static struct enet_frame tx_frame_buffer[ENET_TX_BUFFER_SIZE];

static uint8_t rxrdptr;
static uint8_t rxwrptr;

static uint8_t txrdptr;
static uint8_t txwrptr;


uint8_t write_rx_frame(uint16_t *data, uint16_t len) {
    if (rxrdptr > rxwrptr)  // FIFO full
        return 0;

    uint16_t i;
    for (i = 0; i < DEST_LEN; i++)
        rx_frame_buffer[rxwrptr].dest[i] = data[i];

    for (; i < DEST_LEN + SRC_LEN; i++)
        rx_frame_buffer[rxwrptr].src[i] = data[i];

    rx_frame_buffer[rxwrptr].type = data[i++];

    for (; i < len - (DEST_LEN + SRC_LEN + TYPE_LEN + FCS_LEN); i++)
        rx_frame_buffer[rxwrptr].data[i] = data[i]; 

    for (; i < len; i++)
        rx_frame_buffer[rxwrptr].fcs[i] = data[i];
        
    rxwrptr = (rxwrptr + 1) % ENET_RX_BUFFER_SIZE;
    return 1;
}

uint8_t read_rx_frame(struct enet_frame *frame) {
    if (rxrdptr == rxwrptr)  // FIFO EMPTY
        return 0;

    uint16_t i = 0;
    uint8_t *readptr = &rx_frame_buffer[rxrdptr];
    uint8_t *writeptr = &frame;
    while (i++ < sizeof(struct enet_frame))
        *writeptr++ = *readptr++;

    return 1;
}
