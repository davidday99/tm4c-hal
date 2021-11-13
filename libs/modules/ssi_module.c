#include "ssi.h"
#include "gpio_module.h"
#include "system.h"
#include "tm4c123gh6pm.h"
#include "ssi_module.h"

static void init_ssi_afsel(struct SSI *ssi);
static void init_ssi_pctl(struct SSI *ssi);
static void init_ssi_den(struct SSI *ssi);

struct SSI SSI_0 = {
    SSI0,
    &PORTA_PIN5,
    &PORTA_PIN4,
    &PORTA_PIN3,
    &PORTA_PIN2
};

struct SSI SSI_1 = {
    SSI1,
    &PORTD_PIN3,
    &PORTD_PIN2,
    &PORTD_PIN1,
    &PORTD_PIN0
};

struct SSI SSI_2 = {
    SSI2,
    &PORTB_PIN7,
    &PORTB_PIN6,
    &PORTB_PIN5,
    &PORTB_PIN4,
};

struct SSI SSI_3 = {
    SSI3,
    &PORTD_PIN3,
    &PORTD_PIN2,
    &PORTD_PIN1,
    &PORTD_PIN0
};

/******************** device specific ********************/

void init_ssi_mode(struct SSI *ssi, uint8_t clock_rate, uint8_t clock_phase,
                    uint8_t clock_polarity, uint8_t protocol, uint8_t data_size) {
    init_SSICR0(ssi->module, clock_rate, clock_phase,
                clock_polarity, protocol, data_size - 1);
}

void init_ssi_clock_prescale_divider(struct SSI *ssi, uint8_t prescale_div) {
    init_SSICPSR(ssi->module, prescale_div);
}

/*********************************************************/

void init_ssi(struct SSI *ssi) {    
    set_RCGCSSI(ssi->module, 1);  // TODO: replace with higher-level function
    set_RCGCGPIO(ssi->tx->port, 1);  // TODO: replace with higher-level function
    init_ssi_afsel(ssi);
    init_ssi_pctl(ssi);
    init_ssi_den(ssi);
    init_SSICR1(ssi->module, 0);
    init_SSICC(ssi->module, 0);
}

void enable_ssi(struct SSI *ssi) {
    set_SSICR1SSE_high(ssi->module);
}

void disable_ssi(struct SSI *ssi) {
    set_SSICR1SSE_low(ssi->module);
}

uint32_t read_ssi(struct SSI *ssi, uint8_t *data, uint32_t size) {
    return read_SSIDR_into_array(ssi->module, data, size);
}

void write_ssi(struct SSI *ssi, uint8_t *data) {
    int len = 0;
    while (data[len] !=0)
        len++;
    write_array_SSIDR(ssi->module, data, len);
}

static void init_ssi_afsel(struct SSI *ssi) {
    enable_gpio_pin_alternate_function(ssi->tx);
    enable_gpio_pin_alternate_function(ssi->rx);
    enable_gpio_pin_alternate_function(ssi->fss);
    enable_gpio_pin_alternate_function(ssi->clk);
}

static void init_ssi_pctl(struct SSI *ssi) {
    switch (ssi->module) {
        case SSI0:
            set_gpio_pin_alternate_function(ssi->tx, GPIO_PCTL_PA5_SSI0TX);
            set_gpio_pin_alternate_function(ssi->rx, GPIO_PCTL_PA4_SSI0RX);
            set_gpio_pin_alternate_function(ssi->fss, GPIO_PCTL_PA3_SSI0FSS);
            set_gpio_pin_alternate_function(ssi->clk, GPIO_PCTL_PA2_SSI0CLK);
            break;
        case SSI1:
            set_gpio_pin_alternate_function(ssi->tx, GPIO_PCTL_PD3_SSI1TX);
            set_gpio_pin_alternate_function(ssi->rx, GPIO_PCTL_PD2_SSI1RX);
            set_gpio_pin_alternate_function(ssi->fss, GPIO_PCTL_PD1_SSI1FSS);
            set_gpio_pin_alternate_function(ssi->clk, GPIO_PCTL_PD0_SSI1CLK);
            break;
        case SSI2:
            set_gpio_pin_alternate_function(ssi->tx, GPIO_PCTL_PB7_SSI2TX);
            set_gpio_pin_alternate_function(ssi->rx, GPIO_PCTL_PB6_SSI2RX);
            set_gpio_pin_alternate_function(ssi->fss, GPIO_PCTL_PB5_SSI2FSS);
            set_gpio_pin_alternate_function(ssi->clk, GPIO_PCTL_PB4_SSI2CLK);
            break;
        case SSI3:
            set_gpio_pin_alternate_function(ssi->tx, GPIO_PCTL_PD3_SSI3TX);
            set_gpio_pin_alternate_function(ssi->rx, GPIO_PCTL_PD2_SSI3RX);
            set_gpio_pin_alternate_function(ssi->fss, GPIO_PCTL_PD1_SSI3FSS);
            set_gpio_pin_alternate_function(ssi->clk, GPIO_PCTL_PD0_SSI3CLK);
            break;
    }
}

static void init_ssi_den(struct SSI *ssi) {
    init_gpio_pin_as_digital(ssi->tx);
    init_gpio_pin_as_digital(ssi->rx);
    init_gpio_pin_as_digital(ssi->fss);
    init_gpio_pin_as_digital(ssi->clk);
}
