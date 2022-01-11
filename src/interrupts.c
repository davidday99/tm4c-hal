#include <stdint.h>
#include "gpio.h"
#include "gpio_module.h"
#include "common.h"
#include "tm4c123gh6pm.h"
#include "dma_module.h"
#include "enc28j60.h"
#include "ipv4_in.h"
#include "lcd.h"

extern uint8_t ENC28J60_DMA_IN_PROGRESS;
extern LCD lcd;
extern struct ENC28J60 *enc;

void BusFault_Handler(void) {
    while (1)
    {
        
    }
}

void GPIOPortB_ISR(void) {
    uint8_t pin = get_gpio_port_MIS(PORTB);
    set_gpio_pin_ICR(PORTB, 1);
    set_gpio_pin_IM(PORTB, 1, 0);
        
    /* Check which pin on PORTB triggered the interrupt. PIN2 is connected to the
        ethernet controller's INT pin.
    */
    if (pin == 2) {
        ENC28J60_disable_interrupts(&ENC28J60);
        ENC28J60_decrement_packet_count(&ENC28J60);
        ENC28J60_read_frame_dma(&ENC28J60);
    }

    set_gpio_pin_IM(PORTB, 1, 1);
}

void SPI1_ISR(void) {
    if (dma_interrupt_occurred(enc->dmatx) || dma_interrupt_occurred(enc->dmarx)) {
        clear_dma_interrupt(enc->dmatx);
        clear_dma_interrupt(enc->dmarx);
    }

    if (remaining_dma_transfer_count(enc->dmarx) == 0) {
        set_gpio_pin_high(enc->cs);
        ENC28J60_disable_dma(enc);
        disable_ssi_interrupts(enc->ssi);
        ipv4_deliver((struct ipv4hdr *) &(enc->rx_buf[15]));
        ENC28J60_advance_rdptr(enc);
        ENC28J60_enable_interrupts(enc);
    }
}
