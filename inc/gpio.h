enum PORT {
  PORTA,
  PORTB,
  PORTC,
  PORTD,
  PORTF
};

enum PIN {
  PIN0,
  PIN1,
  PIN2,
  PIN3,
  PIN4,
  PIN5,
  PIN6,
  PIN7
};

//GPIO Definitions
void PortF_Init(void); // prototype call

void init_port_clock(enum PORT port);
void init_pin_as_input(enum PORT port, enum PIN pin);
void init_pin_as_output(enum PORT port, enum PIN pin);
void init_pin_as_af(enum PORT port, enum PIN pin);
void init_pin_as_gpio(enum PORT port, enum PIN pin);
void set_pin_drive_strength_2(enum PORT port, enum PIN pin);
void set_pin_drive_strength_4(enum PORT port, enum PIN pin);
void set_pin_drive_strength_8(enum PORT port, enum PIN pin);
void enable_pin_digital(enum PORT port, enum PIN pin);
void enable_pin_analog(enum PORT port, enum PIN pin);
void set_gpio_output_pin_high(enum PORT port, enum PIN pin);
void set_gpio_output_pin_low(enum PORT port, enum PIN pin);