#include <stdint.h>
#include "enc28j60.h"
#include "ssi_module.h"
#include "gpio_module.h"
#include "tm4c123gh6pm.h"

#define RCR_OPCODE 0
#define RBM_OPCODE 0x20
#define WCR_OPCODE 0x40
#define WBM_OPCODE 0x60
#define BFS_OPCODE 0x80
#define BFC_OPCODE 0x50
#define SRC_OPCODE 0xE0

#define RBM_ARG0 0x1A
#define WBM_ARG0 0x1A
#define SRC_ARG0 0x1F

#define ERDPTL 0
#define ERDPTH 0x01
#define EWRPTL 0x02
#define EWRPTH 0x03
#define ETXSTL 0x04
#define ETXSTH 0x05
#define ETXNDL 0x06
#define ETXNDH 0x07
#define ERXSTL 0x08
#define ERXSTH 0x09
#define ERXNDL 0xA
#define ERXNDH 0xB
#define ERXRDPTL 0xC
#define ERXRDPTH 0xD
#define ERXWRPTL 0xE
#define ERXWRPTH 0xF
#define EDMASTL 0x10
#define EDMASTH 0x11
#define EDMANDL 0x12
#define EDMANDH 0x13
#define EDMADSTL 0x14
#define EDMADSTH 0x15
#define EDMACSL 0x16
#define EDMACSH 0x17
#define EIE 0x1B
#define EIR 0x1C
#define ESTAT 0x1D
#define ECON2 0x1E
#define ECON1 0x1F

#define EHT0 0
#define EHT1 0x01
#define EHT2 0x02
#define EHT3 0x03
#define EHT4 0x04
#define EHT5 0x05
#define EHT6 0x06
#define EHT7 0x07
#define EPMM0 0x08
#define EPMM1 0x09
#define EPMM2 0xA
#define EPMM3 0xB
#define EPMM4 0xC
#define EPMM5 0xD
#define EPMM6 0xE
#define EPMM7 0xF
#define EPMMCSL 0x10
#define EPMMCSH 0x11
#define EPMOL 0x14
#define EPMOH 0x15
#define ERXFCON 0x18
#define EPKTCNT 0x19

#define MACON1 0
#define MACON3 0x02
#define MACON4 0x03
#define MABBIPG 0x04
#define MAIPGL 0x06
#define MAIPGH 0x07
#define MACLCON1 0x08
#define MACLCON2 0x09
#define MAMXFLL 0xA
#define MAMXFLH 0xB
#define MICMD 0x12
#define MIREGADR 0x14
#define MIWRL 0x16
#define MIWRH 0x17
#define MIRDL 0x18
#define MIRDH 0x19

#define MAADR5 0
#define MAADR6 0x01
#define MAADR3 0x02
#define MAADR4 0x03
#define MAADR1 0x04
#define MAADR2 0x05
#define EBSTSD 0x06
#define EBSTCON 0x07
#define EBSTCSL 0x08
#define EBSTCSH 0x09
#define MISTAT 0xA
#define EREVID 0x12
#define ECOCON 0x15
#define EFLOCON 0x17
#define EPAUSL 0x18
#define EPAUSH 0x19


struct ENC28J60 ENC28J60 = {
    &SSI_1,
    &PORTB_PIN0
};

void delay_50ns(uint32_t n) {
    uint8_t volatile delay = 40;
    while (n) {
        while (delay--)
            ;
        n--;
    }
}

static uint8_t read_control_register(struct ENC28J60 *enc28j60, uint8_t reg) {
    reg = (reg & 0x1F) | RCR_OPCODE;
    uint16_t dummy[2];
    dummy[0] = (uint16_t) reg;
    dummy[1] = 0;
    uint16_t data[2];
    set_gpio_pin_low(enc28j60->cs);
    write_ssi(enc28j60->ssi, dummy, 2);
    read_ssi(enc28j60->ssi, data, 2);
    set_gpio_pin_high(enc28j60->cs);
    return data[1];
}

static void read_buffer_memory(struct ENC28J60 *enc28j60, uint16_t *data, uint8_t bytes) {
    uint8_t cmd = RBM_OPCODE | RBM_ARG0;
    uint16_t dummy[bytes + 1];
    dummy[0] = (uint16_t) cmd;
    set_gpio_pin_low(enc28j60->cs);
    write_ssi(enc28j60->ssi, dummy, bytes + 1);
    read_ssi(enc28j60->ssi, data, bytes);
    set_gpio_pin_high(enc28j60->cs);
}

static void write_control_register(struct ENC28J60 *enc28j60, uint8_t reg, uint16_t data) {
    reg = (reg & 0x1F) | WCR_OPCODE;
    uint16_t reg16 = (uint16_t) reg;
    uint16_t datapacked[2];
    datapacked[0] = (uint16_t) reg;
    datapacked[1] = data;
    set_gpio_pin_low(enc28j60->cs);
    write_ssi(enc28j60->ssi, datapacked, 2);
    while (ssi_is_busy(enc28j60->ssi))
        ;
    set_gpio_pin_high(enc28j60->cs);
    while (!ssi_rx_empty(enc28j60->ssi)) 
        read_ssi(enc28j60->ssi, datapacked, 1);
}

static void write_buffer_memory(struct ENC28J60 *enc28j60, uint16_t *data, uint8_t bytes) {
    uint8_t cmd = WBM_OPCODE | WBM_ARG0;
    uint16_t dummy[bytes + 1];
    dummy[0] = (uint16_t) cmd;
    set_gpio_pin_low(enc28j60->cs);
    write_ssi(enc28j60->ssi, dummy, 1);
    write_ssi(enc28j60->ssi, data, bytes);
    set_gpio_pin_high(enc28j60->cs);
}

static void bit_field_set(struct ENC28J60 *enc28j60, uint8_t reg, uint8_t bitfield) {
    reg = (reg & 0x1F) | BFS_OPCODE;
    uint16_t data[2];
    data[0] = (uint16_t) reg;
    data[1] = (uint16_t) bitfield;
    set_gpio_pin_low(enc28j60->cs);
    write_ssi(enc28j60->ssi, data, 2);
    while (ssi_is_busy(enc28j60->ssi))
        ;
    set_gpio_pin_high(enc28j60->cs);
    while (!ssi_rx_empty(enc28j60->ssi)) 
        read_ssi(enc28j60->ssi, data, 1);
}

static void bit_field_clear(struct ENC28J60 *enc28j60, uint8_t reg, uint8_t bitfield) {
    reg = (reg & 0x1F) | BFC_OPCODE;
    uint16_t data[2];
    data[0] = (uint16_t) reg;
    data[1] = (uint16_t) bitfield;
    set_gpio_pin_low(enc28j60->cs);
    write_ssi(enc28j60->ssi, data, 2);
    while (ssi_is_busy(enc28j60->ssi))
        ;
    set_gpio_pin_high(enc28j60->cs);
    while (!ssi_rx_empty(enc28j60->ssi)) 
        read_ssi(enc28j60->ssi, data, 1);
}

static void system_reset(struct ENC28J60 *enc28j60) {
    uint8_t cmd = SRC_OPCODE | SRC_ARG0;
    uint16_t cmd16 = (uint16_t) cmd;
    set_gpio_pin_low(enc28j60->cs);
    write_ssi(enc28j60->ssi, &cmd16, 1);
    set_gpio_pin_high(enc28j60->cs);
}

static void ENC28J60_init_peripherals(struct ENC28J60 *enc28j60) {
    init_ssi(enc28j60->ssi);
    init_ssi_clock_prescale_divider(enc28j60->ssi, 16);
    init_ssi_mode(enc28j60->ssi, 0, 0, 0, 0, 8);
    enable_ssi(enc28j60->ssi);

    init_gpio_port_clock(enc28j60->cs);
    disable_gpio_pin_alternate_function(enc28j60->cs);
    init_gpio_pin_as_output(enc28j60->cs);
    init_gpio_pin_as_digital(enc28j60->cs);
    set_gpio_pin_high(enc28j60->cs);
}

static void init_rx_buffer(struct ENC28J60 *enc28j60) {
    write_control_register(enc28j60, ERXSTL, 0);  // Rx buffer start
    write_control_register(enc28j60, ERXSTH, 0);
    write_control_register(enc28j60, ERXNDL, 0xFE);  // Rx buffer end
    write_control_register(enc28j60, ERXNDH, 0x0F);
    write_control_register(enc28j60, ERXRDPTL, 0);  // set rx read ptr to start; this is only for tracking
    write_control_register(enc28j60, ERXRDPTH, 0);
}

static void init_receive_filters(struct ENC28J60 *enc28j60) {
    write_control_register(enc28j60, ECON1, 0x01); // switch to bank 1
    write_control_register(enc28j60, ERXFCON, 0);  // enable promiscuous mode (receive all packets)
}

uint8_t ENC28J60_init(struct ENC28J60 *enc28j60) {
    ENC28J60_init_peripherals(enc28j60);
    init_rx_buffer(enc28j60);
    init_receive_filters(enc28j60);

    while ((read_control_register(enc28j60, ESTAT) & 1) == 0)  // wait for OST
        ;

    return read_control_register(enc28j60, ESTAT);
}

void ENC28J60_read_data(struct ENC28J60 *enc28j60, uint8_t data[], uint32_t size) {
    return;
}

void ENC28J60_write_data(struct ENC28J60 *enc28j60, uint8_t *data, uint32_t size) {
    return;
}
