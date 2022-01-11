#ifndef _DMA_MODULE_H_
#define _DMA_MODULE_H_

#include <stdint.h>
#include "common.h"

struct DMA {
    enum DMA_CHANNEL channel;
};

extern struct DMA DMA_CH0;
extern struct DMA DMA_CH1;
extern struct DMA DMA_CH2;
extern struct DMA DMA_CH3;
extern struct DMA DMA_CH4;
extern struct DMA DMA_CH5;
extern struct DMA DMA_CH6;
extern struct DMA DMA_CH7;
extern struct DMA DMA_CH8;
extern struct DMA DMA_CH9;
extern struct DMA DMA_CH10;
extern struct DMA DMA_CH11;
extern struct DMA DMA_CH12;
extern struct DMA DMA_CH13;
extern struct DMA DMA_CH14;
extern struct DMA DMA_CH15;
extern struct DMA DMA_CH16;
extern struct DMA DMA_CH17;
extern struct DMA DMA_CH18;
extern struct DMA DMA_CH19;
extern struct DMA DMA_CH20;
extern struct DMA DMA_CH21;
extern struct DMA DMA_CH22;
extern struct DMA DMA_CH23;
extern struct DMA DMA_CH24;
extern struct DMA DMA_CH25;
extern struct DMA DMA_CH26;
extern struct DMA DMA_CH27;
extern struct DMA DMA_CH28;
extern struct DMA DMA_CH29;
extern struct DMA DMA_CH30;
extern struct DMA DMA_CH31;

void init_dma(struct DMA *dma, uint8_t source);
void start_dma_transfer_mem(struct DMA *dma, uint8_t *srcendptr, 
                        uint8_t *destendptr, uint16_t size);
void start_dma_transfer_peripheral_tx(struct DMA *dma, uint8_t *srcendptr, 
                        uint8_t *destendptr, uint16_t size);
void start_dma_transfer_peripheral_rx(struct DMA *dma, uint8_t *srcendptr, 
                        uint8_t *destendptr, uint16_t size);
uint16_t remaining_dma_transfer_count(struct DMA *dma);
uint8_t dma_busy(void);
uint8_t dma_interrupt_occurred(struct DMA *dma);
void clear_dma_interrupt(struct DMA *dma);

#endif /* _DMA_H_ */
