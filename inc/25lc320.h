#ifndef _25LC320_H_
#define _25LC320_H_

#include <stdint.h>

struct EP25LC320 {
    struct SSI *ssi;
    struct GPIO_PIN *cs;
    struct GPIO_PIN *wp;
    struct GPIO_PIN *hold;
};

extern struct EP25LC320 EP25LC320;

void EP25LC320_init(struct EP25LC320 *eeprom);
void EP25LC320_read(struct EP25LC320 *eeprom, uint16_t addr, uint8_t *buf, uint16_t sz);
void EP25LC320_write(struct EP25LC320 *eeprom, uint16_t addr, uint8_t *buf, uint16_t sz);
void EP25LC320_erase(struct EP25LC320 *eeprom);
uint8_t EP25LC320_read_status_register(struct EP25LC320 *eeprom);
void EP25LC320_write_status_register(struct EP25LC320 *eeprom, uint8_t value);

#endif /* _25LC320_H_ */

