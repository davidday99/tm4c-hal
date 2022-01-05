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

int test_dma_ssi_tx() {
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
    enable_ssi_tx_dma(ssi);
    enable_ssi(ssi);

    init_dma(dma, DMACH13, 2);

    enable_DMACHn_requests(dma->channel);

    SSI2_IM_R |= 0xC0;

    NVIC_EN1_R |= 1 << 25;

    start_dma_transfer_peripheral_tx(dma, src + 4, (uint8_t *) &SSI2_DR_R, 5);

    Delay(10);

    int i = 0;
    while ((SSI2_SR_R & 0x4) != 0)
        dest[i++] = SSI2_DR_R;
    
    success &= memcmp(src, dest, 5) == 0;
    return success;
}