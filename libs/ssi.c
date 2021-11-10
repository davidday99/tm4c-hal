#include "tm4c123gh6pm.h"
#include <stdint.h>

struct SSI {
    volatile uint32_t *SSICR0;
    volatile uint32_t *SSICR1;
    volatile uint32_t *SSIDR;
    volatile uint32_t *SSISR;
    volatile uint32_t *SSICPSR;
    volatile uint32_t *SSIIM;
    volatile uint32_t *SSIRIS;
    volatile uint32_t *SSIMIS;
    volatile uint32_t *SSIICR;
    volatile uint32_t *SSIDMACTL;
    volatile uint32_t *SSICC;
};

static struct SSI SSI[] = {
    {&SSI0_CR0_R, &SSI0_CR1_R, &SSI0_DR_R},
    {&SSI1_CR0_R, &SSI1_CR1_R, &SSI1_DR_R},
};

void init_SSI(uint8_t module, uint8_t freq, uint8_t freq_scaler, uint8_t phase, uint8_t polarity, uint8_t size) {
    // TODO: init_GPIO_pins_ssi_mode();
    *(SSI[module].SSICR1) = 0;
    *(SSI[module].SSICC) = 0;
    *(SSI[module].SSICPSR) = freq;

    *(SSI[module].SSICR0) &= 0;
    *(SSI[module].SSICR0) |= freq << 8;
    *(SSI[module].SSICR0) |= phase << 7;
    
    if (phase) {
        *(SSI[module].SSICR0) |= polarity << 6;
        // TODO: init_GPIO_pin_pur()
    }

    *(SSI[module].SSICR0) |= size - 1;

    *(SSI[module].SSICR1) |= 2;
}