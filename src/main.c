#include "system.h"
#include "ssi.h"
#include "gpio.h"
#include "tm4c123gh6pm.h"
#include "st7735.h"
#include "enc28j60.h"
#include "enc.h"
#include "lcd.h"
#include "ip_util.h"
#include "event_queue.h"
#include <string.h>

extern void EnableInterrupts();
extern void DisableInterrupts();
extern void StartCritical();
extern void EndCritical();

LCD lcd;

int main(void){

    DisableInterrupts();

    PLL_init();

    ENC enc;
    lcd_init(&lcd);
    
    if (enc_init(&enc)) {
        lcd_write(&lcd, "ENC initialized.\n");
        ENC28J60_enable_receive(&ENC28J60) ? lcd_write(&lcd, "Receive enabled.\n") : 
                                    lcd_write(&lcd, "Could not enable.\n");
    } else {
        lcd_write(&lcd, "Could not init.\n");
    }

    EnableInterrupts();
    event_t e;

    while (1) {
        while  ((e = event_queue_pop()) != EVENT_QUEUE_EMPTY)
            event_queue_handle_event(e, &lcd, &enc);
    }
}