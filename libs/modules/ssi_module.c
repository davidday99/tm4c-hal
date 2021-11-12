#include "ssi.h"
#include "gpio.h"
#include "system.h"
#include "tm4c123gh6pm.h"

struct SSI {
    enum SSI_MODULE module;
    struct GPIO_PIN tx;
    struct GPIO_PIN rx;
    struct GPIO_PIN fss;
    struct GPIO_PIN clk;
};

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

void init_ssi(struct SSI* ssi) {
    init_SSICR1(ssi->module, SSI_CR1_MS);
    set_RCGCGPIO(ssi->tx.port, 1);
}