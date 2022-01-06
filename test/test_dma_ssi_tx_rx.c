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

int test_dma_ssi_tx_rx() {
    int success = 1;
    
    uint8_t src[5];
    for (int i = 0; i < 5; i++)
        src[i] = i;

    uint8_t dest[5] = {0};

    struct DMA_MODULE _dmatx;
    struct DMA_MODULE *dmatx = &_dmatx;

    struct DMA_MODULE _dmarx;
    struct DMA_MODULE *dmarx = &_dmarx;

    struct SSI *ssi = &SSI_2;

    init_ssi(ssi);
    init_ssi_clock_prescale_divider(ssi, 16);
    init_ssi_mode(ssi, 0, 0, 0, 0, 8);
    enable_ssi_loopback(ssi);
    enable_ssi_tx_dma(ssi);
    enable_ssi(ssi);

    init_dma(dmarx, DMACH12, 2);
    init_dma(dmatx, DMACH13, 2);


    SSI2_IM_R |= 0x80;  // mask tx fifo interrupt

    NVIC_EN1_R |= 1 << 25;

    start_dma_transfer_peripheral_tx(dmatx, src + 4, (uint8_t *) &SSI2_DR_R, 5);
    start_dma_transfer_peripheral_rx(dmarx, (uint8_t *) &SSI2_DR_R, dest + 4, 5);

    Delay(10);
    
    success &= memcmp(src, dest, 5) == 0;
    return success;
}