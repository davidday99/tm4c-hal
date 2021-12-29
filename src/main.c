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

extern void EnableInterrupts();
extern void DisableInterrupts();
extern void StartCritical();
extern void EndCritical();

LCD lcd;
ENC enc;

uint8_t arp[] = {
    0xDE,
    0xAD,
    0xBE,
    0XEF,
    0xCC,
    0XCC,
    0XAB,
    0xCD,
    0xEF,
    0x12,
    0x34,
    0x56,
    0x08,
    0x00,
    0x00,
    0x01,
    0x08,
    0x00,
    0x06,
    0x04,
    0x00,
    0x01,
    0xDE,
    0xAD,
    0xBE,
    0XEF,
    0xCC,
    0XCC,
    0xC0,
    0xA8,
    0x00,
    0x6E,
    0XAB,
    0xCD,
    0xEF,
    0x12,
    0x34,
    0x56,
    0xC0,
    0xA8,
    0x00,
    0x01,
};

void Delay(uint32_t d) {
    uint32_t volatile delay = d * 80000;
    while (delay > 0)
        delay--;
}

int main(void){

    DisableInterrupts();

    PLL_init();

    lcd_init(&lcd);
    
    if (enc_init(&enc)) {
        lcd_write(&lcd, "ENC initialized.\n");
        ENC28J60_enable_receive(&ENC28J60) ? lcd_write(&lcd, "Receive enabled.\n") : 
                                    lcd_write(&lcd, "Could not enable.\n");
    } else {
        lcd_write(&lcd, "Could not init.\n");
    }

    EnableInterrupts();

    producer_event_t p_event;
    consumer_event_t c_event;

    while (1) {
        Delay(10);
        enc_write_frame(&enc, arp, 42);
        Delay(10);


        while ((p_event = producer_queue_pop()) != PRODUCER_QUEUE_EMPTY)
            producer_handle_event(p_event);
        
        while ((c_event = consumer_queue_pop()) != CONSUMER_QUEUE_EMPTY)
            consumer_handle_event(c_event);
    }
}