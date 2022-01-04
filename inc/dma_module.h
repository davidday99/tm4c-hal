#ifndef _DMA_MODULE_H_
#define _DMA_MODULE_H_

#include <stdint.h>
#include "common.h"

struct DMA_MODULE {
    enum DMA_CHANNEL channel;
};

void init_dma(struct DMA_MODULE *dma, enum DMA_CHANNEL ch, uint8_t source);
void start_dma_transfer_mem(struct DMA_MODULE *dma, uint8_t *srcendptr, 
                        uint8_t *destendptr, uint16_t size);
void start_dma_transfer_peripheral(struct DMA_MODULE *dma, uint8_t *srcendptr, 
                        uint8_t *destendptr, uint16_t size);
uint8_t dma_busy(void);

#endif /* _DMA_H_ */
