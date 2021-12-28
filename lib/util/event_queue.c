#include <stdint.h>
#include <stdarg.h>
#include "event_queue.h"
#include "lcd.h"
#include "enc.h"
#include "ip_util.h"
#include "ipv4.h"
#include <string.h>

#define EVENT_QUEUE_LEN 100

struct event {
    event_t eventid;
};

static struct event event_queue[EVENT_QUEUE_LEN];

static uint8_t event_queue_rdptr;
static uint8_t event_queue_wrptr;

// void print_arp_packet(LCD *lcd, struct arphdr *hdr);
 
// static void handle_ethernet_receive(LCD *lcd, ENC *enc);
// static void handle_ethernet_frame_waiting(LCD *lcd, ENC *enc);
void handle_arp(LCD *lcd, ENC *enc);
// static void handle_ipv4(LCD *lcd, ENC *enc);

uint8_t event_queue_push(event_t event) {
    if (event_queue_wrptr == event_queue_rdptr - 1)
        return 0;
    event_queue[event_queue_wrptr].eventid = event;
    event_queue_wrptr = (event_queue_wrptr + 1) % EVENT_QUEUE_LEN;
    
    uint8_t x[] = {1,2,3};
    uint8_t y[] = {0,0,0};

    memcpy(y, x, 3);


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

    static uint8_t cnt;

    switch (eventid) {
        case EVENT_QUEUE_EMPTY:
            lcd_write(lcd, "QUEUE EMPTY\n");
            break;
        case EVENT_QUEUE_FULL:
            lcd_write(lcd, "QUEUE FULL\n");
            break;
        case EVENT_ETHERNET_RECEIVE:
            lcd_write(lcd, "%d: ENET RX EVENT\n", ++cnt);
            enc_clear_interrupt_flag();
            enc_read_frame(enc);
            // handle_ethernet_receive(lcd, enc);
            break;
        case EVENT_ETHERNET_FRAME_WAITING:
            // handle_ethernet_frame_waiting(lcd, enc);
        case EVENT_LCD_WRITE:
            lcd_write(lcd, "LCD WRITE EVENT\n");
            break;
        case EVENT_ARP_RECEIVE:
            lcd_write(lcd, "ARP RECEIVE EVENT\n");
            // handle_arp(lcd, enc);
            break;
        case EVENT_IPV4_RECEIVE:
            lcd_write(lcd, "IPV4 RECEIVE EVENT\n");
            // handle_ipv4(lcd, enc);
            break;
        case EVENT_IPV6_RECEIVE:
            lcd_write(lcd, "IPV6 RECEIVE EVENT\n");
            break;
        default:
            lcd_write(lcd, "UNKNOWN EVENT\n");
            break;
    }
}

// static void handle_ethernet_receive(LCD *lcd, ENC *enc) {
//     enc_clear_interrupt_flag();
//     enc_read_frame(enc);
//     if (hton16(((struct enethdr *) enc_rx_buffer)->type) == ETHERTYPE_ARP)
//         event_queue_push(EVENT_ARP_RECEIVE);
//     if (hton16(((struct enethdr *) enc_rx_buffer)->type) == ETHERTYPE_IPV4)
//         event_queue_push(EVENT_IPV4_RECEIVE);
//     if (hton16(((struct enethdr *) enc_rx_buffer)->type) == ETHERTYPE_IPV6)
//         event_queue_push(EVENT_IPV6_RECEIVE);
// }

// static void handle_ethernet_frame_waiting(LCD *lcd, ENC *enc) {
//     enc_clear_interrupt_flag();
//     /* This function needs to do more than this. It needs to check exactly how many 
//         packets have been missed since the interrupt was triggered and push an
//         ETHERNET_RECEIVE event for each one.
//     */
// }

// void handle_arp(LCD *lcd, ENC *enc) {
//     // print_arp_packet(lcd, (struct arphdr *) (enc_rx_buffer + ENET_DATA_OFFSET));
//     uint8_t arppkt[ENET_HEADER_SIZE + ARP_SIZE];
//     struct arphdr *arptr = (struct arphdr *) (arppkt + ENET_DATA_OFFSET);
//     struct enethdr *enetptr = (struct enethdr *) arppkt;
//     memcopy(enetptr->dest, ((struct enethdr *) enc_rx_buffer)->src, ENET_HEADER_DEST_LEN);
//     memcopy(enetptr->src, enc->mac, ENET_HEADER_SRC_LEN);
//     enetptr->type = hton16(ETHERTYPE_ARP);
//     arp_reply((struct arphdr *) (arppkt + ENET_DATA_OFFSET), (struct arphdr *) (enc_rx_buffer + ENET_DATA_OFFSET));
//     memcopy(arptr->hwsender, enc->mac, ENET_HEADER_SRC_LEN);
//     // enc_write_frame(enc, arppkt, sizeof(arppkt));
//     enc_acknowledge_frame();
//     enc_frame_waiting = 0;
// }

// void send_arp_request(LCD *lcd, ENC *enc) {
//     uint8_t arppkt[ENET_HEADER_SIZE + ARP_SIZE];
//     struct arphdr *arptr = (struct arphdr *) (arppkt + ENET_DATA_OFFSET);
//     struct enethdr *enetptr = (struct enethdr *) arppkt;
//     uint8_t dest_mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xCC, 0xCC};
//     uint32_t dest_ip = 0xC0A80001;  // 192.168.0.1
//     uint32_t src_ip = 0xC0A8006F; // 192.168.0.111
//     memcopy(enetptr->dest, dest_mac, ENET_HEADER_DEST_LEN);
//     memcopy(enetptr->src, enc->mac, ENET_HEADER_SRC_LEN);
//     enetptr->type = hton16(ETHERTYPE_ARP);
//     arp_request((struct arphdr *) (arppkt + ENET_DATA_OFFSET), enc->mac, src_ip, dest_mac, dest_ip);
//     memcopy(arptr->hwsender, enc->mac, ENET_HEADER_SRC_LEN);
//     enc_write_frame(enc, arppkt, sizeof(arppkt));
//     enc_acknowledge_frame();
//     enc_frame_waiting = 0;
// }


// static void handle_ipv4(LCD *lcd, ENC *enc) {
//     struct ipv4hdr *iptr = (struct ipv4hdr *) (enc_rx_buffer + ENET_DATA_OFFSET);
//     char ipaddr[16];
//     lcd_write(lcd, "Version: %d\n", iptr->version);
//     lcd_write(lcd, "IHL: %d\n", iptr->ihl);
//     lcd_write(lcd, "TOS: %d\n", iptr->tos);
//     lcd_write(lcd, "Length: %d\n", hton16(iptr->len));
//     lcd_write(lcd, "ID: %d\n", hton16(iptr->id));
//     lcd_write(lcd, "Flags: %d\n", (hton16(iptr->frag_offset) >> 13) & 7);
//     lcd_write(lcd, "Offset: %d\n", hton16(iptr->frag_offset) & 0x1FFF);
//     lcd_write(lcd, "TTL: %d\n", iptr->ttl);
//     lcd_write(lcd, "Protocol: %d\n", iptr->protocol);
//     lcd_write(lcd, "Checksum: %d\n", hton16(iptr->cksm));
//     int_to_ipv4(hton32(iptr->src), ipaddr);
//     lcd_write(lcd, "Sender IP:\n");
//     lcd_write(lcd, ipaddr);
//     lcd_write(lcd, "\n");
//     int_to_ipv4(hton32(iptr->dest), ipaddr);
//     lcd_write(lcd, "Dest. IP:\n");
//     lcd_write(lcd, ipaddr);
//     lcd_write(lcd, "\n");
// }

// void print_arp_packet(LCD *lcd, struct arphdr *hdr) {
//     char ipaddr[16];
//     lcd_write(lcd, "HW Type: %d\n", hton16(hdr->hwtype));
//     lcd_write(lcd, "Pcol Type: 0x%X\n", hton16(hdr->ptype));
//     lcd_write(lcd, "HW Len: %d\n", hdr->hwlen);
//     lcd_write(lcd, "Pcol Len: %d\n", hdr->plen);
//     lcd_write(lcd, "Opcode: %d\n", hton16(hdr->opcode));

//     int_to_ipv4(hton32(hdr->psender), ipaddr);
//     lcd_write(lcd, "Sender IP:\n");
//     lcd_write(lcd, ipaddr);
//     lcd_write(lcd, "\n");


//     lcd_write(lcd, "Sender MAC:\n");
//     for (uint8_t i = 0; i < ENET_HEADER_DEST_LEN; i++)
//         lcd_write(lcd, "%x%c", hdr->hwsender[i], i < ENET_HEADER_DEST_LEN - 1 ? '-' : ' ');
//     lcd_write(lcd, "\n");

//     int_to_ipv4(hton32(hdr->ptarget), ipaddr);
//     lcd_write(lcd, "Target IP:\n");
//     lcd_write(lcd, ipaddr);
//     lcd_write(lcd, "\n");

//     lcd_write(lcd, "Target MAC:\n");
//     for (uint8_t i = 0; i < ENET_HEADER_DEST_LEN; i++)
//         lcd_write(lcd, "%x%c", hdr->hwtarget[i], i < ENET_HEADER_DEST_LEN - 1 ? '-' : ' ');
//     lcd_write(lcd, "\n");  
// }