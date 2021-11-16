#include "system.h"
#include "ssi.h"
#include "gpio.h"
#include "tm4c123gh6pm.h"
#include "st7735.h"
#include "enc28j60.h"

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

void print_line(char *s) {
    if (ST7735.y_pos == 15) {
        ST7735_OutString(&ST7735, s);
        ST7735_FillScreen(&ST7735, ST7735_BLACK);
    }
    ST7735_OutString(&ST7735, s);
}

void hex_to_str(uint32_t val, char *buf) {
    uint32_t i = 0;
    char copy[20];
    if (val == 0) {
        copy[i++] = '0';
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

    ST7735_init(&ST7735);
    ST7735_OutString(&ST7735, "Read value: \n");

    char buf[20];

    uint8_t read = ENC28J60_init(&ENC28J60);

    hex_to_str(read, buf);
    ST7735_OutString(&ST7735, buf);
    ST7735_OutString(&ST7735, "\n");

    while (1) {
        ST7735_OutString(&ST7735, ".");
        Delay();
        ST7735_OutString(&ST7735, ".");
        Delay();
        ST7735_OutString(&ST7735, ".");
        Delay();
        ST7735_OutChar(&ST7735, 8);
        ST7735_OutChar(&ST7735, 8);
        ST7735_OutChar(&ST7735, 8);
    }

}