#include <stdint.h>
#include"gpio.h"
#include "tm4c123gh6pm.h"
#include "common.h"

struct GPIO_PORT {
    volatile uint32_t *DATA;
    volatile uint32_t *DIR;
    volatile uint32_t *IS;
    volatile uint32_t *IBE;
    volatile uint32_t *IEV;
    volatile uint32_t *IM;
    volatile uint32_t *RIS;
    volatile uint32_t *MIS;
    volatile uint32_t *ICR;
    volatile uint32_t *AFSEL;
    volatile uint32_t *DR2R;
    volatile uint32_t *DR4R;
    volatile uint32_t *DR8R;
    volatile uint32_t *ODR;
    volatile uint32_t *PUR;
    volatile uint32_t *PDR;
    volatile uint32_t *SLR;
    volatile uint32_t *DEN;
    volatile uint32_t *LOCK;
    volatile uint32_t *CR;
    volatile uint32_t *AMSEL;
    volatile uint32_t *PCTL;
    volatile uint32_t *ADCCTL;
    volatile uint32_t *DMACTL;
};

static struct GPIO_PORT PORTS[] = {
    {&GPIO_PORTA_DATA_R, &GPIO_PORTA_DIR_R, &GPIO_PORTA_IS_R, &GPIO_PORTA_IBE_R, &GPIO_PORTA_IEV_R, &GPIO_PORTA_IM_R,
    &GPIO_PORTA_RIS_R, &GPIO_PORTA_MIS_R, &GPIO_PORTA_ICR_R, &GPIO_PORTA_AFSEL_R, &GPIO_PORTA_DR2R_R, &GPIO_PORTA_DR4R_R,
    &GPIO_PORTA_DR8R_R, &GPIO_PORTA_ODR_R, &GPIO_PORTA_PUR_R, &GPIO_PORTA_PDR_R, &GPIO_PORTA_SLR_R, &GPIO_PORTA_DEN_R, 
    &GPIO_PORTA_LOCK_R, &GPIO_PORTA_CR_R, &GPIO_PORTA_AMSEL_R, &GPIO_PORTA_PCTL_R, &GPIO_PORTA_ADCCTL_R, &GPIO_PORTA_DMACTL_R},

    {&GPIO_PORTB_DATA_R, &GPIO_PORTB_DIR_R, &GPIO_PORTB_IS_R, &GPIO_PORTB_IBE_R, &GPIO_PORTB_IEV_R, &GPIO_PORTB_IM_R,
    &GPIO_PORTB_RIS_R, &GPIO_PORTB_MIS_R, &GPIO_PORTB_ICR_R, &GPIO_PORTB_AFSEL_R, &GPIO_PORTB_DR2R_R, &GPIO_PORTB_DR4R_R,
    &GPIO_PORTB_DR8R_R, &GPIO_PORTB_ODR_R, &GPIO_PORTB_PUR_R, &GPIO_PORTB_PDR_R, &GPIO_PORTB_SLR_R, &GPIO_PORTB_DEN_R, 
    &GPIO_PORTB_LOCK_R, &GPIO_PORTB_CR_R, &GPIO_PORTB_AMSEL_R, &GPIO_PORTB_PCTL_R, &GPIO_PORTB_ADCCTL_R, &GPIO_PORTB_DMACTL_R},

    {&GPIO_PORTC_DATA_R, &GPIO_PORTC_DIR_R, &GPIO_PORTC_IS_R, &GPIO_PORTC_IBE_R, &GPIO_PORTC_IEV_R, &GPIO_PORTC_IM_R,
    &GPIO_PORTC_RIS_R, &GPIO_PORTC_MIS_R, &GPIO_PORTC_ICR_R, &GPIO_PORTC_AFSEL_R, &GPIO_PORTC_DR2R_R, &GPIO_PORTC_DR4R_R,
    &GPIO_PORTC_DR8R_R, &GPIO_PORTC_ODR_R, &GPIO_PORTC_PUR_R, &GPIO_PORTC_PDR_R, &GPIO_PORTC_SLR_R, &GPIO_PORTC_DEN_R, 
    &GPIO_PORTC_LOCK_R, &GPIO_PORTC_CR_R, &GPIO_PORTC_AMSEL_R, &GPIO_PORTC_PCTL_R, &GPIO_PORTC_ADCCTL_R, &GPIO_PORTC_DMACTL_R},

    {&GPIO_PORTD_DATA_R, &GPIO_PORTD_DIR_R, &GPIO_PORTD_IS_R, &GPIO_PORTD_IBE_R, &GPIO_PORTD_IEV_R, &GPIO_PORTD_IM_R,
    &GPIO_PORTD_RIS_R, &GPIO_PORTD_MIS_R, &GPIO_PORTD_ICR_R, &GPIO_PORTD_AFSEL_R, &GPIO_PORTD_DR2R_R, &GPIO_PORTD_DR4R_R,
    &GPIO_PORTD_DR8R_R, &GPIO_PORTD_ODR_R, &GPIO_PORTD_PUR_R, &GPIO_PORTD_PDR_R, &GPIO_PORTD_SLR_R, &GPIO_PORTD_DEN_R, 
    &GPIO_PORTD_LOCK_R, &GPIO_PORTD_CR_R, &GPIO_PORTD_AMSEL_R, &GPIO_PORTD_PCTL_R, &GPIO_PORTD_ADCCTL_R, &GPIO_PORTD_DMACTL_R},

    {&GPIO_PORTE_DATA_R, &GPIO_PORTE_DIR_R, &GPIO_PORTE_IS_R, &GPIO_PORTE_IBE_R, &GPIO_PORTE_IEV_R, &GPIO_PORTE_IM_R,
    &GPIO_PORTE_RIS_R, &GPIO_PORTE_MIS_R, &GPIO_PORTE_ICR_R, &GPIO_PORTE_AFSEL_R, &GPIO_PORTE_DR2R_R, &GPIO_PORTE_DR4R_R,
    &GPIO_PORTE_DR8R_R, &GPIO_PORTE_ODR_R, &GPIO_PORTE_PUR_R, &GPIO_PORTE_PDR_R, &GPIO_PORTE_SLR_R, &GPIO_PORTE_DEN_R, 
    &GPIO_PORTE_LOCK_R, &GPIO_PORTE_CR_R, &GPIO_PORTE_AMSEL_R, &GPIO_PORTE_PCTL_R, &GPIO_PORTE_ADCCTL_R, &GPIO_PORTE_DMACTL_R},

    {&GPIO_PORTF_DATA_R, &GPIO_PORTF_DIR_R, &GPIO_PORTF_IS_R, &GPIO_PORTF_IBE_R, &GPIO_PORTF_IEV_R, &GPIO_PORTF_IM_R,
    &GPIO_PORTF_RIS_R, &GPIO_PORTF_MIS_R, &GPIO_PORTF_ICR_R, &GPIO_PORTF_AFSEL_R, &GPIO_PORTF_DR2R_R, &GPIO_PORTF_DR4R_R,
    &GPIO_PORTF_DR8R_R, &GPIO_PORTF_ODR_R, &GPIO_PORTF_PUR_R, &GPIO_PORTF_PDR_R, &GPIO_PORTF_SLR_R, &GPIO_PORTF_DEN_R, 
    &GPIO_PORTF_LOCK_R, &GPIO_PORTF_CR_R, &GPIO_PORTF_AMSEL_R, &GPIO_PORTF_PCTL_R, &GPIO_PORTF_ADCCTL_R, &GPIO_PORTF_DMACTL_R},
};

void init_gpio_pin_DIR(enum PORT port, enum PIN pin, uint8_t value) {
    *(PORTS[port].DIR) = (*(PORTS[port].DIR) & ~(1 << pin)) |
                            (value << pin);
}

void init_gpio_pin_AFSEL(enum PORT port, enum PIN pin, uint8_t value) {
    *(PORTS[port].AFSEL) = (*(PORTS[port].AFSEL) & ~(1 << pin)) |
                            (value << pin);
}

void init_gpio_pin_DR2R(enum PORT port, enum PIN pin, uint8_t value) {
    *(PORTS[port].DR2R) = (*(PORTS[port].DR2R) & ~(1 << pin)) |
                            (value << pin);
}

void init_gpio_pin_DR4R(enum PORT port, enum PIN pin, uint8_t value) {
    *(PORTS[port].DR4R) = (*(PORTS[port].DR4R) & ~(1 << pin)) |
                            (value << pin);
}

void init_gpio_pin_DR8R(enum PORT port, enum PIN pin, uint8_t value) {
    *(PORTS[port].DR8R) = (*(PORTS[port].DR8R) & ~(1 << pin)) |
                            (value << pin);
}

void init_gpio_pin_PUR(enum PORT port, enum PIN pin, uint8_t value) {
    *(PORTS[port].PUR) = (*(PORTS[port].PUR) & ~(1 << pin)) |
                            (value << pin);
}

void init_gpio_pin_PDR(enum PORT port, enum PIN pin, uint8_t value) {
    *(PORTS[port].PDR) = (*(PORTS[port].PDR) & ~(1 << pin)) |
                            (value << pin);
}

void init_gpio_pin_DEN(enum PORT port, enum PIN pin, uint8_t value) {
    *(PORTS[port].DEN) = (*(PORTS[port].DEN) & ~(1 << pin)) |
                            (value << pin);
}

void unlock_gpio_port(enum PORT port) {
    *(PORTS[port].LOCK) = GPIO_LOCK_KEY;
}

void lock_gpio_port(enum PORT port) {
    *(PORTS[port].LOCK) = 0;
}

void init_gpio_pin_CR(enum PORT port, enum PIN pin, uint8_t value) {
    *(PORTS[port].CR) = (*(PORTS[port].CR) & ~(1 << pin)) |
                            (value << pin);
}

void init_gpio_pin_AMSEL(enum PORT port, enum PIN pin, uint8_t value) {
    *(PORTS[port].AMSEL) = (*(PORTS[port].AMSEL) & ~(1 << pin)) |
                            (value << pin);
}

void init_gpio_pin_PCTL(enum PORT port, enum PIN pin, uint32_t value) {
    uint8_t shift = pin * 4;
    *(PORTS[port].PCTL) = (*(PORTS[port].PCTL) & ~(0xF << shift)) | value;
}

uint8_t read_gpio_pin_DATA(enum PORT port, enum PIN pin) {
    return *(PORTS[port].DATA) & ~(1 << pin);
}

void set_gpio_pin_DATA(enum PORT port, enum PIN pin, uint8_t value) {
    *(PORTS[port].DATA) = (*(PORTS[port].DATA) & ~(1 << pin)) |
                            (value << pin);
}

void set_gpio_pin_IS(enum PORT port, enum PIN pin, uint8_t value) {
    *(PORTS[port].IS) = (*(PORTS[port].IS) & ~(1 << pin)) |
                            (value << pin);
}

uint8_t get_gpio_port_RIS(enum PORT port) {
    return *(PORTS[port].RIS);
}

uint8_t get_gpio_port_MIS(enum PORT port) {
    return *(PORTS[port].MIS);
}

void set_gpio_pin_ICR(enum PORT port, enum PIN pin) {
    *(PORTS[port].ICR) = (*(PORTS[port].ICR) & ~(1 << pin)) |
                            (1 << pin);
}

void set_gpio_pin_IEV(enum PORT port, enum PIN pin, uint8_t value) {
    *(PORTS[port].IEV) = (*(PORTS[port].IEV) & ~(1 << pin)) |
                            (value << pin);
}

void set_gpio_pin_IBE(enum PORT port, enum PIN pin, uint8_t value) {
    *(PORTS[port].IBE) = (*(PORTS[port].IBE) & ~(1 << pin)) |
                            (value << pin);
}

void set_gpio_pin_IM(enum PORT port, enum PIN pin, uint8_t value) {
    *(PORTS[port].IM) = (*(PORTS[port].IM) & ~(1 << pin)) |
                            (value << pin);
}