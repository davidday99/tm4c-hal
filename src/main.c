#include "system.h"
#include "ssi.h"
#include "gpio.h"
#include "tm4c123gh6pm.h"
#include "st7735.h"
#include "enc28j60.h"
#include "enc.h"
#include "ethernet.h"
#include "ip.h"
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

uint32_t strlen(const char *s) {
    uint32_t len = 0;
    while (*s++)
        len++;
    return len;
}

int main(void){
    uint8_t delay = 0;

    DisableInterrupts();

    PLL_init();

    struct enet_frame e_frame;
    struct ip_pkt ip;
    uint32_t len;


    LCD lcd;
    ENC enc;
    lcd_init(&lcd);

    uint8_t read = enc_init(&enc);
    
    if (read) {
        lcd_write(&lcd, "ENC initialized.\n");
        lcd_write(&lcd, "Enabling receive.\n");
        ENC28J60_enable_receive(&ENC28J60) ? lcd_write(&lcd, "Receive enabled.\n") : 
                                    lcd_write(&lcd, "Could not enable.\n");
    } else {
        lcd_write(&lcd, "Could not init.\n");
    }

    lcd_write(&lcd, "Waiting for frames.\n");
    
    uint8_t prev = 0;
    read = 0;
    while (1) {
        // ENC28J60_write_frame(&ENC28J60, transmit, 16);
        // read += 1;
        read = get_packet_count(&ENC28J60);
        if (prev != read) {
            prev = read;
            enc_read_frame(&enc, &e_frame);
            
            lcd_write(&lcd, "des:");
            for (uint8_t i = 0; i < ENET_DEST_LEN; i++)
                lcd_write(&lcd, "%x%c", e_frame.dest[i], i < ENET_DEST_LEN - 1 ? '-' : ' ');
            lcd_write(&lcd, "\n");

            lcd_write(&lcd, "src:");
            for (uint8_t i = 0; i < ENET_SRC_LEN; i++)
                lcd_write(&lcd, "%x%c", e_frame.src[i], i < ENET_SRC_LEN - 1 ? '-' : ' ');
            lcd_write(&lcd, "\n");

            lcd_write(&lcd, "type:0x%X\n", e_frame.type);

            lcd_write(&lcd, "frame size:%d\n", e_frame.dlen);            
        }
    }
}