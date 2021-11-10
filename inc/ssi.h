#ifndef _SSI_H_
#define _SSI_H_

#include <stdint.h>

void init_SSI(uint8_t module, uint8_t freq, uint8_t freq_scaler, uint8_t phase, uint8_t polarity, uint8_t size);

#endif /* _SSI_H_ */