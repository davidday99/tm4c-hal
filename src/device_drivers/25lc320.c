#include "25lc320.h"
#include "ssi_module.h"
#include "gpio_module.h"

struct EP25LC320 EP25LC320 = {
    &SSI_0,
    &PORTB_PIN3,
    &PORTB_PIN0,
    &PORTB_PIN1
};

void EP25LC320_init(struct EP25LC320 *eeprom) {
    init_ssi(eeprom->ssi);
    init_ssi_clock_prescale_divider(eeprom->ssi, 80);
    init_ssi_mode(eeprom->ssi, 0, 0, 0, 0, 8);

    /* We're going to manually control CS
     * because my HAL interface is garbage. */
    init_gpio_port_clock(eeprom->cs);
    disable_gpio_pin_alternate_function(eeprom->cs);
    init_gpio_pin_as_output(eeprom->cs);
    init_gpio_pin_as_digital(eeprom->cs);
    set_gpio_pin_high(eeprom->cs);
    
    /* Initialize WP pin and set to low since it's active low.
     * Keep it low unless performing a write. */
    init_gpio_port_clock(eeprom->wp);
    disable_gpio_pin_alternate_function(eeprom->wp);
    init_gpio_pin_as_output(eeprom->wp);
    init_gpio_pin_as_digital(eeprom->wp);
    set_gpio_pin_low(eeprom->wp);

    /* Initialize HOLD pin and set to high since it's also active low. */
    init_gpio_port_clock(eeprom->hold);
    disable_gpio_pin_alternate_function(eeprom->hold);
    init_gpio_pin_as_output(eeprom->hold);
    init_gpio_pin_as_digital(eeprom->hold);
    set_gpio_pin_high(eeprom->hold);

    enable_ssi(eeprom->ssi);
}

void EP25LC320_read(struct EP25LC320 *eeprom, uint16_t addr, uint8_t *buf, uint16_t sz) {
    uint8_t read_cmd = 3;
    uint8_t cmd_buf[3];
    cmd_buf[0] = read_cmd;
    cmd_buf[1] = (uint8_t) ((addr & 0xFF00) >> 8);
    cmd_buf[2] = (uint8_t) (addr & 0xFF);

    /* Wait for any write in progress to complete. */
    while ((EP25LC320_read_status_register(eeprom) & 1) == 1)
        ;

    set_gpio_pin_low(eeprom->cs);
    write_ssi(eeprom->ssi, cmd_buf, 3);
    while (ssi_is_busy(eeprom->ssi))
        ;
    dump_rx_fifo(eeprom->ssi);
    read_ssi(eeprom->ssi, buf, sz, 0);
    set_gpio_pin_high(eeprom->cs);
}

void EP25LC320_write(struct EP25LC320 *eeprom, uint16_t addr, uint8_t *buf, uint16_t sz) {
    uint8_t wren_cmd = 6;
    uint8_t write_cmd = 2;
    uint8_t cmd_buf[3];
    cmd_buf[0] = write_cmd;

    uint8_t offset = 0;
    uint8_t maxwrite, wcount;
    while (sz > 0) {
        while ((EP25LC320_read_status_register(eeprom) & 1) == 1)
            ;
        cmd_buf[1] = (uint8_t) ((addr & 0xFF00) >> 8);
        cmd_buf[2] = (uint8_t) (addr & 0xFF);
        maxwrite = 32 - (addr & 0x1F);
        wcount = sz > maxwrite ? maxwrite : sz;
        set_gpio_pin_low(eeprom->cs);
        write_ssi(eeprom->ssi, &wren_cmd, 1);
        while (ssi_is_busy(eeprom->ssi))
            ;
        set_gpio_pin_high(eeprom->cs);

        set_gpio_pin_low(eeprom->cs);
        write_ssi(eeprom->ssi, cmd_buf, 3);
        write_ssi(eeprom->ssi, buf + offset, wcount); 
        while (ssi_is_busy(eeprom->ssi))
            ;
        set_gpio_pin_high(eeprom->cs);
        sz -= wcount;
        offset += wcount;
        addr += wcount;
        dump_rx_fifo(eeprom->ssi);
    }
}

void EP25LC320_erase(struct EP25LC320 *eeprom);

uint8_t EP25LC320_read_status_register(struct EP25LC320 *eeprom) {
    uint8_t rdsr_cmd = 5;
    uint8_t sr;

    set_gpio_pin_low(eeprom->cs);
    write_ssi(eeprom->ssi, &rdsr_cmd, 1);
    while (ssi_is_busy(eeprom->ssi))
        ;
    dump_rx_fifo(eeprom->ssi);
    read_ssi(eeprom->ssi, &sr, 1, 0);
    set_gpio_pin_high(eeprom->cs);

    return sr;
}

void EP25LC320_write_status_register(struct EP25LC320 *eeprom, uint8_t value) {
    uint8_t wrsr_cmd = 1;
    uint8_t cmd_buf[2];
    cmd_buf[0] = wrsr_cmd;
    cmd_buf[1] = value;

    set_gpio_pin_low(eeprom->cs);
    write_ssi(eeprom->ssi, cmd_buf, 2);
    while (ssi_is_busy(eeprom->ssi))
        ;
    dump_rx_fifo(eeprom->ssi);
    set_gpio_pin_high(eeprom->cs);
}

