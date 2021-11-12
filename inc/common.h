#ifndef _COMMON_H_
#define _COMMON_H_

enum PORT {
    PORTA,
    PORTB,
    PORTC,
    PORTD,
    PORTE,
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

enum SSI_MODULE {
    SSI0,
    SSI1,
    SSI2,
    SSI3,
};

struct GPIO_PIN{
    enum PORT port;
    enum PIN pin;
};


#endif /* _COMMON_H_ */
