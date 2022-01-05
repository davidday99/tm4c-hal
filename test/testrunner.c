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

int test_dma_mem();
int test_dma_ssi_rx();
int test_dma_ssi_tx();
int test_arp_send_many();
int test_rx_udp_packet();

int (*test_cases[])(void) = {
    test_dma_mem,
    test_dma_ssi_rx,
    test_dma_ssi_tx,
    test_arp_send_many,
    test_rx_udp_packet
};

LCD lcd;
ENC _enc;
ENC *enc = &_enc;


int main(void){

    DisableInterrupts();

    PLL_init();

    lcd_init(&lcd);
    enc_init(enc);
    enc_enable(enc);

    EnableInterrupts();

    unsigned int test_count = sizeof(test_cases) / 4;

    lcd_write(&lcd, "Running tests.\n");

    for (unsigned int i = 0; i < test_count; i++) {
        if (test_cases[i]() == 0) {
            lcd_write(&lcd, "TEST %d FAILED!\n", i);
        } else {
            lcd_write(&lcd, "Test %d passed.\n", i);
        }
    }

    lcd_write(&lcd, "Done.\n");
    
    while (1)
        ;
}