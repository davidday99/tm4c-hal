#include "tm4c123gh6pm.h"
#include <stdint.h>

enum SSI_MODULE {
    SSI0,
    SSI1,
    SSI2,
    SSI3,
};

struct SSI {
    volatile uint32_t *SSICR0;
    volatile uint32_t *SSICR1;
    volatile uint32_t *SSIDR;
    volatile uint32_t *SSISR;
    volatile uint32_t *SSICPSR;
    volatile uint32_t *SSIIM;
    volatile uint32_t *SSIRIS;
    volatile uint32_t *SSIMIS;
    volatile uint32_t *SSIICR;
    volatile uint32_t *SSIDMACTL;
    volatile uint32_t *SSICC;
};

static struct SSI SSI[] = {
    {&SSI0_CR0_R, &SSI0_CR1_R, &SSI0_DR_R},
    {&SSI1_CR0_R, &SSI1_CR1_R, &SSI1_DR_R},
};

void init_SSICR0(enum SSI_MODULE module, uint8_t scr,
                    uint8_t sph, uint8_t spo,
                    uint8_t frf, uint8_t dss) {
    *(SSI[module].SSICR0) &= ~0xFFFF;
    *(SSI[module].SSICR0) |= scr << SSI_CR0_SCR_S;
    if (sph) 
        *(SSI[module].SSICR0) |= SSI_CR0_SPO;
    if (spo)
        *(SSI[module].SSICR0) |= SSI_CR0_SPO;

    *(SSI[module].SSICR0) |= dss;

}

void init_SSICR1(enum SSI_MODULE module, uint8_t mode) {
    *(SSI[module].SSICR1) &= ~SSI_CR1_MS;
    *(SSI[module].SSICR1) &= ~SSI_CR1_SSE;
    *(SSI[module].SSICR1) |= mode;
}

void set_SSICR1SSE_high(enum SSI_MODULE module) {
    *(SSI[module].SSICR1) |= 2;
}

void set_SSICR1SSE_low(enum SSI_MODULE module) {
    *(SSI[module].SSICR1) &= ~SSI_CR1_SSE;
}

void init_SSICC(enum SSI_MODULE module, uint8_t cs) {
    *(SSI[module].SSICC) &= ~SSI_CC_CS_M;
    *(SSI[module].SSICC) |= cs;
}

void init_SSICPSR(enum SSI_MODULE module, uint8_t cs) {
    *(SSI[module].SSICPSR) &= ~SSI_CPSR_CPSDVSR_M;
    *(SSI[module].SSICC) |= cs;
}

uint16_t read_SSIDR(enum SSI_MODULE module) {
    uint16_t data = *(SSI[module].SSIDR) & SSI_DR_DATA_M;
    return data;
}

void write_SSIDR(enum SSI_MODULE module, uint16_t data) {
    *(SSI[module].SSIDR) = data;
}

void SSI_isbusy(enum SSI_MODULE module) {
    return (*(SSI[module].SSIDR) & SSI_SR_TNF) == 0;
}