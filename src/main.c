#include "system.h"
#include "ssi.h"
#include "gpio.h"
#include "tm4c123gh6pm.h"
#include "st7735.h"
#include "enc28j60.h"
#include "lcd.h"
#include "ipv4.h"
#include "socket.h"

extern void EnableInterrupts();
extern void DisableInterrupts();
extern void StartCritical();
extern void EndCritical();

LCD lcd;
struct ENC28J60 *enc = &ENC28J60;

uint8_t ipv4_packet[] = {
    0xB4,
    0x2E,
    0x99,
    0XEC,
    0x02,
    0XC5,  // dest mac
    0XAB,
    0xCD,
    0xEF,
    0x12,
    0x34,
    0x56,  // src mac
    0x08,
    0x00,  // ether type IP
    0x45,  // version 4, IHL 5 (20 bytes header + 0 bytes options),
    0x01,  // TOS 1 (not sure what typical values are for this field)
    0x00,
    0x1E,  // Total len, 30 bytes (20 bytes header + 0 bytes options + 10 bytes data)
    0xAB,
    0XCD,  // ID, 0xABCD
    0x4F,
    0xAA,  // flags = 010, offset = 0b0111110101010
    0x40,  // TTL, 64
    0x11,  // Protocol, 0x11 (UDP)
    0xAB,
    0xCD,  // Header checksum
    0xC0,
    0xA8,
    0x00,
    0x6F,  // Src ip, 192.168.0.111
    0xC0,
    0xA8,
    0x00,
    0x01,  // Dest ip, 192168.0.1
    0xC0,
    0xC1,   // udp payload, src port
    0x00,
    0x50,  // dest port
    0x00,
    0x0A,  // udp header plus data length
    0xAB,
    0xCD,  // dummy checksum
    'h',
    'i'  // 2 bytes of data
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
    
    if (ENC28J60_init(enc)) {
        lcd_write(&lcd, "ENC initialized.\n");
        ENC28J60_enable_receive(enc) ? lcd_write(&lcd, "Receive enabled.\n") : 
                                    lcd_write(&lcd, "Could not enable.\n");
    } else {
        lcd_write(&lcd, "Could not init.\n");
    }

    struct socket *sock = socket_init(SOCKTYPE_UDP);
    struct socket_addr sockaddr = {SOCKADDR_IP_ANY, 8000};
    socket_bind(sock, &sockaddr);
    uint8_t data[64];

    EnableInterrupts();

    while (1) {
        socket_read(sock, &sockaddr, data, 64);
        if (data[0] != '\0') {
            lcd_write(&lcd, (char *) data);
            lcd_write(&lcd, "\n");
            data[0] = '\0';
        }
    }
}