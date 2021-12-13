#include <stdint.h>
#include <stdarg.h>
#include "event_queue.h"
#include "ethernet.h"
#include "lcd.h"
#include "enc.h"
#include "arp.h"
#include "netcommon.h"
#include "ip_util.h"
#include "stdlib.h"

#define EVENT_QUEUE_LEN 100

struct event {
    event_t eventid;
};

static struct event event_queue[EVENT_QUEUE_LEN];

static uint8_t event_queue_rdptr;
static uint8_t event_queue_wrptr;

void print_arp_packet(LCD *lcd, struct arphdr *hdr);
static void handle_arp(LCD *lcd, ENC *enc);

uint8_t event_queue_push(event_t event) {
    if (event_queue_wrptr == event_queue_rdptr - 1)
        return 0;
    event_queue[event_queue_wrptr].eventid = event;
    event_queue_wrptr = (event_queue_wrptr + 1) % EVENT_QUEUE_LEN;
    return 1;
}

event_t event_queue_pop(void) {
    if (event_queue_rdptr == event_queue_wrptr)
        return EVENT_QUEUE_EMPTY;

    event_t e = event_queue[event_queue_rdptr].eventid;
    event_queue_rdptr = (event_queue_rdptr + 1) % EVENT_QUEUE_LEN;
    return e;
}

void event_queue_handle_event(event_t eventid, ...) {
    va_list list;
    uint32_t val;
    va_start(list, eventid);
    LCD *lcd = va_arg(list, LCD *);
    ENC *enc = va_arg(list, ENC *);


    switch (eventid) {
        case EVENT_QUEUE_EMPTY:
            lcd_write(lcd, "QUEUE EMPTY\n");
            break;
        case EVENT_QUEUE_FULL:
            lcd_write(lcd, "QUEUE FULL\n");
            break;
        case EVENT_ETHERNET_RECEIVE:
            lcd_write(lcd, "ENET RECEIVE EVENT\n");
            enc_read_frame(enc);
            if (hton16(((struct enethdr *) enc_rx_buffer)->type) == ETHERTYPE_ARP)
                event_queue_push(EVENT_ARP_RECEIVE);
            break;
        case EVENT_LCD_WRITE:
            lcd_write(lcd, "LCD WRITE EVENT\n");
            break;
        case EVENT_ARP_RECEIVE:
            lcd_write(lcd, "ARP RECEIVE EVENT\n");
            handle_arp(lcd, enc);
            break;
        default:
            lcd_write(lcd, "UNKNOWN EVENT\n");
            break;
    }
}

static void handle_arp(LCD *lcd, ENC *enc) {
    uint8_t arppkt[ENET_HEADER_SIZE + ARP_SIZE];
    struct arphdr *arptr = (struct arphdr *) (arppkt + ENET_DATA_OFFSET);
    struct enethdr *enetptr = (struct enethdr *) arppkt;
    if (hton16(((struct enethdr *) enc_rx_buffer)->type) == ETHERTYPE_ARP)
        print_arp_packet(lcd, (struct arphdr *) (enc_rx_buffer + ENET_DATA_OFFSET));
        memcopy(enetptr->dest, ((struct enethdr *) enc_rx_buffer)->src, ENET_HEADER_DEST_LEN);
        memcopy(enetptr->src, (uint8_t[]) {0xA0, 0xCD, 0xEF, 0x12, 0x34, 0x56}, ENET_HEADER_SRC_LEN);
        enetptr->type = hton16(ETHERTYPE_ARP);
        arp_reply(arppkt + ENET_DATA_OFFSET, (enc_rx_buffer + ENET_DATA_OFFSET), enetptr->dest);
        memcopy(arptr->hwtarget, enetptr->dest, ENET_HEADER_SRC_LEN);
        memcopy(arptr->hwsender, (uint8_t[]) {0xA0, 0xCD, 0xEF, 0x12, 0x34, 0x56}, ENET_HEADER_SRC_LEN);
        arptr->ptarget = arptr->psender;
        arptr->psender = hton32(0xC0A8006F);
        enc_write_frame(enc, arppkt, sizeof(arppkt));
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