#include <stdint.h>
#include "event_queue.h"
#include "gpio.h"
#include "common.h"

void BusFault_Handler(void) {
    while (1)
    {
        
    }
}

void GPIOPortB_ISR(void) {
    uint8_t pin = get_gpio_port_MIS(PORTB);
    set_gpio_pin_ICR(PORTB, 1);
    set_gpio_pin_IM(PORTB, 1, 0);
    if (pin == 2)
        event_queue_push(EVENT_ETHERNET_RECEIVE);

    set_gpio_pin_IM(PORTB, 1, 1);
}