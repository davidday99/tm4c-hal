#include <stdint.h>
#include "event_queue.h"
#include "gpio.h"
#include "common.h"
#include "enc.h"
#include "ssi.h"

void BusFault_Handler(void) {
    while (1)
    {
        
    }
}

void GPIOPortB_ISR(void) {
    uint8_t pin = get_gpio_port_MIS(PORTB);
    set_gpio_pin_ICR(PORTB, 1);
    set_gpio_pin_IM(PORTB, 1, 0);

    /* if SSI is currently in use by the ethernet controller in the main thread,
        then put off clearing the flag until a later time. If this happens then
        the packet count needs to be manually checked to account for any packets
        that are missed during the time between the last interrupt and the point 
        at which the controller's interrupt flag is actually cleared.
    */
    // if (!SSI_bsy(SSI1))
    //     enc_clear_interrupt_flag();
    // else
    //     event_queue_push(EVENT_ETHERNET_FRAME_WAITING);
    
    /* Check which pin on PORTB triggered the interrupt. PIN2 is connected to the
        ethernet controller's INT pin.
    */
    if (pin == 2)
        event_queue_push(EVENT_ETHERNET_RECEIVE);

    set_gpio_pin_IM(PORTB, 1, 1);
}