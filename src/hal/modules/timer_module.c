#include "timer_module.h"
#include "timer.h"
#include "common.h"

struct TIMER TIMER0A = {TIMER0, TIMER_A, 0, 0};
struct TIMER TIMER0B = {TIMER0, TIMER_B, 0, 0};
struct TIMER TIMER1A = {TIMER1, TIMER_A, 0, 0};
struct TIMER TIMER1B = {TIMER1, TIMER_B, 0, 0};
struct TIMER TIMER2A = {TIMER2, TIMER_A, 0, 0};
struct TIMER TIMER2B = {TIMER2, TIMER_B, 0, 0};
struct TIMER TIMER3A = {TIMER3, TIMER_A, 0, 0};
struct TIMER TIMER3B = {TIMER3, TIMER_B, 0, 0};
struct TIMER TIMER4A = {TIMER4, TIMER_A, 0, 0};
struct TIMER TIMER4B = {TIMER4, TIMER_B, 0, 0};
struct TIMER TIMER5A = {TIMER5, TIMER_A, 0, 0};
struct TIMER TIMER5B = {TIMER5, TIMER_B, 0, 0};
struct TIMER WTIMER0A = {WTIMER0, TIMER_A, 0, 0};
struct TIMER WTIMER0B = {WTIMER0, TIMER_B, 0, 0};
struct TIMER WTIMER1A = {WTIMER1, TIMER_A, 0, 0};
struct TIMER WTIMER1B = {WTIMER1, TIMER_B, 0, 0};
struct TIMER WTIMER2A = {WTIMER2, TIMER_A, 0, 0};
struct TIMER WTIMER2B = {WTIMER2, TIMER_B, 0, 0};
struct TIMER WTIMER3A = {WTIMER3, TIMER_A, 0, 0};
struct TIMER WTIMER3B = {WTIMER3, TIMER_B, 0, 0};
struct TIMER WTIMER4A = {WTIMER4, TIMER_A, 0, 0};
struct TIMER WTIMER4B = {WTIMER4, TIMER_B, 0, 0};
struct TIMER WTIMER5A = {WTIMER5, TIMER_A, 0, 0};
struct TIMER WTIMER5B = {WTIMER5, TIMER_B, 0, 0};


void init_timer(struct TIMER *timer, uint8_t wide) {
    init_GPTM(timer->module, timer->submodule, wide, TIMER_MODE_ONE_SHOT);
}

void set_timer_32_bit_starting_value(struct TIMER *timer, uint32_t start) {
    set_GPTM_start_value(timer->module, timer->submodule, start);
    timer->start_low = start;
}

void start_timer(struct TIMER *timer) {
    start_GPTM(timer->module, timer->submodule);
}

void stop_timer(struct TIMER *timer) {
    stop_GPTM(timer->module, timer->submodule);
}

void enable_timer_interrupts(struct TIMER *timer) {
    enable_GPTM_interrupts(timer->module, timer->submodule);
}

void disable_timer_interrupts(struct TIMER *timer) {
    disable_GPTM_interrupts(timer->module, timer->submodule);
}

void enable_timer_timeout_interrupt(struct TIMER *timer) {
    enable_GPTM_timeout_interrupt(timer->module, timer->submodule);
}

void disable_timer_timeout_interrupt(struct TIMER *timer) {
    disable_GPTM_timeout_interrupt(timer->module, timer->submodule);
}

void clear_timer_timeout_interrupt(struct TIMER *timer) {
    clear_GPTM_timeout_interrupt(timer->module, timer->submodule);
}