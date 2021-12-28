#include <stdint.h>
#include "gpio_module.h"
#include "gpio.h"
#include "common.h"
#include "system.h"

struct GPIO_PIN PORTA_PIN0 = {PORTA, PIN0};
struct GPIO_PIN PORTA_PIN1 = {PORTA, PIN1};
struct GPIO_PIN PORTA_PIN2 = {PORTA, PIN2};
struct GPIO_PIN PORTA_PIN3 = {PORTA, PIN3};
struct GPIO_PIN PORTA_PIN4 = {PORTA, PIN4};
struct GPIO_PIN PORTA_PIN5 = {PORTA, PIN5};
struct GPIO_PIN PORTA_PIN6 = {PORTA, PIN6};
struct GPIO_PIN PORTA_PIN7 = {PORTA, PIN7};

struct GPIO_PIN PORTB_PIN0 = {PORTB, PIN0};
struct GPIO_PIN PORTB_PIN1 = {PORTB, PIN1};
struct GPIO_PIN PORTB_PIN2 = {PORTB, PIN2};
struct GPIO_PIN PORTB_PIN3 = {PORTB, PIN3};
struct GPIO_PIN PORTB_PIN4 = {PORTB, PIN4};
struct GPIO_PIN PORTB_PIN5 = {PORTB, PIN5};
struct GPIO_PIN PORTB_PIN6 = {PORTB, PIN6};
struct GPIO_PIN PORTB_PIN7 = {PORTB, PIN7};

struct GPIO_PIN PORTC_PIN0 = {PORTC, PIN0};
struct GPIO_PIN PORTC_PIN1 = {PORTC, PIN1};
struct GPIO_PIN PORTC_PIN2 = {PORTC, PIN2};
struct GPIO_PIN PORTC_PIN3 = {PORTC, PIN3};
struct GPIO_PIN PORTC_PIN4 = {PORTC, PIN4};
struct GPIO_PIN PORTC_PIN5 = {PORTC, PIN5};
struct GPIO_PIN PORTC_PIN6 = {PORTC, PIN6};
struct GPIO_PIN PORTC_PIN7 = {PORTC, PIN7};

struct GPIO_PIN PORTD_PIN0 = {PORTD, PIN0};
struct GPIO_PIN PORTD_PIN1 = {PORTD, PIN1};
struct GPIO_PIN PORTD_PIN2 = {PORTD, PIN2};
struct GPIO_PIN PORTD_PIN3 = {PORTD, PIN3};
struct GPIO_PIN PORTD_PIN4 = {PORTD, PIN4};
struct GPIO_PIN PORTD_PIN5 = {PORTD, PIN5};
struct GPIO_PIN PORTD_PIN6 = {PORTD, PIN6};
struct GPIO_PIN PORTD_PIN7 = {PORTD, PIN7};

struct GPIO_PIN PORTE_PIN0 = {PORTE, PIN0};
struct GPIO_PIN PORTE_PIN1 = {PORTE, PIN1};
struct GPIO_PIN PORTE_PIN2 = {PORTE, PIN2};
struct GPIO_PIN PORTE_PIN3 = {PORTE, PIN3};
struct GPIO_PIN PORTE_PIN4 = {PORTE, PIN4};
struct GPIO_PIN PORTE_PIN5 = {PORTE, PIN5};
struct GPIO_PIN PORTE_PIN6 = {PORTE, PIN6};
struct GPIO_PIN PORTE_PIN7 = {PORTE, PIN7};

struct GPIO_PIN PORTF_PIN0 = {PORTF, PIN0};
struct GPIO_PIN PORTF_PIN1 = {PORTF, PIN1};
struct GPIO_PIN PORTF_PIN2 = {PORTF, PIN2};
struct GPIO_PIN PORTF_PIN3 = {PORTF, PIN3};
struct GPIO_PIN PORTF_PIN4 = {PORTF, PIN4};
struct GPIO_PIN PORTF_PIN5 = {PORTF, PIN5};
struct GPIO_PIN PORTF_PIN6 = {PORTF, PIN6};
struct GPIO_PIN PORTF_PIN7 = {PORTF, PIN7};

static void init_gpio_falling_edge_or_level_low_interrupt(struct GPIO_PIN *gpio_pin);
static void init_gpio_rising_edge_or_level_high_interrupt(struct GPIO_PIN *gpio_pin);

void init_gpio_port_clock(struct GPIO_PIN *gpio_pin) {
    set_RCGCGPIO(gpio_pin->port, 1);
}

void init_gpio_pin_as_input(struct GPIO_PIN *gpio_pin) {
    init_gpio_pin_DIR(gpio_pin->port, gpio_pin->pin, 0);
}

void init_gpio_pin_as_output(struct GPIO_PIN *gpio_pin) {
    init_gpio_pin_DIR(gpio_pin->port, gpio_pin->pin, 1);
}

void init_gpio_pin_as_digital(struct GPIO_PIN *gpio_pin) {
    init_gpio_pin_AMSEL(gpio_pin->port, gpio_pin->pin, 0);
    init_gpio_pin_DEN(gpio_pin->port, gpio_pin->pin, 1);
}

void init_gpio_pin_as_analog(struct GPIO_PIN *gpio_pin) {
    init_gpio_pin_DEN(gpio_pin->port, gpio_pin->pin, 0);
    init_gpio_pin_AMSEL(gpio_pin->port, gpio_pin->pin, 1);
}

void enable_gpio_pin_alternate_function(struct GPIO_PIN *gpio_pin) {
    init_gpio_pin_AFSEL(gpio_pin->port, gpio_pin->pin, 1);
}

void disable_gpio_pin_alternate_function(struct GPIO_PIN *gpio_pin) {
    init_gpio_pin_AFSEL(gpio_pin->port, gpio_pin->pin, 0);
}

void set_gpio_pin_drive_strength(struct GPIO_PIN *gpio_pin, enum DRIVE_STRENGTH drive_strength) {
    switch (drive_strength) {
        case DS_2ma:
            init_gpio_pin_DR4R(gpio_pin->port, gpio_pin->pin, 0);
            init_gpio_pin_DR8R(gpio_pin->port, gpio_pin->pin, 0);
            init_gpio_pin_DR2R(gpio_pin->port, gpio_pin->pin, 1);
            break;
        case DS_4ma:
            init_gpio_pin_DR2R(gpio_pin->port, gpio_pin->pin, 0);
            init_gpio_pin_DR8R(gpio_pin->port, gpio_pin->pin, 0);
            init_gpio_pin_DR4R(gpio_pin->port, gpio_pin->pin, 1);
            break;
        case DS_8ma:
            init_gpio_pin_DR2R(gpio_pin->port, gpio_pin->pin, 0);
            init_gpio_pin_DR4R(gpio_pin->port, gpio_pin->pin, 0);
            init_gpio_pin_DR8R(gpio_pin->port, gpio_pin->pin, 1);
            break;
    }
}

void enable_gpio_pin_pullup_resistor(struct GPIO_PIN *gpio_pin) {
    init_gpio_pin_PDR(gpio_pin->port, gpio_pin->pin, 0);
    init_gpio_pin_PUR(gpio_pin->port, gpio_pin->pin, 1);
}

void enable_gpio_pin_pulldown_resistor(struct GPIO_PIN *gpio_pin) {
    init_gpio_pin_PUR(gpio_pin->port, gpio_pin->pin, 0);
    init_gpio_pin_PDR(gpio_pin->port, gpio_pin->pin, 1);
}

void set_gpio_pin_alternate_function(struct GPIO_PIN *gpio_pin, uint32_t value) {
    init_gpio_pin_PCTL(gpio_pin->port, gpio_pin->pin, value);
}

void unlock_and_commit_gpio_pin(struct GPIO_PIN *gpio_pin) {
    unlock_gpio_port(gpio_pin->port);
    init_gpio_pin_CR(gpio_pin->port, gpio_pin->pin, 1);
}

void uncommit_and_lock_gpio_pin(struct GPIO_PIN *gpio_pin) {
    init_gpio_pin_CR(gpio_pin->port, gpio_pin->pin, 0);
    lock_gpio_port(gpio_pin->port);
}

void set_gpio_pin_high(struct GPIO_PIN *gpio_pin) {
    set_gpio_pin_DATA(gpio_pin->port, gpio_pin->pin, 1);
}

void set_gpio_pin_low(struct GPIO_PIN *gpio_pin) {
    set_gpio_pin_DATA(gpio_pin->port, gpio_pin->pin, 0);
}

void enable_gpio_edge_interrupt(struct GPIO_PIN *gpio_pin) {
    set_gpio_pin_IS(gpio_pin->port, gpio_pin->pin, 0);
}

void enable_gpio_level_interrupt(struct GPIO_PIN *gpio_pin) {
    set_gpio_pin_IS(gpio_pin->port, gpio_pin->pin, 1);
}

void mask_gpio_interrupt(struct GPIO_PIN *gpio_pin) {
    set_gpio_pin_IM(gpio_pin->port, gpio_pin->pin, 0);
}

void unmask_gpio_interrupt(struct GPIO_PIN *gpio_pin) {
    set_gpio_pin_IM(gpio_pin->port, gpio_pin->pin, 1);
}

uint8_t get_gpio_interrupt_mask(enum PORT port) {
    return get_gpio_port_MIS(port);
}

uint8_t get_gpio_raw_interrupt_mask(enum PORT port) {
    return get_gpio_port_RIS(port);
}

void clear_gpio_interrupt(struct GPIO_PIN *gpio_pin) {
    set_gpio_pin_ICR(gpio_pin->port, gpio_pin->pin);
}

void init_gpio_both_edges_interrupt(struct GPIO_PIN *gpio_pin) {
    set_gpio_pin_IBE(gpio_pin->port, gpio_pin->pin, 1);
}

void enable_gpio_falling_edge_interrupt(struct GPIO_PIN *gpio_pin) {
    set_NVIC_EN0_bit(gpio_pin->port);
    mask_gpio_interrupt(gpio_pin);
    enable_gpio_edge_interrupt(gpio_pin);
    init_gpio_falling_edge_or_level_low_interrupt(gpio_pin);
    clear_gpio_interrupt(gpio_pin);
    unmask_gpio_interrupt(gpio_pin);
}

void enable_gpio_rising_edge_interrupt(struct GPIO_PIN *gpio_pin) {
    mask_gpio_interrupt(gpio_pin);
    enable_gpio_edge_interrupt(gpio_pin);
    init_gpio_rising_edge_or_level_high_interrupt(gpio_pin);
    clear_gpio_interrupt(gpio_pin);
    unmask_gpio_interrupt(gpio_pin);
}

void enable_gpio_both_edges_interrupt(struct GPIO_PIN *gpio_pin) {
    mask_gpio_interrupt(gpio_pin);
    enable_gpio_edge_interrupt(gpio_pin);
    init_gpio_both_edges_interrupt(gpio_pin);
    clear_gpio_interrupt(gpio_pin);
    unmask_gpio_interrupt(gpio_pin);
}

void enable_gpio_level_low_interrupt(struct GPIO_PIN *gpio_pin) {
    mask_gpio_interrupt(gpio_pin);
    enable_gpio_level_interrupt(gpio_pin);
    init_gpio_falling_edge_or_level_low_interrupt(gpio_pin);
    clear_gpio_interrupt(gpio_pin);
    unmask_gpio_interrupt(gpio_pin);
}

void enable_gpio_level_high_interrupt(struct GPIO_PIN *gpio_pin) {
    mask_gpio_interrupt(gpio_pin);
    enable_gpio_level_interrupt(gpio_pin);
    init_gpio_rising_edge_or_level_high_interrupt(gpio_pin);
    clear_gpio_interrupt(gpio_pin);
    unmask_gpio_interrupt(gpio_pin);
}

static void init_gpio_falling_edge_or_level_low_interrupt(struct GPIO_PIN *gpio_pin) {
    set_gpio_pin_IBE(gpio_pin->port, gpio_pin->pin, 0);
    set_gpio_pin_IEV(gpio_pin->port, gpio_pin->pin, 0);
}

static void init_gpio_rising_edge_or_level_high_interrupt(struct GPIO_PIN *gpio_pin) {
    set_gpio_pin_IBE(gpio_pin->port, gpio_pin->pin, 0);
    set_gpio_pin_IEV(gpio_pin->port, gpio_pin->pin, 1);
}


