#ifndef _EVENT_QUEUE_H_
#define _EVENT_QUEUE_H_

#include <stdint.h>

typedef uint8_t event_t;

enum EVENT {
    EVENT_QUEUE_EMPTY,
    EVENT_QUEUE_FULL,
    EVENT_ETHERNET_RECEIVE,
    EVENT_ETHERNET_TRANSMIT,
    EVENT_ARP_RECEIVE,
    EVENT_IP_RECEIVE,
    EVENT_LCD_WRITE,
};

uint8_t event_queue_push(event_t event);
event_t event_queue_pop(void);
void event_queue_handle_event(event_t eventid, ...);

#endif /* _EVENT_QUEUE_H_ */ 
