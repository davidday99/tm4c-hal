#include "dma_module.h"
#include "dma.h"
#include "common.h"
#include "string.h"
#include "tm4c123gh6pm.h"

int test_dma_mem() {
    int success = 1;

    uint8_t src[100];
    for (int i = 0; i < 100; i++)
        src[i] = i;
        
    uint8_t dest[100];

    struct DMA_MODULE _dma;
    struct DMA_MODULE *dma = &_dma;

    init_dma(dma, DMACH30, 0);
    start_dma_transfer_mem(dma, src + 99, dest + 99, 100);

    while (dma_busy()) {
        static int x;  // initialize this to allow for a breakpoint inside here
        x++;
    }
    
    success &= memcmp(src, dest, 100) == 0;
    return success;
}