#include <stdint.h>
#include "consumer.h"
#include "enc.h"

static struct consumer_event consumer_queue[CONSUMER_EVENT_QUEUE_LEN] ;

static uint8_t consumer_rdidx;
static uint8_t consumer_wridx;

extern ENC enc;

static void handle_ethernet_waiting();

uint8_t consumer_queue_push(consumer_event_t event) {
    if (consumer_wridx == consumer_rdidx - 1)
        return 0;
    consumer_queue[consumer_wridx].eventid = event;
    consumer_wridx = (consumer_wridx + 1) % CONSUMER_EVENT_QUEUE_LEN;
    return 1;
}

consumer_event_t consumer_queue_pop(void) {
    if (consumer_rdidx == consumer_wridx)
        return CONSUMER_QUEUE_EMPTY;

    consumer_event_t e = consumer_queue[consumer_rdidx].eventid;
    consumer_rdidx = (consumer_rdidx + 1) % CONSUMER_EVENT_QUEUE_LEN;
    return e;
}

void consumer_handle_event(consumer_event_t event) {
    switch (event) {
        case CONSUMER_QUEUE_EMPTY:
            return;
        case CONSUMER_QUEUE_FULL:
            return;
        case CONSUMER_READ_ETHERNET_FRAME:
            enc_clear_interrupt_flag();
            enc_read_frame(&enc);
            break;
        case CONSUMER_ETHERNET_WAITING:
            handle_ethernet_waiting();
            break;
        default:
            break;
    }
}

static void handle_ethernet_waiting() {
    return;
}