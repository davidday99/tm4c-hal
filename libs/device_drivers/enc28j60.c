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


#define ECON1 0x1F
#define ECON2 0x1E

#define ERDPTL 0

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

static void read_control_register(struct ENC28J60 *enc28j60, uint8_t reg, uint16_t *data) {
    reg = (reg & 0x1F) | RCR_OPCODE;
    uint16_t dummy[2];
    dummy[0] = (uint16_t) reg;
    dummy[1] = 0;
    set_gpio_pin_low(enc28j60->intr);
    write_ssi(enc28j60->ssi, dummy, 2);
    read_ssi(enc28j60->ssi, data, 2);
    set_gpio_pin_high(enc28j60->intr);
}

static void read_buffer_memory(struct ENC28J60 *enc28j60, uint16_t *data, uint8_t bytes) {
    uint8_t cmd = RBM_OPCODE | RBM_ARG0;
    uint16_t dummy[bytes + 1];
    dummy[0] = (uint16_t) cmd;
    set_gpio_pin_low(enc28j60->ssi->fss);
    write_ssi(enc28j60->ssi, dummy, bytes + 1);
    read_ssi(enc28j60->ssi, data, bytes);
    set_gpio_pin_high(enc28j60->ssi->fss);
}

static void write_control_register(struct ENC28J60 *enc28j60, uint8_t reg, uint16_t data) {
    reg = (reg & 0x1F) | WCR_OPCODE;
    uint16_t reg16 = (uint16_t) reg;
    uint16_t datapacked[2];
    datapacked[0] = (uint16_t) reg;
    datapacked[1] = data;
    set_gpio_pin_low(enc28j60->intr);
    write_ssi(enc28j60->ssi, datapacked, 2);
    while (ssi_is_busy(enc28j60->ssi))
        ;
    set_gpio_pin_high(enc28j60->intr);
    while (!ssi_rx_empty(enc28j60->ssi)) 
        read_ssi(enc28j60->ssi, datapacked, 1);
}

static void write_buffer_memory(struct ENC28J60 *enc28j60, uint16_t *data, uint8_t bytes) {
    uint8_t cmd = WBM_OPCODE | WBM_ARG0;
    uint16_t dummy[bytes + 1];
    dummy[0] = (uint16_t) cmd;
    set_gpio_pin_low(enc28j60->ssi->fss);
    write_ssi(enc28j60->ssi, dummy, 1);
    write_ssi(enc28j60->ssi, data, bytes);
    set_gpio_pin_high(enc28j60->ssi->fss);
}

static void bit_field_set(struct ENC28J60 *enc28j60, uint8_t reg, uint8_t bitfield) {
    reg = (reg & 0x1F) | BFS_OPCODE;
    uint16_t data[2];
    data[0] = (uint16_t) reg;
    data[1] = (uint16_t) bitfield;
    set_gpio_pin_low(enc28j60->ssi->fss);
    write_ssi(enc28j60->ssi, data, 2);
    set_gpio_pin_high(enc28j60->ssi->fss);
}

static void bit_field_clear(struct ENC28J60 *enc28j60, uint8_t reg, uint8_t bitfield) {
    reg = (reg & 0x1F) | BFC_OPCODE;
    uint16_t data[2];
    data[0] = (uint16_t) reg;
    data[1] = (uint16_t) bitfield;
    set_gpio_pin_low(enc28j60->ssi->fss);
    write_ssi(enc28j60->ssi, data, 2);
    set_gpio_pin_high(enc28j60->ssi->fss);
}

static void system_reset(struct ENC28J60 *enc28j60) {
    uint8_t cmd = SRC_OPCODE | SRC_ARG0;
    uint16_t cmd16 = (uint16_t) cmd;
    set_gpio_pin_low(enc28j60->ssi->fss);
    write_ssi(enc28j60->ssi, &cmd16, 1);
    set_gpio_pin_high(enc28j60->ssi->fss);
}

static void ENC28J60_init_peripherals(struct ENC28J60 *enc28j60) {
    init_ssi(enc28j60->ssi);
    init_ssi_clock_prescale_divider(enc28j60->ssi, 16);
    init_ssi_mode(enc28j60->ssi, 0, 0, 0, 0, 8);
    enable_ssi(enc28j60->ssi);

    init_gpio_port_clock(enc28j60->intr);
    disable_gpio_pin_alternate_function(enc28j60->intr);
    init_gpio_pin_as_output(enc28j60->intr);
    init_gpio_pin_as_digital(enc28j60->intr);
    set_gpio_pin_high(enc28j60->intr);
}

uint8_t ENC28J60_init(struct ENC28J60 *enc28j60) {
    ENC28J60_init_peripherals(enc28j60);
    uint16_t data = 0x03;
    uint16_t read[4] = {0};
    uint32_t i = 0;
    // while (read[0] == 0) {
    //     write_control_register(enc28j60, ECON2, &data, 1);
    //     read_control_register(enc28j60, ECON2, read, 4);
    //     i++;
    // }
    //
    write_control_register(enc28j60, ECON1, data);

    read_control_register(enc28j60, ECON1, read);
    // init_rx_buf(enc28j60);
    return read[1];
}

void ENC28J60_read_data(struct ENC28J60 *enc28j60, uint8_t data[], uint32_t size) {
    return;
}

void ENC28J60_write_data(struct ENC28J60 *enc28j60, uint8_t *data, uint32_t size) {
    return;
}

void init_rx_buf(struct ENC28J60 *enc28j60) {
    return;
}