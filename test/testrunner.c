#include "system.h"
#include "ssi.h"
#include "gpio.h"
#include "tm4c123gh6pm.h"
#include "st7735.h"
#include "enc28j60.h"
#include "enc.h"
#include "lcd.h"
#include "producer.h"
#include "consumer.h"
#include "ipv4.h"

extern void EnableInterrupts();
extern void DisableInterrupts();
extern void StartCritical();
extern void EndCritical();

int test1() {
    return 1;
}

int test_arp_send_many();

int (*test_cases[])(void) = {
    test_arp_send_many
};

LCD lcd;
ENC enc;

int main(void){

    DisableInterrupts();

    PLL_init();

    lcd_init(&lcd);
    enc_init(&enc);
    enc_enable(&enc);

    EnableInterrupts();

    unsigned int test_count = sizeof(test_cases) / 4;

    lcd_write(&lcd, "Running tests.\n");

    for (unsigned int i = 0; i < test_count; i++) {
        if (test_cases[i]() == 0) {
            lcd_write(&lcd, "TEST %d FAILED!\n", i);
        }
    }

    lcd_write(&lcd, "Done.\n");
    
    while (1)
        ;
}