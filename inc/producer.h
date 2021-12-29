#ifndef _PRODUCER_H_
#define _PRODUCER_H_

#include <stdint.h>

#define PRODUCER_EVENT_QUEUE_LEN 100

typedef uint8_t producer_event_t;

struct producer_event {
    producer_event_t eventid;
};

enum PRODUCER_EVENT {
    PRODUCER_QUEUE_EMPTY,
    PRODUCER_QUEUE_FULL,
    PRODUCER_ETHERNET_RECEIVE,
};

uint8_t producer_queue_push(producer_event_t event);
producer_event_t producer_queue_pop(void);
void producer_handle_event(producer_event_t event);

#endif /* _PRODUCER_H_ */