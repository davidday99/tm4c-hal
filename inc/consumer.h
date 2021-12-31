#ifndef _CONSUMER_H_
#define _CONSUMER_H_

#include <stdint.h>

#define CONSUMER_EVENT_QUEUE_LEN 100

typedef uint8_t consumer_event_t;

struct consumer_event {
    consumer_event_t eventid;
};

enum CONSUMER_EVENT {
    CONSUMER_QUEUE_EMPTY,
    CONSUMER_QUEUE_FULL,
    CONSUMER_READ_ETHERNET_FRAME,
    CONSUMER_ETHERNET_WAITING,
    CONSUMER_ARP_RECEIVE,
    CONSUMER_IPV4_RECEIVE,
    CONSUMER_IPV6_RECEIVE,
    CONSUMER_TCP_RECEIVE,
    CONSUMER_UDP_RECEIVE
};

uint8_t consumer_queue_push(consumer_event_t event);
consumer_event_t consumer_queue_pop(void);
void consumer_handle_event(consumer_event_t event);

#endif /* _CONSUMER_H_ */