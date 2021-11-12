#include "tm4c123gh6pm.h"

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


void set_RCGCGPIO(uint8_t bit, uint8_t value) {
    SYSCTL_RCGCGPIO_R = (SYSCTL_RCGCGPIO_R & ~(1 << bit)) |
                            (value << bit);
}