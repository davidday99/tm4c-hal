#ifndef _TIMER_MODULE_H_
#define _TIMER_MODULE_H_

#include <stdint.h>
#include "common.h"

struct TIMER {
    enum TIMER_MODULE module;
    enum TIMER_SUBMODULE submodule;
    uint32_t start_low;
    uint32_t start_high;
};

extern struct TIMER TIMER0A;
extern struct TIMER TIMER0B;
extern struct TIMER TIMER1A;
extern struct TIMER TIMER1B;
extern struct TIMER TIMER2A; 
extern struct TIMER TIMER2B;
extern struct TIMER TIMER3A;
extern struct TIMER TIMER3B;
extern struct TIMER TIMER4A; 
extern struct TIMER TIMER4B; 
extern struct TIMER TIMER5A;
extern struct TIMER TIMER5B;
extern struct TIMER WTIMER0A;
extern struct TIMER WTIMER0B;
extern struct TIMER WTIMER1A;
extern struct TIMER WTIMER1B;
extern struct TIMER WTIMER2A; 
extern struct TIMER WTIMER2B; 
extern struct TIMER WTIMER3A; 
extern struct TIMER WTIMER3B;
extern struct TIMER WTIMER4A; 
extern struct TIMER WTIMER4B; 
extern struct TIMER WTIMER5A; 
extern struct TIMER WTIMER5B; 

void init_timer(struct TIMER *timer, uint8_t wide);
void set_timer_32_bit_starting_value(struct TIMER *timer, uint32_t start);
void start_timer(struct TIMER *timer);
void stop_timer(struct TIMER *timer);
void enable_timer_interrupts(struct TIMER *timer);
void disable_timer_interrupts(struct TIMER *timer);
void enable_timer_timeout_interupt(struct TIMER *timer);
void disable_timer_timeout_interupt(struct TIMER *timer);

#endif /* _TIMER_MODULE_H_ */