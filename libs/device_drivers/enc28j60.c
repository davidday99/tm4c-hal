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
#define BFC_OPCODE 0xA0
#define SRC_OPCODE 0xE0
#define NOP 0xF0  // any opcode not mapped to an instruction

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

#define PHCON1 0
#define PHSTAT1 0x01
#define PHID1 0x02
#define PHID2 0x03
#define PHCON2 0x10
#define PHSTAT2 0x11
#define PHIE 0x12
#define PHIR 0x13
#define PHLCON 0x14


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

void write_nop(struct ENC28J60 *enc28j60) {
    uint16_t datapacked[1];
    datapacked[0] = NOP;
    set_gpio_pin_low(enc28j60->cs);
    write_ssi(enc28j60->ssi, datapacked, 1);
    while (ssi_is_busy(enc28j60->ssi))
        ;
    set_gpio_pin_high(enc28j60->cs);
    while (!ssi_rx_empty(enc28j60->ssi)) 
        read_ssi(enc28j60->ssi, datapacked, 1);
}

static uint8_t read_control_register(struct ENC28J60 *enc28j60, uint8_t reg, uint8_t ethreg) {
    reg = (reg & 0x1F) | RCR_OPCODE;
    uint8_t read;

    uint16_t dummy[3];
    dummy[0] = (uint16_t) reg;
    dummy[1] = NOP;
    dummy[2] = NOP;
    uint16_t data[3];
    set_gpio_pin_low(enc28j60->cs);

    write_ssi(enc28j60->ssi, dummy, ethreg ? 2 : 3);
    read_ssi(enc28j60->ssi, data, ethreg ? 2 : 3);

    set_gpio_pin_high(enc28j60->cs);
    return ethreg ? data[1] : data[2];
}

void read_buffer_memory(struct ENC28J60 *enc28j60, uint16_t *data, uint32_t bytes) {
    uint8_t cmd = RBM_OPCODE | RBM_ARG0;
    uint16_t dummy[2];
    dummy[0] = (uint16_t) cmd;
    dummy[2] = NOP;
    set_gpio_pin_low(enc28j60->cs);
    write_ssi(enc28j60->ssi, dummy, 1);
    read_ssi(enc28j60->ssi, data, 1);
    for (uint16_t i = 0; i < bytes; i++) {
        write_ssi(enc28j60->ssi, &dummy[2], 1);
        read_ssi(enc28j60->ssi, &data[i], 1);
    }
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
    uint8_t bank = read_control_register(enc28j60, ECON1, 1) & 3;
    bit_field_clear(enc28j60, ECON1, 3); // switch to bank 0
    bit_field_set(enc28j60, ECON1, 0);

    write_control_register(enc28j60, ERXSTL, 0);  // Rx buffer start
    write_control_register(enc28j60, ERXSTH, 0);
    write_control_register(enc28j60, ERXNDL, 0xFF);  // Rx buffer end
    write_control_register(enc28j60, ERXNDH, 0x1F);

    write_control_register(enc28j60, ERDPTL, 0);  // Rx read pointer
    write_control_register(enc28j60, ERDPTH, 0);

    // set rx read ptr to start; data cannot be written past this, and it must be incremented manually
    write_control_register(enc28j60, ERXRDPTL, 0xFF);  
    write_control_register(enc28j60, ERXRDPTH, 0x1F);

    bit_field_clear(enc28j60, ECON1, 3);  // restore bank to previous value
    bit_field_set(enc28j60, ECON1, bank);
}

static void init_receive_filters(struct ENC28J60 *enc28j60) {
    uint8_t bank = read_control_register(enc28j60, ECON1, 1) & 3;
    bit_field_clear(enc28j60, ECON1, 3); // switch to bank 1
    bit_field_set(enc28j60, ECON1, 1);
    write_control_register(enc28j60, ERXFCON, 0);  // enable promiscuous mode (receive all packets)
    bit_field_clear(enc28j60, ECON1, 3);  // restore bank to previous value
    bit_field_set(enc28j60, ECON1, bank);
}

static void init_mac_registers(struct ENC28J60 *enc28j60) {
    uint8_t bank = read_control_register(enc28j60, ECON1, 1) & 3;
    bit_field_clear(enc28j60, ECON1, 3); // switch to bank 2
    bit_field_set(enc28j60, ECON1, 2);

    write_control_register(enc28j60, MACON1, 0xF);  // enable receiving of all frames + flow control
    
    // enable padding, append CRC, no proprietary header, no large frames, check frame length, full-duplex
    write_control_register(enc28j60, MACON3, 0xB3);
    write_control_register(enc28j60, MACON4, 0x40);  // init defer and backoff settings, applies to half-duplex only? needed?

    write_control_register(enc28j60, MAMXFLL, 0xEE);  // max frame size is 1518 bytes
    write_control_register(enc28j60, MAMXFLH, 0x05);

    write_control_register(enc28j60, MABBIPG, 0x15);  // back-to-back inter-packet gap of IEEE specified 9.6us

    write_control_register(enc28j60, MAIPGL, 0x12);  // non-back-to-back interpacket gap, datasheet says 12h is typical

    bit_field_clear(enc28j60, ECON1, 3); // switch to bank 3
    bit_field_set(enc28j60, ECON1, 3);

    // init MAC address to A0-CD-DF-01-23-45, 0 in LSB if first byte indicates unicast address
    write_control_register(enc28j60, MAADR1, 0xA0);
    write_control_register(enc28j60, MAADR2, 0xCD);
    write_control_register(enc28j60, MAADR3, 0xEF);
    write_control_register(enc28j60, MAADR4, 0x01);
    write_control_register(enc28j60, MAADR5, 0x23);
    write_control_register(enc28j60, MAADR6, 0x45);

    bit_field_clear(enc28j60, ECON1, 3);  // restore bank to previous value
    bit_field_set(enc28j60, ECON1, bank);
}

static uint16_t read_phy_register(struct ENC28J60 *enc28j60, uint8_t phy_addr) {
    uint8_t bank = read_control_register(enc28j60, ECON1, 1) & 3;
    bit_field_clear(enc28j60, ECON1, 3); // switch to bank 2
    bit_field_set(enc28j60, ECON1, 2);

    write_control_register(enc28j60, MIREGADR, phy_addr);
    write_control_register(enc28j60, MICMD, 1);

    while (read_control_register(enc28j60, MISTAT, 0) & 1)  // poll MIISTAT.BUSY bit
        ;

    write_control_register(enc28j60, MICMD, 0);

    uint16_t data = read_control_register(enc28j60, MIRDL, 0) | (read_control_register(enc28j60, MIRDH, 0) << 8);

    bit_field_clear(enc28j60, ECON1, 3);  // restore bank to previous value
    bit_field_set(enc28j60, ECON1, bank);

    return data;
}

static uint16_t write_phy_register(struct ENC28J60 *enc28j60, uint8_t phy_addr, int16_t value) {
    uint8_t bank = read_control_register(enc28j60, ECON1, 1) & 3;
    bit_field_clear(enc28j60, ECON1, 3); // switch to bank 2
    bit_field_set(enc28j60, ECON1, 2);

    write_control_register(enc28j60, MIREGADR, phy_addr);
    write_control_register(enc28j60, MIWRL, value & 0xFF);
    write_control_register(enc28j60, MIWRH, (value & 0xFF00) >> 8);


    while (read_control_register(enc28j60, MISTAT, 0) & 1)  // poll MIISTAT.BUSY bit
        ;

    bit_field_clear(enc28j60, ECON1, 3);  // restore bank to previous value
    bit_field_set(enc28j60, ECON1, bank);
}

static void init_phy_registers(struct ENC28J60 *enc28j60) {
    write_phy_register(enc28j60, PHCON1, 0x0100);
}

static uint8_t init_success(struct ENC28J60 *enc28j60) {
    uint8_t bank = read_control_register(enc28j60, ECON1, 1) & 3;
    bit_field_clear(enc28j60, ECON1, 3); // switch to bank 0
    bit_field_set(enc28j60, ECON1, 0);

    uint8_t success = ((uint16_t) (read_control_register(enc28j60, ERXSTL, 1) | (read_control_register(enc28j60, ERXSTH, 1) << 8))) == 0;
    success &= ((uint16_t) (read_control_register(enc28j60, ERXNDL, 1) |
                        (read_control_register(enc28j60, ERXNDH, 1) << 8))) == 0x1FFF;
    success &= ((uint16_t) read_control_register(enc28j60, ERXRDPTL, 1) |
                        (read_control_register(enc28j60, ERXRDPTH, 1) << 8)) == 0x1FFF;
    success &= ((uint16_t) read_control_register(enc28j60, ERDPTL, 1) |
                        (read_control_register(enc28j60, ERDPTH, 1) << 8)) == 0;
 
    bit_field_clear(enc28j60, ECON1, 3); // switch to bank 2
    bit_field_set(enc28j60, ECON1, 2);                    

    success &= read_control_register(enc28j60, MACON1, 0) == 0xF;
    success &= read_control_register(enc28j60, MACON3, 0) == 0xB3;
    success &= read_control_register(enc28j60, MACON4, 0) == 0x40;
    success &= ((uint16_t) (read_control_register(enc28j60, MAMXFLL, 0) |
                (read_control_register(enc28j60, MAMXFLH, 0) << 8))) == 0x05EE;
    success &= read_control_register(enc28j60, MABBIPG, 0) == 0x15;
    success &= read_control_register(enc28j60, MAIPGL, 0) == 0x12;

    bit_field_clear(enc28j60, ECON1, 3); // switch to bank 3
    bit_field_set(enc28j60, ECON1, 3);   

    success &= read_control_register(enc28j60, MAADR1, 0) == 0xA0;
    success &= read_control_register(enc28j60, MAADR2, 0) == 0xCD;
    success &= read_control_register(enc28j60, MAADR3, 0) == 0xEF;
    success &= read_control_register(enc28j60, MAADR4, 0) == 0x01;
    success &= read_control_register(enc28j60, MAADR5, 0) == 0x23;
    success &= read_control_register(enc28j60, MAADR6, 0) == 0x45;

    success &= read_phy_register(enc28j60, PHCON1) == 0x0100;

    bit_field_clear(enc28j60, ECON1, 3);  // restore bank to previous value
    bit_field_set(enc28j60, ECON1, bank);

    return success;
} 

uint8_t enable_receive(struct ENC28J60 *enc28j60) {
    bit_field_set(enc28j60, ECON1, 4);

    return (read_control_register(enc28j60, ECON1, 1) & 4) != 0; 
}

uint8_t disable_receive(struct ENC28J60 *enc28j60) {
    bit_field_clear(enc28j60, ECON1, ~4);

    return (read_control_register(enc28j60, ECON1, 1) & 4) == 0; 
}

uint8_t get_packet_count(struct ENC28J60 *enc28j60) {
    uint8_t bank = read_control_register(enc28j60, ECON1, 1) & 3;
    bit_field_clear(enc28j60, ECON1, 3); // switch to bank 1
    bit_field_set(enc28j60, ECON1, 1);
    uint8_t count = read_control_register(enc28j60, EPKTCNT, 1);
    bit_field_clear(enc28j60, ECON1, 3);  // restore bank to previous value
    bit_field_set(enc28j60, ECON1, bank);
    return count;
}

uint8_t ENC28J60_init(struct ENC28J60 *enc28j60) {
    ENC28J60_init_peripherals(enc28j60);
    init_rx_buffer(enc28j60);
    init_receive_filters(enc28j60);

    while ((read_control_register(enc28j60, ESTAT, 1) & 1) == 0)  // wait for OST
        ;

    init_mac_registers(enc28j60);
    init_phy_registers(enc28j60);

    return init_success(enc28j60);
}

uint16_t ENC28J60_read_frame(struct ENC28J60 *enc28j60, uint8_t data[], uint32_t size) {
    uint16_t len;
    uint8_t next_frame[2];
    uint8_t rsv[4];

    uint8_t bank = read_control_register(enc28j60, ECON1, 1) & 3;
    bit_field_clear(enc28j60, ECON1, 3); // switch to bank 0
    bit_field_set(enc28j60, ECON1, 0);

    read_buffer_memory(enc28j60, next_frame, 2);
    read_buffer_memory(enc28j60, rsv, 4);

    len = rsv[0] | (rsv[1] << 8);
    
    read_buffer_memory(enc28j60, data, len);
    write_control_register(enc28j60, ERXRDPTL, next_frame[0]);
    write_control_register(enc28j60, ERXRDPTH, next_frame[1]);
    
    bit_field_clear(enc28j60, ECON1, 3);  // restore bank to previous value
    bit_field_set(enc28j60, ECON1, bank);

    return len;
}

void ENC28J60_write_frame(struct ENC28J60 *enc28j60, uint8_t *data, uint32_t size) {
    return;
}
