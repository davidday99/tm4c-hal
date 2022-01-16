#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>
#include "common.h"

void init_GPTM(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub, uint8_t wide, enum TIMER_MODE mode);
void set_GPTM_start_value(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub, uint32_t start);
void start_GPTM(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub);
void stop_GPTM(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub);
void init_GPTMnCTL(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub);
void set_GPTM_mode(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub, enum TIMER_MODE mode);
void init_GPTM_width(enum TIMER_MODULE timer, uint8_t wide);
void set_GPTM_count_direction(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub, enum TIMER_COUNT_DIR direction);
void enable_GPTM_interrupts(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub);
void disable_GPTM_interrupts(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub);
void enable_GPTM_timeout_interrupt(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub);
void disable_GPTM_timeout_interrupt(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub);
void clear_GPTM_timeout_interrupt(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub);
uint16_t get_GPTM_RIS(enum TIMER_MODULE timer);
uint32_t get_GPTM_value(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub);

#endif /* _TIMER_H_ */