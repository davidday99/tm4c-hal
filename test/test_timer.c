#include "tm4c123gh6pm.h"
#include "timer.h"

static void Delay(uint32_t d) {
    uint32_t volatile delay = d * 80000;
    while (delay > 0)
        delay--;
}

int test_timer_16_bit() {
    int success = 1;
    
    init_GPTM(TIMER0, TIMER_A, 0, TIMER_MODE_ONE_SHOT);
    set_GPTM_start_value(TIMER0, TIMER_A, 10000);
    start_GPTM(TIMER0, TIMER_A);

    Delay(10);

    success &= (get_GPTM_RIS(TIMER0) & 1) == 1;

    return success;
}

int test_timer_32_bit() {
    int success = 1;

    init_GPTM(TIMER1, TIMER_A, 1, TIMER_MODE_ONE_SHOT);
    set_GPTM_start_value(TIMER1, TIMER_A, 100000000);
    // enable_GPTM_interrupts(TIMER1, TIMER_A);
    // enable_GPTM_timeout_interrupt(TIMER1, TIMER_A);
    start_GPTM(TIMER1, TIMER_A);

    // while (1)
    //     ;

    Delay(100);

    success &= (get_GPTM_RIS(TIMER1) & 1) == 1;

    return success;

}

int test_timer() {
    int success = 1;

    success &= test_timer_16_bit();
    success &= test_timer_32_bit();

    return success;
}