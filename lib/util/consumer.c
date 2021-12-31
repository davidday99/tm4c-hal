#include <stdint.h>
#include "consumer.h"
#include "enc.h"
#include "ethernet.h"
#include "arp.h"
#include "netcommon.h"
#include "lcd.h"
#include "string.h"
#include "ipv4.h"
#include "tcp.h"
#include "udp.h"

extern LCD lcd;

static struct consumer_event consumer_queue[CONSUMER_EVENT_QUEUE_LEN] ;

static uint8_t consumer_rdidx;
static uint8_t consumer_wridx;

extern ENC enc;

static void handle_read_ethernet_frame();
static void handle_ipv4_event();
static void print_ipv4_header();
static void print_udp_header();

uint8_t consumer_queue_push(consumer_event_t event) {
    if (consumer_wridx == consumer_rdidx - 1)
        return 0;
    consumer_queue[consumer_wridx].eventid = event;
    consumer_wridx = (consumer_wridx + 1) % CONSUMER_EVENT_QUEUE_LEN;
    return 1;
}

consumer_event_t consumer_queue_pop(void) {
    if (consumer_rdidx == consumer_wridx)
        return CONSUMER_QUEUE_EMPTY;

    consumer_event_t e = consumer_queue[consumer_rdidx].eventid;
    consumer_rdidx = (consumer_rdidx + 1) % CONSUMER_EVENT_QUEUE_LEN;
    return e;
}

void consumer_handle_event(consumer_event_t event) {
    switch (event) {
        case CONSUMER_QUEUE_EMPTY:
            break;
        case CONSUMER_QUEUE_FULL:
            break;
        case CONSUMER_READ_ETHERNET_FRAME:
            handle_read_ethernet_frame();
            break;
        case CONSUMER_ETHERNET_WAITING:
            break;
        case CONSUMER_ARP_RECEIVE:
            lcd_write(&lcd, "ARP RECEIVED\n");
            break;
        case CONSUMER_IPV4_RECEIVE:
            lcd_write(&lcd, "IPV4 RECEIVED\n");
            handle_ipv4_event();
            break;
        case CONSUMER_IPV6_RECEIVE:
            lcd_write(&lcd, "IPV6 RECEIVED\n");
            break;
        case CONSUMER_TCP_RECEIVE:
            lcd_write(&lcd, "TCP RECEIVED\n");
            break;
        case CONSUMER_UDP_RECEIVE:
            lcd_write(&lcd, "UDP RECEIVED\n");
            print_udp_header();
            break;
        default:
            break;
    }
}

static void handle_read_ethernet_frame() {
    enc_clear_interrupt_flag();
    enc_read_frame(&enc);

    if (hton16(((struct enethdr *) enc_rx_buffer)->type) == ETHERTYPE_ARP)
        consumer_queue_push(CONSUMER_ARP_RECEIVE);
    if (hton16(((struct enethdr *) enc_rx_buffer)->type) == ETHERTYPE_IPV4)
        consumer_queue_push(CONSUMER_IPV4_RECEIVE);
    if (hton16(((struct enethdr *) enc_rx_buffer)->type) == ETHERTYPE_IPV6)
        consumer_queue_push(CONSUMER_IPV6_RECEIVE);
}

static void handle_arp_event() {
    uint8_t arppkt[ENET_HEADER_SIZE + ARP_SIZE];
    struct arphdr *arptr = (struct arphdr *) (arppkt + ENET_DATA_OFFSET);
    struct enethdr *enetptr = (struct enethdr *) arppkt;
    memcpy(enetptr->dest, ((struct enethdr *) enc_rx_buffer)->src, ENET_HEADER_DEST_LEN);
    memcpy(enetptr->src, enc.mac, ENET_HEADER_SRC_LEN);
    enetptr->type = hton16(ETHERTYPE_ARP);
    // arp_reply((struct arphdr *) (arppkt + ENET_DATA_OFFSET), (struct arphdr *) (enc_rx_buffer + ENET_DATA_OFFSET));
    memcpy(arptr->hwsender, enc.mac, ENET_HEADER_SRC_LEN);
    enc_write_frame(&enc, arppkt, sizeof(arppkt));
    enc_acknowledge_frame();
    enc_frame_waiting = 0; 
}

static void handle_ipv4_event() {
    struct ipv4hdr *hdr = enc_rx_buffer + ENET_DATA_OFFSET;
    // print_ipv4_header();

    if (hdr->protocol == IPV4_PROTOCOL_TCP)
        consumer_queue_push(CONSUMER_TCP_RECEIVE);
    if (hdr->protocol == IPV4_PROTOCOL_UDP)
        consumer_queue_push(CONSUMER_UDP_RECEIVE);
}

static void print_ipv4_header() {
    struct ipv4hdr *hdr = enc_rx_buffer + ENET_DATA_OFFSET;    
    lcd_write(&lcd, "src ip: %d\n", hdr->src);
    lcd_write(&lcd, "dest ip: %d\n", hdr->dest);
    lcd_write(&lcd, "protocol: %d\n", hdr->protocol);
    lcd_write(&lcd, "header len: %d\n", hdr->ihl);
    lcd_write(&lcd, "total len: %d\n", hdr->len);
    lcd_write(&lcd, "version: %d\n", hdr->version);
    lcd_write(&lcd, "ttl: %d\n", hdr->ttl);
    lcd_write(&lcd, "checksum: %d\n", hdr->cksm);
}

void print_tcp_header() {
    struct ipv4hdr *iphdr = enc_rx_buffer + ENET_DATA_OFFSET;
    struct tcphdr *hdr = iphdr + IPV4_MIN_HEADER_LEN + ipv4_options_len(iphdr);
    
    lcd_write(&lcd, "src port: %d\n", hdr->srcport);
    lcd_write(&lcd, "dest port: %d\n", hdr->destport);
    lcd_write(&lcd, "seq num: %d\n", hdr->seqnum);
    lcd_write(&lcd, "ack num: %d\n", hdr->acknum);
    lcd_write(&lcd, "flags: %d\n", hdr->flags);
    lcd_write(&lcd, "window: %d\n", hdr->window);
    lcd_write(&lcd, "checksum: %d\n", hdr->chksm);
    lcd_write(&lcd, "urgent ptr: %d\n", hdr->urgentptr);
}

static void print_udp_header() {
    struct ipv4hdr *iphdr = (struct ipv4hdr *) (enc_rx_buffer + ENET_DATA_OFFSET);
    struct udphdr *hdr = (struct udphdr *) (enc_rx_buffer + ENET_DATA_OFFSET + IPV4_MIN_HEADER_LEN + ipv4_options_len(iphdr));
    char line[22];
    
    lcd_write(&lcd, "src port: %d\n", hton16(hdr->srcport));
    lcd_write(&lcd, "dest port: %d\n", hton16(hdr->destport));
    lcd_write(&lcd, "len: %d\n", hton16(hdr->len));
    lcd_write(&lcd, "checksum: %d\n", hton16(hdr->cksm));

    uint16_t dlen = hton16(hdr->len) - sizeof(struct udphdr);
    uint8_t *dptr = (uint8_t *) (((uint32_t) hdr) + sizeof(struct udphdr));

    while (dlen > 20) {
        memcpy(line, dptr, 20);
        line[20] = '\n';
        line[21] = '\0'; 
        dptr += 20;
        dlen -= 20;
        lcd_write(&lcd, line);
    }
    memcpy(line, dptr, dlen);
    line[dlen] = '\n';
    line[dlen + 1] = '\0';
    lcd_write(&lcd, line);
}