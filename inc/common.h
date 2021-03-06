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

enum TIMER_MODULE {
    TIMER0,
    TIMER1,
    TIMER2,
    TIMER3,
    TIMER4,
    TIMER5,
    WTIMER0,
    WTIMER1,
    WTIMER2,
    WTIMER3,
    WTIMER4,
    WTIMER5,
};

enum TIMER_SUBMODULE {
    TIMER_A,
    TIMER_B
};

enum TIMER_MODE {
    TIMER_MODE_ONE_SHOT,
    TIMER_MODE_PERIODIC,
    TIMER_MODE_CAPTURE
};

enum TIMER_COUNT_DIR {
    TIMER_COUNT_DIR_UP,
    TIMER_COUNT_DIR_DOWN
};

enum DMA_CHANNEL {
    DMACH0,
    DMACH1,
    DMACH2,
    DMACH3,
    DMACH4,
    DMACH5,
    DMACH6,
    DMACH7,
    DMACH8,
    DMACH9,
    DMACH10,
    DMACH11,
    DMACH12,
    DMACH13,
    DMACH14,
    DMACH15,
    DMACH16,
    DMACH17,
    DMACH18,
    DMACH19,
    DMACH20,
    DMACH21,
    DMACH22,
    DMACH23,
    DMACH24,
    DMACH25,
    DMACH26,
    DMACH27,
    DMACH28,
    DMACH29,
    DMACH30,
    DMACH31
};

enum DMACHCTL_ADDRESS_INC {
    INC_BYTE,
    INC_HALFWORD,
    INC_WORD,
    INC_NONE
};

enum DMACHCTL_DATA_SIZE {
    SIZE_BYTE,
    SIZE_HALFWORD,
    SIZE_WORD
};

enum DMACHCTL_ARB_SIZE {
    ARBSIZE_1,
    ARBSIZE_2,
    ARBSIZE_4,
    ARBSIZE_8,
    ARBSIZE_16,
    ARBSIZE_32,
    ARBSIZE_64,
    ARBSIZE_128,
    ARBSIZE_256,
    ARBSIZE_512,
    ARBSIZE_1024,
};

enum DMACHCTL_NXTUSEBURSTMODE {
    BURSTMODE_DIS,
    BURSTMODE_EN
};

enum DMACHCTL_TRANSFER_MODE {
    TRANSFER_MODE_STOP,
    TRANSFER_MODE_BASIC,
    TRANSFER_MODE_AUTO,
    TRANSFER_MODE_PINGPONG,
    TRANSFER_MODE_MEMSCATGAT,
    TRANSFER_MODE_ALTSCATGAT,
    TRANSFER_MODE_PERIPHSCATGAT,
    TRANSFER_MODE_ALTPERIPHSCATGAT
};

struct GPIO_PIN{
    enum PORT port;
    enum PIN pin;
};

#endif /* _COMMON_H_ */
