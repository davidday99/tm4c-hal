#include "tm4c123gh6pm.h"
#include "timer.h"

static void Delay(uint32_t d) {
    uint32_t volatile delay = d * 80000;
    while (delay > 0)
        delay--;
}

int test_timer() {
    int success = 1;

    init_GPTM(TIMER0, TIMER_A, 0, TIMER_MODE_ONE_SHOT);
    set_GPTM_start_value(TIMER0, TIMER_A, 10000);
    start_GPTM(TIMER0, TIMER_A);

    Delay(10);

    success &= (get_GPTM_RIS(TIMER0) & 1) == 1;

    return success;
}