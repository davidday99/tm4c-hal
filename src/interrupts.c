#include <stdint.h>
#include "common.h"
#include "tm4c123gh6pm.h"
#include "enc28j60.h"
#include "lcd.h"

extern LCD lcd;
extern struct ENC28J60 *enc;

void BusFault_Handler(void) {
    while (1)
    {
        
    }
}

void GPIOPortB_ISR(void) {
    
}

void SPI1_ISR(void) {
   
}

void Timer0A_ISR(void) {
    TIMER0_ICR_R = 0;
}

void Timer1A_ISR(void) {
    TIMER0_ICR_R = 0;
}