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

void print_arp_packet(LCD *lcd, struct arphdr *hdr) {
    char ipaddr[16];
    lcd_write(lcd, "HW Type: %d\n", hton16(hdr->hwtype));
    lcd_write(lcd, "Pcol Type: 0x%X\n", hton16(hdr->ptype));
    lcd_write(lcd, "HW Len: %d\n", hdr->hwlen);
    lcd_write(lcd, "Pcol Len: %d\n", hdr->plen);
    lcd_write(lcd, "Opcode: %d\n", hton16(hdr->opcode));

    int_to_ipv4(hton32(hdr->psender), ipaddr);
    lcd_write(lcd, "Sender IP:\n");
    lcd_write(lcd, ipaddr);
    lcd_write(lcd, "\n");


    lcd_write(lcd, "Sender MAC:\n");
    for (uint8_t i = 0; i < ENET_HEADER_DEST_LEN; i++)
        lcd_write(lcd, "%x%c", hdr->hwsender[i], i < ENET_HEADER_DEST_LEN - 1 ? '-' : ' ');
    lcd_write(lcd, "\n");

    int_to_ipv4(hton32(hdr->ptarget), ipaddr);
    lcd_write(lcd, "Target IP:\n");
    lcd_write(lcd, ipaddr);
    lcd_write(lcd, "\n");

    lcd_write(lcd, "Target MAC:\n");
    for (uint8_t i = 0; i < ENET_HEADER_DEST_LEN; i++)
        lcd_write(lcd, "%x%c", hdr->hwtarget[i], i < ENET_HEADER_DEST_LEN - 1 ? '-' : ' ');
    lcd_write(lcd, "\n");  
}

int main(void){
    uint8_t delay = 0;

    DisableInterrupts();

    PLL_init();

    LCD lcd;
    ENC enc;
    lcd_init(&lcd);
    
    if (enc_init(&enc)) {
        lcd_write(&lcd, "ENC initialized.\n");
        lcd_write(&lcd, "Enabling receive.\n");
        ENC28J60_enable_receive(&ENC28J60) ? lcd_write(&lcd, "Receive enabled.\n") : 
                                    lcd_write(&lcd, "Could not enable.\n");
    } else {
        lcd_write(&lcd, "Could not init.\n");
    }

    lcd_write(&lcd, "Waiting for frames.\n");
    
    uint8_t pktcnt = 0;
    uint16_t len;
    while (1) {
        Delay1s();
        pktcnt = get_packet_count(&ENC28J60);
        if (pktcnt > 0) {
            len = enc_read_frame(&enc);
            // ethernet_handle_frame(enc_rx_buffer);
            // print_ethernet_frame(&lcd, enc_rx_buffer, len);    
            if (hton16(((struct enethdr *) enc_rx_buffer)->type) == ETHERTYPE_ARP)
                print_arp_packet(&lcd, (struct arphdr *) (enc_rx_buffer + ENET_DATA_OFFSET));
        }
    }
}