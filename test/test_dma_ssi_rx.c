#include "dma_module.h"
#include "ssi_module.h"
#include "dma.h"
#include "common.h"
#include "string.h"
#include "tm4c123gh6pm.h"

static void Delay(uint32_t d) {
    uint32_t volatile delay = d * 80000;
    while (delay > 0)
        delay--;
}

int test_dma_ssi_rx() {
    int success = 1;
    
    uint8_t src[5];
    for (int i = 0; i < 5; i++)
        src[i] = i;

    uint8_t dest[5] = {0};

    struct DMA_MODULE _dma;
    struct DMA_MODULE *dma = &_dma;

    struct SSI *ssi = &SSI_2;

    init_ssi(ssi);
    init_ssi_clock_prescale_divider(ssi, 16);
    init_ssi_mode(ssi, 0, 0, 0, 0, 8);
    enable_ssi_loopback(ssi);
    enable_ssi_rx_dma(ssi);
    enable_ssi(ssi);

    init_dma(dma, DMACH12, 2);

    enable_DMACHn_requests(dma->channel);

    SSI2_IM_R |= 0xC0;  // TODO: create functions for enabling/disabling peripheral interrupts

    NVIC_EN1_R |= 1 << 25;  // TODO: create functions for enabling/disabling interrupts in NVIC

    write_ssi(ssi, src, 5);

    start_dma_transfer_peripheral_rx(dma, (uint8_t *) &SSI2_DR_R, dest + 4, 5);

    Delay(10);
    
    success &= memcmp(src, dest, 5) == 0;
    return success;
}