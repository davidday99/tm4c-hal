#include "tm4c123gh6pm.h"
#include "timer.h"
#include "system.h"
#include "common.h"

struct TIMER {
    volatile uint32_t *GPTMCTL;
    volatile uint32_t *GPTMCFG;
    volatile uint32_t *GPTMIMR;
    volatile uint32_t *GPTMRIS;
    volatile uint32_t *GPTMMIS;
    volatile uint32_t *GPTMICR;
    volatile uint32_t *GPTMTAMR;
    volatile uint32_t *GPTMTBMR;
    volatile uint32_t *GPTMTAILR;
    volatile uint32_t *GPTMTBILR;
    volatile uint32_t *GPTMTAPR;
    volatile uint32_t *GPTMTBPR;
    volatile uint32_t *GPTMTAPV;
    volatile uint32_t *GPTMTBPV;
    volatile uint32_t *GPTMTAR;
    volatile uint32_t *GPTMTBR;
    volatile uint32_t *GPTMTAV;
    volatile uint32_t *GPTMTBV;
};

struct TIMER TIMER[] = {
    {
        &TIMER0_CTL_R, &TIMER0_CFG_R, &TIMER0_IMR_R, &TIMER0_RIS_R, &TIMER0_MIS_R, &TIMER0_ICR_R,
        &TIMER0_TAMR_R, &TIMER0_TBMR_R, &TIMER0_TAILR_R, &TIMER0_TBILR_R, &TIMER0_TAPR_R, &TIMER0_TBPR_R,
        &TIMER0_TAPV_R, &TIMER0_TBPV_R, &TIMER0_TAR_R, &TIMER0_TBR_R, &TIMER0_TAV_R, &TIMER0_TBV_R
    },
    {
        &TIMER1_CTL_R, &TIMER1_CFG_R, &TIMER1_IMR_R, &TIMER1_RIS_R, &TIMER1_MIS_R, &TIMER1_ICR_R,
        &TIMER1_TAMR_R, &TIMER1_TBMR_R, &TIMER1_TAILR_R, &TIMER1_TBILR_R, &TIMER1_TAPR_R, &TIMER1_TBPR_R,
        &TIMER1_TAPV_R, &TIMER1_TBPV_R, &TIMER1_TAR_R, &TIMER1_TBR_R, &TIMER1_TAV_R, &TIMER1_TBV_R
    },
    {
        &TIMER2_CTL_R, &TIMER2_CFG_R, &TIMER2_IMR_R, &TIMER2_RIS_R, &TIMER2_MIS_R, &TIMER2_ICR_R,
        &TIMER2_TAMR_R, &TIMER2_TBMR_R, &TIMER2_TAILR_R, &TIMER2_TBILR_R, &TIMER2_TAPR_R, &TIMER2_TBPR_R,
        &TIMER2_TAPV_R, &TIMER2_TBPV_R, &TIMER2_TAR_R, &TIMER2_TBR_R, &TIMER2_TAV_R, &TIMER2_TBV_R
    },
    {
        &TIMER3_CTL_R, &TIMER3_CFG_R, &TIMER3_IMR_R, &TIMER3_RIS_R, &TIMER3_MIS_R, &TIMER3_ICR_R,
        &TIMER3_TAMR_R, &TIMER3_TBMR_R, &TIMER3_TAILR_R, &TIMER3_TBILR_R, &TIMER3_TAPR_R, &TIMER3_TBPR_R,
        &TIMER3_TAPV_R, &TIMER3_TBPV_R, &TIMER3_TAR_R, &TIMER3_TBR_R, &TIMER3_TAV_R, &TIMER3_TBV_R
    },
    {
        &TIMER4_CTL_R, &TIMER4_CFG_R, &TIMER4_IMR_R, &TIMER4_RIS_R, &TIMER4_MIS_R, &TIMER4_ICR_R,
        &TIMER4_TAMR_R, &TIMER4_TBMR_R, &TIMER4_TAILR_R, &TIMER4_TBILR_R, &TIMER4_TAPR_R, &TIMER4_TBPR_R,
        &TIMER4_TAPV_R, &TIMER4_TBPV_R, &TIMER4_TAR_R, &TIMER4_TBR_R, &TIMER4_TAV_R, &TIMER4_TBV_R
    },
    {
        &TIMER5_CTL_R, &TIMER5_CFG_R, &TIMER5_IMR_R, &TIMER5_RIS_R, &TIMER5_MIS_R, &TIMER5_ICR_R,
        &TIMER5_TAMR_R, &TIMER5_TBMR_R, &TIMER5_TAILR_R, &TIMER5_TBILR_R, &TIMER5_TAPR_R, &TIMER5_TBPR_R,
        &TIMER5_TAPV_R, &TIMER5_TBPV_R, &TIMER5_TAR_R, &TIMER5_TBR_R, &TIMER5_TAV_R, &TIMER5_TBV_R
    },
    {
        &WTIMER0_CTL_R, &WTIMER0_CFG_R, &WTIMER0_IMR_R, &WTIMER0_RIS_R, &WTIMER0_MIS_R, &WTIMER0_ICR_R,
        &WTIMER0_TAMR_R, &WTIMER0_TBMR_R, &WTIMER0_TAILR_R, &WTIMER0_TBILR_R, &WTIMER0_TAPR_R, &WTIMER0_TBPR_R,
        &WTIMER0_TAPV_R, &WTIMER0_TBPV_R, &WTIMER0_TAR_R, &WTIMER0_TBR_R, &WTIMER0_TAV_R, &WTIMER0_TBV_R
    },
    {
        &WTIMER1_CTL_R, &WTIMER1_CFG_R, &WTIMER1_IMR_R, &WTIMER1_RIS_R, &WTIMER1_MIS_R, &WTIMER1_ICR_R,
        &WTIMER1_TAMR_R, &WTIMER1_TBMR_R, &WTIMER1_TAILR_R, &WTIMER1_TBILR_R, &WTIMER1_TAPR_R, &WTIMER1_TBPR_R,
        &WTIMER1_TAPV_R, &WTIMER1_TBPV_R, &WTIMER1_TAR_R, &WTIMER1_TBR_R, &WTIMER1_TAV_R, &WTIMER1_TBV_R
    },
    {
        &WTIMER2_CTL_R, &WTIMER2_CFG_R, &WTIMER2_IMR_R, &WTIMER2_RIS_R, &WTIMER2_MIS_R, &WTIMER2_ICR_R,
        &WTIMER2_TAMR_R, &WTIMER2_TBMR_R, &WTIMER2_TAILR_R, &WTIMER2_TBILR_R, &WTIMER2_TAPR_R, &WTIMER2_TBPR_R,
        &WTIMER2_TAPV_R, &WTIMER2_TBPV_R, &WTIMER2_TAR_R, &WTIMER2_TBR_R, &WTIMER2_TAV_R, &WTIMER2_TBV_R
    },
    {
        &WTIMER3_CTL_R, &WTIMER3_CFG_R, &WTIMER3_IMR_R, &WTIMER3_RIS_R, &WTIMER3_MIS_R, &WTIMER3_ICR_R,
        &WTIMER3_TAMR_R, &WTIMER3_TBMR_R, &WTIMER3_TAILR_R, &WTIMER3_TBILR_R, &WTIMER3_TAPR_R, &WTIMER3_TBPR_R,
        &WTIMER3_TAPV_R, &WTIMER3_TBPV_R, &WTIMER3_TAR_R, &WTIMER3_TBR_R, &WTIMER3_TAV_R, &WTIMER3_TBV_R
    },
    {
        &TIMER4_CTL_R, &TIMER4_CFG_R, &TIMER4_IMR_R, &TIMER4_RIS_R, &TIMER4_MIS_R, &TIMER4_ICR_R,
        &TIMER4_TAMR_R, &TIMER4_TBMR_R, &TIMER4_TAILR_R, &TIMER4_TBILR_R, &TIMER4_TAPR_R, &TIMER4_TBPR_R,
        &TIMER4_TAPV_R, &TIMER4_TBPV_R, &TIMER4_TAR_R, &TIMER4_TBR_R, &TIMER4_TAV_R, &TIMER4_TBV_R
    },
    {
        &TIMER5_CTL_R, &TIMER5_CFG_R, &TIMER5_IMR_R, &TIMER5_RIS_R, &TIMER5_MIS_R, &TIMER5_ICR_R,
        &TIMER5_TAMR_R, &TIMER5_TBMR_R, &TIMER5_TAILR_R, &TIMER5_TBILR_R, &TIMER5_TAPR_R, &TIMER5_TBPR_R,
        &TIMER5_TAPV_R, &TIMER5_TBPV_R, &TIMER5_TAR_R, &TIMER5_TBR_R, &TIMER5_TAV_R, &TIMER5_TBV_R
    }
};

static void set_GPTM_NVI(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub);
static void clear_GPTM_NVI(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub);

void init_GPTM(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub, uint8_t wide, enum TIMER_MODE mode) {
    if (timer >= WTIMER0)
        set_RCGCWTIMER(timer - WTIMER0, 1);
    else
        set_RCGCTIMER(timer, 1);

    init_GPTMnCTL(timer, sub);
    (*TIMER[timer].GPTMCFG) = 0;
    set_GPTM_mode(timer, sub, mode);
    init_GPTM_width(timer, wide);
}

void set_GPTM_start_value(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub, uint32_t start) {
    volatile uint32_t *submodule_register_ptr = 0;
    if (sub == TIMER_A) {
        submodule_register_ptr = TIMER[timer].GPTMTAILR;
    } else if (sub == TIMER_B) {
        submodule_register_ptr = TIMER[timer].GPTMTBILR;
    }

    *submodule_register_ptr = start;
}

void start_GPTM(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub) {
    if (sub == TIMER_A) {
        *(TIMER[timer].GPTMCTL) |= 1;
    }
    else if (sub == TIMER_B) {
        *(TIMER[timer].GPTMCTL) = 0x100;
    }
}

void stop_GPTM(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub) {
    if (sub == TIMER_A) {
        *(TIMER[timer].GPTMCTL) &= ~1;
    }
    else if (sub == TIMER_B) {
        *(TIMER[timer].GPTMCTL) &= ~0x100;
    }
}

void init_GPTMnCTL(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub) {
    if (sub == TIMER_A) {
        *(TIMER[timer].GPTMCTL) &= ~1;
        *(TIMER[timer].GPTMCTL) |= 2;
    }
    else if (sub == TIMER_B) {
        *(TIMER[timer].GPTMCTL) &= ~0x100;
        *(TIMER[timer].GPTMCTL) |= 0x200;
    }
}

void init_GPTM_width(enum TIMER_MODULE timer, uint8_t wide) {
    if (wide) {
        *(TIMER[timer].GPTMCFG) = (*(TIMER[timer].GPTMCFG) & ~7);
    } else {
        *(TIMER[timer].GPTMCFG) = (*(TIMER[timer].GPTMCFG) & ~7) | 4;
    }
}

void enable_GPTM_rtc_configuration(enum TIMER_MODULE timer) {
    *(TIMER[timer].GPTMCFG) = (*(TIMER[timer].GPTMCFG) & ~7) | 1;
}

void set_GPTM_mode(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub, enum TIMER_MODE mode) {
    volatile uint32_t *submodule_register_ptr = 0;
    if (sub == TIMER_A) {
        submodule_register_ptr = TIMER[timer].GPTMTAMR;
    } else if (sub == TIMER_B) {
        submodule_register_ptr = TIMER[timer].GPTMTBMR;
    }

    switch (mode) {
        case TIMER_MODE_ONE_SHOT:
            *submodule_register_ptr = (*submodule_register_ptr & ~3) | 1;
            break;
        case TIMER_MODE_PERIODIC:
            *submodule_register_ptr = (*submodule_register_ptr & ~3) | 2;
            break;
        case TIMER_MODE_CAPTURE:
            *submodule_register_ptr = (*submodule_register_ptr & ~3) | 3;
            break;
    }
}

void set_GPTM_count_direction(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub, enum TIMER_COUNT_DIR direction) {
    volatile uint32_t *submodule_register_ptr = 0;
    if (sub == TIMER_A) {
        submodule_register_ptr = TIMER[timer].GPTMTAMR;
    } else if (sub == TIMER_B) {
        submodule_register_ptr = TIMER[timer].GPTMTBMR;
    }

    if (direction == TIMER_COUNT_DIR_UP) {
        *submodule_register_ptr = (*(TIMER[timer].GPTMCFG) & 7) | 4;
    } else if (direction == TIMER_COUNT_DIR_DOWN) {
        *submodule_register_ptr = (*(TIMER[timer].GPTMCFG) & 7);
    }
}

void enable_GPTM_interrupts(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub) {
    set_GPTM_NVI(timer, sub);
}

void disable_GPTM_interrupts(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub) {
    clear_GPTM_NVI(timer, sub);
}

void enable_GPTM_timeout_interrupt(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub) {
    if (sub == TIMER_A) {
        *(TIMER[timer].GPTMIMR) |= 1;
    } else if (sub == TIMER_B) {
        *(TIMER[timer].GPTMIMR) |= 0x100;
    }
}

void disable_GPTM_timeout_interrupt(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub) {
    if (sub == TIMER_A) {
        *(TIMER[timer].GPTMIMR) &= ~1;
    } else if (sub == TIMER_B) {
        *(TIMER[timer].GPTMIMR) &= ~0x100;
    }
}

void clear_GPTM_timeout_interrupt(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub) {
    if (sub == TIMER_A) {
        *(TIMER[timer].GPTMICR) |= 1;
    } else if (sub == TIMER_B) {
        *(TIMER[timer].GPTMICR) |= 0x100;
    }
}

uint16_t get_GPTM_RIS(enum TIMER_MODULE timer) {
    return *(TIMER[timer].GPTMRIS) & 0xFFF;
}

uint32_t get_GPTM_value(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub) {
    volatile uint32_t *submodule_register_ptr = 0;
    if (sub == TIMER_A) {
        submodule_register_ptr = TIMER[timer].GPTMTAV;
    } else if (sub == TIMER_B) {
        submodule_register_ptr = TIMER[timer].GPTMTBV;
    }

    return *submodule_register_ptr;
}

static void set_GPTM_NVI(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub) {
    switch (timer) {
        case TIMER0:
            NVIC_EN0_R |= 1 << (19 + sub);
            break;
        case TIMER1:
            NVIC_EN0_R |= 1 << (21 + sub);
            break;
        case TIMER2:
            NVIC_EN0_R |= 1 << (23 + sub);
            break;
        case TIMER3:
            NVIC_EN1_R |= 1 << (3 + sub);
            break;
        case TIMER4:
            NVIC_EN2_R |= 1 << (6 + sub);
            break;
        case TIMER5:
            NVIC_EN2_R |= 1 << (28 + sub);
            break;
        case WTIMER0:
            NVIC_EN2_R |= 1 << (30 + sub);
            break;
        case WTIMER1:
            NVIC_EN3_R |= 1 << sub;
            break;
        case WTIMER2:
            NVIC_EN0_R |= 1 << (2 + sub);
            break;
        case WTIMER3:
            NVIC_EN0_R |= 1 << (4 + sub);
            break;
        case WTIMER4:
            NVIC_EN0_R |= 1 << (6 + sub);
            break;
        case WTIMER5:
            NVIC_EN0_R |= 1 << (8 + sub);
            break;
    }
}

static void clear_GPTM_NVI(enum TIMER_MODULE timer, enum TIMER_SUBMODULE sub) {
    switch (timer) {
        case TIMER0:
            NVIC_DIS0_R |= 1 << (19 + sub);
            break;
        case TIMER1:
            NVIC_DIS0_R |= 1 << (21 + sub);
            break;
        case TIMER2:
            NVIC_DIS0_R |= 1 << (23 + sub);
            break;
        case TIMER3:
            NVIC_DIS1_R |= 1 << (3 + sub);
            break;
        case TIMER4:
            NVIC_DIS2_R |= 1 << (6 + sub);
            break;
        case TIMER5:
            NVIC_DIS2_R |= 1 << (28 + sub);
            break;
        case WTIMER0:
            NVIC_DIS2_R |= 1 << (30 + sub);
            break;
        case WTIMER1:
            NVIC_DIS3_R |= 1 << sub;
            break;
        case WTIMER2:
            NVIC_DIS0_R |= 1 << (2 + sub);
            break;
        case WTIMER3:
            NVIC_DIS0_R |= 1 << (4 + sub);
            break;
        case WTIMER4:
            NVIC_DIS0_R |= 1 << (6 + sub);
            break;
        case WTIMER5:
            NVIC_DIS0_R |= 1 << (8 + sub);
            break;
    }
}