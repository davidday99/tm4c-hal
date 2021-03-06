#include "tm4c123gh6pm.h"
#include "system.h"
#include "common.h"

void PLL_init(void) {
    SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;
    SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;
    SYSCTL_RCC_R &= ~SYSCTL_RCC_USESYSDIV;
    SYSCTL_RCC_R = (SYSCTL_RCC_R & ~SYSCTL_RCC_XTAL_M) + 
                    SYSCTL_RCC_XTAL_16MHZ;
    SYSCTL_RCC2_R = SYSCTL_RCC2_R & ~SYSCTL_RCC2_OSCSRC2_M;
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2    ;
    SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;          
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R & 
                    ~(SYSCTL_RCC2_SYSDIV2_M | SYSCTL_RCC2_SYSDIV2LSB)) +
                    (4 << 22);
    while ((SYSCTL_RIS_R & SYSCTL_RIS_PLLLRIS) == 0)
        ;
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;
}

static void delay(void) {
    uint8_t delay = 0;
    delay++;
}

void set_RCGCGPIO(enum PORT port, uint8_t value) {
    SYSCTL_RCGCGPIO_R = (SYSCTL_RCGCGPIO_R & ~(1 << port)) |
                            (value << port);
    delay();
}

void set_RCGCSSI(enum SSI_MODULE module, uint8_t value) {
    SYSCTL_RCGCSSI_R = (SYSCTL_RCGCSSI_R & ~(1 << module)) |
                            (value << module);
    delay();
}

void set_RCGCTIMER(enum TIMER_MODULE module, uint8_t value) {
    SYSCTL_RCGCTIMER_R = (SYSCTL_RCGCTIMER_R & ~(1 << module)) |
                            (value << module);
    delay();
}

void set_RCGCWTIMER(enum TIMER_MODULE module, uint8_t value) {
    SYSCTL_RCGCWTIMER_R = (SYSCTL_RCGCWTIMER_R & ~(1 << module)) |
                            (value << module);
    delay();
}

void set_RCGCDMA(void) {
    SYSCTL_RCGCDMA_R |= 1;
    delay();
}

void set_NVIC_EN0_bit(uint32_t bit) {
    NVIC_EN0_R |= (1 << bit);
}

void clear_NVIC_EN0_bit(uint32_t bit) {
    NVIC_EN0_R &= ~(1 << bit);
}