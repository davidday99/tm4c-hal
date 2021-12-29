#include <stdint.h>
#include "producer.h"
#include "consumer.h"
#include "lcd.h"

extern LCD lcd;

static struct producer_event producer_queue[PRODUCER_EVENT_QUEUE_LEN] ;

static uint8_t producer_rdidx;
static uint8_t producer_wridx;

static void handle_ethernet_receive();

uint8_t producer_queue_push(producer_event_t event) {
    if (producer_wridx == producer_rdidx - 1)
        return 0;
    producer_queue[producer_wridx].eventid = event;
    producer_wridx = (producer_wridx + 1) % PRODUCER_EVENT_QUEUE_LEN;
    return 1;
}

producer_event_t producer_queue_pop(void) {
    if (producer_rdidx == producer_wridx)
        return PRODUCER_QUEUE_EMPTY;

    producer_event_t e = producer_queue[producer_rdidx].eventid;
    producer_rdidx = (producer_rdidx + 1) % PRODUCER_EVENT_QUEUE_LEN;
    return e;
}

void producer_handle_event(producer_event_t event) {
    switch (event) {
        case PRODUCER_QUEUE_EMPTY:
            break;
        case PRODUCER_QUEUE_FULL:
            break;
        case PRODUCER_ETHERNET_RECEIVE:
            handle_ethernet_receive();
            break;
        default:
            break;
    }
}

static void handle_ethernet_receive() {
    lcd_write(&lcd, "ETHERNET RECEIVED\n");
    consumer_queue_push(CONSUMER_READ_ETHERNET_FRAME);
    return;
}