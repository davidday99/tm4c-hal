#include "system.h"
#include "ssi.h"
#include "gpio.h"
#include "tm4c123gh6pm.h"
#include "st7735.h"
#include "enc28j60.h"
#include "enc.h"
#include "ethernet.h"
#include "lcd.h"
#include "netcommon.h"
#include "arp.h"
#include "ip_util.h"
#include "stdlib.h"
#include "event_queue.h"
#include "string.h"

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

// uint32_t strlen(const char *s) {
//     uint32_t len = 0;
//     while (*s++)
//         len++;
//     return len;
// }

LCD lcd;

void send_arp_request(LCD *lcd, ENC *enc);

void print_ethernet_frame(LCD *lcd, uint8_t *data, uint16_t len) {
    struct enethdr *hdr = (struct enethdr *) data;

    lcd_write(lcd, "des:");
    for (uint8_t i = 0; i < ENET_HEADER_DEST_LEN; i++)
        lcd_write(lcd, "%x%c", hdr->dest[i], i < ENET_HEADER_DEST_LEN - 1 ? '-' : ' ');
    lcd_write(lcd, "\n");

    lcd_write(lcd, "src:");
    for (uint8_t i = 0; i < ENET_HEADER_SRC_LEN; i++)
        lcd_write(lcd, "%x%c", hdr->src[i], i < ENET_HEADER_SRC_LEN - 1 ? '-' : ' ');
    lcd_write(lcd, "\n");

    lcd_write(lcd, "type:0x%X\n", hton16(hdr->type));

    lcd_write(lcd, "frame size:%d\n", len);    
}

int main(void){
    uint8_t delay = 0;

    DisableInterrupts();

    PLL_init();

    ENC enc;
    lcd_init(&lcd);

    uint8_t x[] = {1,2,3};
    uint8_t y[3] = {0,0,0};

    memcpy(y, x, 3);
    
    if (enc_init(&enc)) {
        lcd_write(&lcd, "ENC initialized.\n");
        lcd_write(&lcd, "Enabling receive.\n");
        ENC28J60_enable_receive(&ENC28J60) ? lcd_write(&lcd, "Receive enabled.\n") : 
                                    lcd_write(&lcd, "Could not enable.\n");
    } else {
        lcd_write(&lcd, "Could not init.\n");
    }

    EnableInterrupts();
    event_t e;

    while (1) {
        // send_arp_request(&lcd, &enc);
        // Delay1s();
        while  ((e = event_queue_pop()) != EVENT_QUEUE_EMPTY)
            event_queue_handle_event(e, &lcd, &enc);
    }
}