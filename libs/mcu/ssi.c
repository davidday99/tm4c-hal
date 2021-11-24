#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "ssi.h"

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
    {&SSI0_CR0_R, &SSI0_CR1_R, &SSI0_DR_R, &SSI0_SR_R, &SSI0_CPSR_R, &SSI0_IM_R,
    &SSI0_RIS_R, &SSI0_MIS_R, &SSI0_ICR_R, &SSI0_DMACTL_R, &SSI0_CC_R},

    {&SSI1_CR0_R, &SSI1_CR1_R, &SSI1_DR_R, &SSI1_SR_R, &SSI1_CPSR_R, &SSI1_IM_R,
    &SSI1_RIS_R, &SSI1_MIS_R, &SSI1_ICR_R, &SSI1_DMACTL_R, &SSI1_CC_R},

    {&SSI2_CR0_R, &SSI2_CR1_R, &SSI2_DR_R, &SSI2_SR_R, &SSI2_CPSR_R, &SSI2_IM_R,
    &SSI2_RIS_R, &SSI2_MIS_R, &SSI2_ICR_R, &SSI2_DMACTL_R, &SSI2_CC_R},

    {&SSI3_CR0_R, &SSI3_CR1_R, &SSI3_DR_R, &SSI3_SR_R, &SSI3_CPSR_R, &SSI3_IM_R,
    &SSI3_RIS_R, &SSI3_MIS_R, &SSI3_ICR_R, &SSI3_DMACTL_R, &SSI3_CC_R}
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
    *(SSI[module].SSICR1) &= ~SSI_CR1_SSE;
    *(SSI[module].SSICR1) &= ~SSI_CR1_MS;
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

void init_SSICPSR(enum SSI_MODULE module, uint8_t psdvsr) {
    *(SSI[module].SSICPSR) &= ~SSI_CPSR_CPSDVSR_M;
    *(SSI[module].SSICPSR) |= psdvsr;
}

uint8_t read_SSIDR(enum SSI_MODULE module) {
    uint8_t data = *(SSI[module].SSIDR) & SSI_DR_DATA_M;
    return data;
}

void write_SSIDR(enum SSI_MODULE module, uint8_t data) {
    *(SSI[module].SSIDR) = data;
}

void write_array_SSIDR(enum SSI_MODULE module, uint8_t *data, uint32_t len) {
    uint32_t i = 0;
    while (len-- > 0) {
        while (SSI_tx_full(module))
            ;
        write_SSIDR(module, data[i++]);
    }
}

uint32_t read_SSIDR_into_array(enum SSI_MODULE module, uint8_t *data, uint32_t bytes) {
    uint32_t i = 0;
    while (i < bytes && !SSI_rx_empty(module)) {
        data[i++] = read_SSIDR(module);
    }
    return i;
}

uint8_t SSI_bsy(enum SSI_MODULE module) {
    return (*(SSI[module].SSISR) & SSI_SR_BSY) != 0;
}

uint8_t SSI_tx_full(enum SSI_MODULE module) {
    return (*(SSI[module].SSISR) & SSI_SR_TNF) == 0;
}

uint8_t SSI_tx_empty(enum SSI_MODULE module) {
    return (*(SSI[module].SSISR) & SSI_SR_TFE) == 1;
}

uint8_t SSI_rx_full(enum SSI_MODULE module) {
    return (*(SSI[module].SSISR) & SSI_SR_RFF) != 0;
}

uint8_t SSI_rx_empty(enum SSI_MODULE module) {
    return (*(SSI[module].SSISR) & SSI_SR_RNE) == 0;
}