#include "ssi.h"
#include "gpio.h"
#include "system.h"
#include "tm4c123gh6pm.h"
#include "ssi_module.h"

static void init_ssi_afsel(struct SSI *ssi);
static void init_ssi_pctl(struct SSI *ssi);
static void init_ssi_den(struct SSI *ssi);
static void init_ssi_amsel(struct SSI *ssi);

struct SSI SSI_0 = {
    SSI0,
    {PORTA, PIN5},
    {PORTA, PIN4},
    {PORTA, PIN3},
    {PORTA, PIN2}
};

struct SSI SSI_1 = {
    SSI1,
    {PORTD, PIN3},
    {PORTD, PIN2},
    {PORTD, PIN1},
    {PORTD, PIN0}
};

struct SSI SSI_2 = {
    SSI2,
    {PORTB, PIN7},
    {PORTB, PIN6},
    {PORTB, PIN5},
    {PORTB, PIN4}
};

struct SSI SSI_3 = {
    SSI3,
    {PORTD, PIN3},
    {PORTD, PIN2},
    {PORTD, PIN1},
    {PORTD, PIN0}
};

/******************** device specific ********************/

void init_ssi_mode(struct SSI *ssi, uint8_t clock_rate, uint8_t clock_phase,
                    uint8_t clock_polarity, uint8_t protocol, uint8_t data_size) {
    init_SSICR0(ssi->module, clock_rate, clock_phase,
                clock_polarity, protocol, data_size);
}

void init_ssi_clock_prescale_divider(struct SSI *ssi, uint8_t prescale_div) {
    init_SSICPSR(ssi->module, prescale_div);
}

/*********************************************************/

void init_ssi(struct SSI *ssi) {    
    set_RCGCSSI(ssi->module, 1);
    set_RCGCGPIO(ssi->tx.port, 1);
    init_ssi_afsel(ssi);
    init_ssi_pctl(ssi);
    init_ssi_den(ssi);
    init_ssi_amsel(ssi);
    init_SSICR1(ssi->module, 0);
    init_SSICC(ssi->module, 0);
}

void enable_ssi(struct SSI *ssi) {
    set_SSICR1SSE_high(ssi->module);
}

void disable_ssi(struct SSI *ssi) {
    set_SSICR1SSE_low(ssi->module);
}

static void init_ssi_afsel(struct SSI *ssi) {
    init_gpio_pin_AFSEL(ssi->tx.port, ssi->tx.pin, 1);
    init_gpio_pin_AFSEL(ssi->rx.port, ssi->rx.pin, 1);
    init_gpio_pin_AFSEL(ssi->fss.port, ssi->fss.pin, 1);
    init_gpio_pin_AFSEL(ssi->clk.port, ssi->clk.pin, 1);
}

static void init_ssi_pctl(struct SSI *ssi) {
    switch (ssi->module) {
        case SSI0:
            init_gpio_pin_PCTL(ssi->tx.port, ssi->tx.pin, GPIO_PCTL_PA5_SSI0TX);
            init_gpio_pin_PCTL(ssi->rx.port, ssi->rx.pin, GPIO_PCTL_PA4_SSI0RX);
            init_gpio_pin_PCTL(ssi->fss.port, ssi->fss.pin, GPIO_PCTL_PA3_SSI0FSS);
            init_gpio_pin_PCTL(ssi->clk.port, ssi->clk.pin, GPIO_PCTL_PA2_SSI0CLK);
            break;
        case SSI1:
            init_gpio_pin_PCTL(ssi->tx.port, ssi->tx.pin, GPIO_PCTL_PD3_SSI1TX);
            init_gpio_pin_PCTL(ssi->rx.port, ssi->rx.pin, GPIO_PCTL_PD2_SSI1RX);
            init_gpio_pin_PCTL(ssi->fss.port, ssi->fss.pin, GPIO_PCTL_PD1_SSI1FSS);
            init_gpio_pin_PCTL(ssi->clk.port, ssi->clk.pin, GPIO_PCTL_PD0_SSI1CLK);
            break;
        case SSI2:
            init_gpio_pin_PCTL(ssi->tx.port, ssi->tx.pin, GPIO_PCTL_PB7_SSI2TX);
            init_gpio_pin_PCTL(ssi->rx.port, ssi->rx.pin, GPIO_PCTL_PB6_SSI2RX);
            init_gpio_pin_PCTL(ssi->fss.port, ssi->fss.pin, GPIO_PCTL_PB5_SSI2FSS);
            init_gpio_pin_PCTL(ssi->clk.port, ssi->clk.pin, GPIO_PCTL_PB4_SSI2CLK);
            break;
        case SSI3:
            init_gpio_pin_PCTL(ssi->tx.port, ssi->tx.pin, GPIO_PCTL_PD3_SSI3TX);
            init_gpio_pin_PCTL(ssi->rx.port, ssi->rx.pin, GPIO_PCTL_PD2_SSI3RX);
            init_gpio_pin_PCTL(ssi->fss.port, ssi->fss.pin, GPIO_PCTL_PD1_SSI3FSS);
            init_gpio_pin_PCTL(ssi->clk.port, ssi->clk.pin, GPIO_PCTL_PD0_SSI3CLK);
            break;
    }
}

static void init_ssi_den(struct SSI *ssi) {
    init_gpio_pin_DEN(ssi->tx.port, ssi->tx.pin, 1);
    init_gpio_pin_DEN(ssi->rx.port, ssi->rx.pin, 1);
    init_gpio_pin_DEN(ssi->fss.port, ssi->fss.pin, 1);
    init_gpio_pin_DEN(ssi->clk.port, ssi->clk.pin, 1);
}

static void init_ssi_amsel(struct SSI *ssi) {
    init_gpio_pin_AMSEL(ssi->tx.port, ssi->tx.pin, 0);
    init_gpio_pin_AMSEL(ssi->rx.port, ssi->rx.pin, 0);
    init_gpio_pin_AMSEL(ssi->fss.port, ssi->fss.pin, 0);
    init_gpio_pin_AMSEL(ssi->clk.port, ssi->clk.pin, 0);
}
