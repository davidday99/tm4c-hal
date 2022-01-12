#include "system.h"
#include "ssi.h"
#include "gpio.h"
#include "tm4c123gh6pm.h"
#include "st7735.h"
#include "enc28j60.h"
#include "lcd.h"

extern void EnableInterrupts();
extern void DisableInterrupts();
extern void StartCritical();
extern void EndCritical();

int test_dummy() {
    return 1;
}

int (*test_cases[])(void) = {
    test_dummy
};

LCD _lcd;
LCD *lcd = &_lcd; 

int main(void){

    DisableInterrupts();

    PLL_init();

    lcd_init(lcd);

    EnableInterrupts();

    unsigned int test_count = sizeof(test_cases) / 4;

    int passing = 0;
    for (unsigned int i = 0; i < test_count; i++) {
        int result = test_cases[i]();
        passing += result;
        if (result)
            lcd_write(lcd, "Test %d passed.\n");
        else
            lcd_write(lcd, "TEST %d FAILED!\n");
    }
    lcd_write(lcd, "Done.\n");
    lcd_write(lcd, "%d/%d passed.\n", passing, test_count);

    while (1)
        ;
}