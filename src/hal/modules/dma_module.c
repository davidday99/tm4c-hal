#include "dma_module.h"
#include "dma.h"
#include "common.h"
#include "system.h"

struct DMA DMA_CH0 = {DMACH0};
struct DMA DMA_CH1 = {DMACH1};
struct DMA DMA_CH2 = {DMACH2};
struct DMA DMA_CH3 = {DMACH3};
struct DMA DMA_CH4 = {DMACH4};
struct DMA DMA_CH5 = {DMACH5};
struct DMA DMA_CH6 = {DMACH6};
struct DMA DMA_CH7 = {DMACH7};
struct DMA DMA_CH8 = {DMACH8};
struct DMA DMA_CH9 = {DMACH9};
struct DMA DMA_CH10 = {DMACH10};
struct DMA DMA_CH11 = {DMACH11};
struct DMA DMA_CH12 = {DMACH12};
struct DMA DMA_CH13 = {DMACH13};
struct DMA DMA_CH14 = {DMACH14};
struct DMA DMA_CH15 = {DMACH15};
struct DMA DMA_CH16 = {DMACH16};
struct DMA DMA_CH17 = {DMACH17};
struct DMA DMA_CH18 = {DMACH18};
struct DMA DMA_CH19 = {DMACH19};
struct DMA DMA_CH20 = {DMACH20};
struct DMA DMA_CH21 = {DMACH21};
struct DMA DMA_CH22 = {DMACH22};
struct DMA DMA_CH23 = {DMACH23};
struct DMA DMA_CH24 = {DMACH24};
struct DMA DMA_CH25 = {DMACH25};
struct DMA DMA_CH26 = {DMACH26};
struct DMA DMA_CH27 = {DMACH27};
struct DMA DMA_CH28 = {DMACH28};
struct DMA DMA_CH29 = {DMACH29};
struct DMA DMA_CH30 = {DMACH30};
struct DMA DMA_CH31 = {DMACH31};

void init_dma(struct DMA *dma, uint8_t source) {
    set_RCGCDMA();
    init_DMACHn(dma->channel, 0);
    set_DMACHn_channel_source(dma->channel, source);
}

void start_dma_transfer_mem(struct DMA *dma, uint8_t *srcendptr, 
                        uint8_t *destendptr, uint16_t size) {
    set_DMACHn_src_end(dma->channel, srcendptr);
    set_DMACHn_dest_end(dma->channel, destendptr);
    set_DMACHn_control_word(dma->channel, INC_BYTE, SIZE_BYTE,
                            INC_BYTE, SIZE_BYTE, ARBSIZE_4,
                            size, BURSTMODE_EN, TRANSFER_MODE_AUTO);
    enable_DMACHn(dma->channel);
    issue_DMACHn_software_request(dma->channel);
}

void start_dma_transfer_peripheral_tx(struct DMA *dma, uint8_t *srcendptr, 
                        uint8_t *destendptr, uint16_t size) {
    set_DMACHn_src_end(dma->channel, srcendptr);
    set_DMACHn_dest_end(dma->channel, destendptr);
    set_DMACHn_control_word(dma->channel, INC_NONE, SIZE_BYTE,
                            INC_BYTE, SIZE_BYTE, ARBSIZE_4,
                            size, BURSTMODE_DIS, TRANSFER_MODE_BASIC);
    enable_DMACHn_requests(dma->channel);
    enable_DMACHn(dma->channel);
}

void start_dma_transfer_peripheral_rx(struct DMA *dma, uint8_t *srcendptr, 
                        uint8_t *destendptr, uint16_t size) {
    set_DMACHn_src_end(dma->channel, srcendptr);
    set_DMACHn_dest_end(dma->channel, destendptr);
    set_DMACHn_control_word(dma->channel, INC_BYTE, SIZE_BYTE,
                            INC_NONE, SIZE_BYTE, ARBSIZE_4,
                            size, BURSTMODE_DIS, TRANSFER_MODE_BASIC);
    enable_DMACHn_requests(dma->channel);
    enable_DMACHn(dma->channel);
}

uint16_t remaining_dma_transfer_count(struct DMA *dma) {
    struct DMA_CONTROL_STRUCTURE_T *cs = get_DMACHn_control_structure(dma->channel);
    return (cs->ctrlword >> 4) & 0x3FF;
}

uint8_t dma_busy(void) {
    uint8_t status = (get_DMA_status() & 0xF0);
    if (status == 0 || status == 0x90)
        return 0;
    else
        return 1;
}

uint8_t dma_interrupt_occurred(struct DMA *dma) {
    return get_DMACHn_interrupt_status(dma->channel);
}

void clear_dma_interrupt(struct DMA *dma) {
    clear_DMACHn_interrupt(dma->channel);
}
