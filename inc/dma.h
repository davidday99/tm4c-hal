#ifndef _DMA_H_
#define _DMA_H_

#include <stdint.h>

struct DMA {
    volatile uint32_t *DMASRCENDP;
    volatile uint32_t *DMADSTENDP;
    volatile uint32_t *DMACHCTL;
    volatile uint32_t *DMASTAT;
    volatile uint32_t *DMACFG;
    volatile uint32_t *DMACTLBASE;
    volatile uint32_t *DMAALTBASE;
    volatile uint32_t *DMAWAITSTAT;
    volatile uint32_t *DMASWREQ;
    volatile uint32_t *DMAUSEBURSTSET;
    volatile uint32_t *DMAUSEBURSTCLR;
    volatile uint32_t *DMAREQMASKSET;
    volatile uint32_t *DMAREQMASKCLR;
    volatile uint32_t *DMAENASET;
    volatile uint32_t *DMAENACLR;
    volatile uint32_t *DMAALTCLR;
    volatile uint32_t *DMAPRIOSET;
    volatile uint32_t *DMAPRIOCLR;
    volatile uint32_t *DMACHASGN;
    volatile uint32_t *DMACHIS;
    volatile uint32_t *DMACHMAP0;
    volatile uint32_t *DMACHMAP1;
    volatile uint32_t *DMACHMAP2;
    volatile uint32_t *DMACHMAP3;
};

#endif _DMA_H_