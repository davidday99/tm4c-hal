#ifndef _GPIO_MODULE_H_
#define _GPIO_MODULE_H_

extern struct GPIO_PIN PORTA_PIN0;
extern struct GPIO_PIN PORTA_PIN1;
extern struct GPIO_PIN PORTA_PIN2;
extern struct GPIO_PIN PORTA_PIN3;
extern struct GPIO_PIN PORTA_PIN4;
extern struct GPIO_PIN PORTA_PIN5;
extern struct GPIO_PIN PORTA_PIN6;
extern struct GPIO_PIN PORTA_PIN7;

extern struct GPIO_PIN PORTB_PIN0;
extern struct GPIO_PIN PORTB_PIN1;
extern struct GPIO_PIN PORTB_PIN2;
extern struct GPIO_PIN PORTB_PIN3;
extern struct GPIO_PIN PORTB_PIN4;
extern struct GPIO_PIN PORTB_PIN5;
extern struct GPIO_PIN PORTB_PIN6;
extern struct GPIO_PIN PORTB_PIN7;

extern struct GPIO_PIN PORTC_PIN0;
extern struct GPIO_PIN PORTC_PIN1;
extern struct GPIO_PIN PORTC_PIN2;
extern struct GPIO_PIN PORTC_PIN3;
extern struct GPIO_PIN PORTC_PIN4;
extern struct GPIO_PIN PORTC_PIN5;
extern struct GPIO_PIN PORTC_PIN6;
extern struct GPIO_PIN PORTC_PIN7;

extern struct GPIO_PIN PORTD_PIN0;
extern struct GPIO_PIN PORTD_PIN1;
extern struct GPIO_PIN PORTD_PIN2;
extern struct GPIO_PIN PORTD_PIN3;
extern struct GPIO_PIN PORTD_PIN4;
extern struct GPIO_PIN PORTD_PIN5;
extern struct GPIO_PIN PORTD_PIN6;
extern struct GPIO_PIN PORTD_PIN7;

extern struct GPIO_PIN PORTE_PIN0;
extern struct GPIO_PIN PORTE_PIN1;
extern struct GPIO_PIN PORTE_PIN2;
extern struct GPIO_PIN PORTE_PIN3;
extern struct GPIO_PIN PORTE_PIN4;
extern struct GPIO_PIN PORTE_PIN5;
extern struct GPIO_PIN PORTE_PIN6;
extern struct GPIO_PIN PORTE_PIN7;

extern struct GPIO_PIN PORTF_PIN0;
extern struct GPIO_PIN PORTF_PIN1;
extern struct GPIO_PIN PORTF_PIN2;
extern struct GPIO_PIN PORTF_PIN3;
extern struct GPIO_PIN PORTF_PIN4;
extern struct GPIO_PIN PORTF_PIN5;
extern struct GPIO_PIN PORTF_PIN6;
extern struct GPIO_PIN PORTF_PIN7;

enum DRIVE_STRENGTH {
    DS_2ma,
    DS_4ma,
    DS_8ma
};

void init_gpio_port_clock(struct GPIO_PIN *gpio_pin);
void init_gpio_pin_as_input(struct GPIO_PIN *gpio_pin);
void init_gpio_pin_as_output(struct GPIO_PIN *gpio_pin);
void init_gpio_pin_as_digital(struct GPIO_PIN *gpio_pin);
void init_gpio_pin_as_analog(struct GPIO_PIN *gpio_pin);
void enable_gpio_pin_alternate_function(struct GPIO_PIN *gpio_pin);
void disable_gpio_pin_alternate_function(struct GPIO_PIN *gpio_pin);
void set_gpio_pin_drive_strength(struct GPIO_PIN *gpio_pin, enum DRIVE_STRENGTH drive_strength);
void enable_gpio_pin_pullup_resistor(struct GPIO_PIN *gpio_pin);
void enable_gpio_pin_pulldown_resistor(struct GPIO_PIN *gpio_pin);
void set_gpio_pin_alternate_function(struct GPIO_PIN *gpio_pin, uint32_t value);
void unlock_and_commit_gpio_pin(struct GPIO_PIN *gpio_pin);
void uncommit_and_lock_gpio_pin(struct GPIO_PIN *gpio_pin);
void set_gpio_pin_high(struct GPIO_PIN *gpio_pin);
void set_gpio_pin_low(struct GPIO_PIN *gpio_pin);

#endif /* _GPIO_MODULE_H_ */