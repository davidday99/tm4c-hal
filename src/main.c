
#include "system.h"
#include "ssi.h"
#include "gpio.h"
#include "tm4c123gh6pm.h"
#include "st7735.h"

extern void EnableInterrupts();
extern void DisableInterrupts();
extern void StartCritical();
extern void EndCritical();

void Delay(void){unsigned long volatile time;
    time = 8000000;
    while(time){
        time--;
    }
}

char HEX_CONV[] = "abcdef";

void hex_to_str(uint32_t val, char *buf) {
    uint32_t i = 0;
    char copy[20];
    if (val == 0) {
        copy[i++] = 0;
    } else {
        while (val > 0) {
            uint8_t tmp = (val & 0xF);
            if (tmp > 9) {
                tmp = HEX_CONV[tmp - 10];
            } else {
                tmp += '0';
            }
            copy[i++] = tmp;
            val >>= 4;
        }
    }
    uint32_t j = 2;
    buf[0] = '0';
    buf[1] = 'x';
    while (i > 0) {
        buf[j++] = copy[--i];
    }
    buf[j] = '\0';
}

int main(void){
    uint8_t delay = 0;

    DisableInterrupts();

    PLL_init();

    Output_Init();
    ST7735_OutString("Hello, world!\n");
    Delay();

    ST7735_OutString("Address of i:\n");
    char buf[20];
    hex_to_str((uint32_t) &buf, buf);
    ST7735_OutString(buf);
    ST7735_OutString("\n");

    while (1) {

    }

}