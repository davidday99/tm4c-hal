#include "system.h"
#include "ssi.h"
#include "gpio.h"
#include "tm4c123gh6pm.h"
#include "st7735.h"
#include "enc28j60.h"
#include "ethernet.h"
#include "lcd.h"

extern void EnableInterrupts();
extern void DisableInterrupts();
extern void StartCritical();
extern void EndCritical();

void Delay1s(void){
    unsigned long volatile time = 2400000;
    while(time){
        time--;
    }
}

uint8_t transmit[16] = {
    0xAB,
    0xCD,
    0xEF,
    0x01,
    0x02,
    0x03,
    0xC0,
    0x0C,
    0xC0,
    0x0C,
    0xC0,
    0x0C,
    0x00,
    0x08,
    0xFF,
    0xFF
};



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

    char buf[20];
    uint8_t frame[1518];
    uint32_t len;


    LCD lcd;
    lcd_init(&lcd);

    uint8_t read = ENC28J60_init(&ENC28J60);
    
    if (read) {
        lcd_write(&lcd, "ENC initialized.\n");
        lcd_write(&lcd, "Enabling receive.\n");
        ENC28J60_enable_receive(&ENC28J60) ? lcd_write(&lcd, "Receive enabled.\n") : 
                                    lcd_write(&lcd, "Could not enable.\n");
        ENC28J60_enable_loopback_mode(&ENC28J60);   
    } else {
        lcd_write(&lcd, "Could not init.\n");
    }

    lcd_write(&lcd, "Waiting for frames.\n");
    
    uint8_t prev = 0;
    while (1) {
        ENC28J60_write_frame(&ENC28J60, transmit, 16);
        read += 1;
        // read = get_packet_count(&ENC28J60);
        if (prev == read) {
            lcd_write(&lcd, ".");
            Delay1s();
            lcd_write(&lcd, ".");
            Delay1s();
            lcd_write(&lcd, ".");
            Delay1s();
            lcd_write(&lcd, "%b", 8);
            lcd_write(&lcd, "%b", 8);
            lcd_write(&lcd, "%b", 8);
        } else {
            hex_to_str(read, buf);
            prev = read;
            lcd_write(&lcd, "Frame %d received!\n", read);
            len = ENC28J60_read_frame(&ENC28J60);
        }
    }
}