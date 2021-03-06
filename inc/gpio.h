#ifndef _GPIO_H_
#define _GPIO_H_

#include "common.h"

void init_gpio_pin_DIR(enum PORT port, enum PIN pin, uint8_t value);
void init_gpio_pin_AFSEL(enum PORT port, enum PIN pin, uint8_t value);
void init_gpio_pin_DR2R(enum PORT port, enum PIN pin, uint8_t value);
void init_gpio_pin_DR4R(enum PORT port, enum PIN pin, uint8_t value);
void init_gpio_pin_DR8R(enum PORT port, enum PIN pin, uint8_t value);
void init_gpio_pin_PUR(enum PORT port, enum PIN pin, uint8_t value);
void init_gpio_pin_PDR(enum PORT port, enum PIN pin, uint8_t value);
void init_gpio_pin_DEN(enum PORT port, enum PIN pin, uint8_t value);
void unlock_gpio_port(enum PORT port);
void init_gpio_pin_CR(enum PORT port, enum PIN pin, uint8_t value);
void lock_gpio_port(enum PORT port);
void init_gpio_pin_AMSEL(enum PORT port, enum PIN pin, uint8_t value);
void init_gpio_pin_PCTL(enum PORT port, enum PIN pin, uint32_t value);
uint8_t read_gpio_pin_DATA(enum PORT port, enum PIN pin);
void set_gpio_pin_DATA(enum PORT port, enum PIN pin, uint8_t value);
void set_gpio_pin_IS(enum PORT port, enum PIN pin, uint8_t value);
uint8_t get_gpio_port_RIS(enum PORT port);
uint8_t get_gpio_port_MIS(enum PORT port);
void set_gpio_pin_ICR(enum PORT port, enum PIN pin);
void set_gpio_pin_IEV(enum PORT port, enum PIN pin, uint8_t value);
void set_gpio_pin_IBE(enum PORT port, enum PIN pin, uint8_t value);
void set_gpio_pin_IM(enum PORT port, enum PIN pin, uint8_t value);

#endif /* _GPIO_H_ */