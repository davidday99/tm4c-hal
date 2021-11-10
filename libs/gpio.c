#include <stdint.h>

#include"gpio.h"
#include "tm4c123gh6pm.h"

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
};

void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
}

void init_port_clock(enum PORT port) {
  SYSCTL_RCGCGPIO_R |= 1 << port;
}

void init_pin_as_input(enum PORT port, enum PIN pin) {
  *(PORTS[port].DIR) &= ~(1 << pin);
}

void init_pin_as_output(enum PORT port, enum PIN pin) {
  *(PORTS[port].DIR) |= 1 << pin;
}

void init_pin_as_af(enum PORT port, enum PIN pin) {
  *(PORTS[port].AFSEL) |= 1 << pin;
}

void init_pin_as_gpio(enum PORT port, enum PIN pin) {
  *(PORTS[port].AFSEL) &= ~(1 << pin);
}

void set_pin_drive_strength_2(enum PORT port, enum PIN pin) {
  *(PORTS[port].DR2R) |= 1 << pin;
  *(PORTS[port].DR4R) &= ~(1 << pin);
  *(PORTS[port].DR8R) &= ~(1 << pin);
}

void set_pin_drive_strength_4(enum PORT port, enum PIN pin) {
  *(PORTS[port].DR2R) &= ~(1 << pin);
  *(PORTS[port].DR4R) |= 1 << pin;
  *(PORTS[port].DR8R) &= ~(1 << pin);
}

void set_pin_drive_strength_8(enum PORT port, enum PIN pin) {
  *(PORTS[port].DR2R) &= ~(1 << pin);
  *(PORTS[port].DR4R) &= ~(1 << pin);
  *(PORTS[port].DR8R) |= 1 << pin;
}

void enable_pin_digital(enum PORT port, enum PIN pin) {
  *(PORTS[port].DEN) |= 1 << pin;
  *(PORTS[port].AMSEL) &= ~(1 << pin);
}

void enable_pin_analog(enum PORT port, enum PIN pin) {
  *(PORTS[port].AMSEL) |= 1 << pin;
  *(PORTS[port].DEN) &= ~(1 << pin);
}

void set_gpio_output_pin_high(enum PORT port, enum PIN pin) {
  *(PORTS[port].DATA) |= 1 << pin;
}

void set_gpio_output_pin_low(enum PORT port, enum PIN pin) {
  *(PORTS[port].DATA) &= ~(1 << pin);
}