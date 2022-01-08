#include "dma_module.h"
#include "dma.h"
#include "common.h"
#include "system.h"

void init_dma(struct DMA_MODULE *dma, enum DMA_CHANNEL ch, uint8_t source) {
    dma->channel = ch;
    set_RCGCDMA();
    init_DMACHn(dma->channel, 0);
    set_DMACHn_channel_source(dma->channel, source);
}

void start_dma_transfer_mem(struct DMA_MODULE *dma, uint8_t *srcendptr, 
                        uint8_t *destendptr, uint16_t size) {
    set_DMACHn_src_end(dma->channel, srcendptr);
    set_DMACHn_dest_end(dma->channel, destendptr);
    set_DMACHn_control_word(dma->channel, INC_BYTE, SIZE_BYTE,
                            INC_BYTE, SIZE_BYTE, ARBSIZE_4,
                            size, BURSTMODE_EN, TRANSFER_MODE_AUTO);
    enable_DMACHn(dma->channel);
    issue_DMACHn_software_request(dma->channel);
}

void start_dma_transfer_peripheral_tx(struct DMA_MODULE *dma, uint8_t *srcendptr, 
                        uint8_t *destendptr, uint16_t size) {
    set_DMACHn_src_end(dma->channel, srcendptr);
    set_DMACHn_dest_end(dma->channel, destendptr);
    set_DMACHn_control_word(dma->channel, INC_NONE, SIZE_BYTE,
                            INC_BYTE, SIZE_BYTE, ARBSIZE_4,
                            size, BURSTMODE_DIS, TRANSFER_MODE_BASIC);
    enable_DMACHn_requests(dma->channel);
    enable_DMACHn(dma->channel);
}

void start_dma_transfer_peripheral_rx(struct DMA_MODULE *dma, uint8_t *srcendptr, 
                        uint8_t *destendptr, uint16_t size) {
    set_DMACHn_src_end(dma->channel, srcendptr);
    set_DMACHn_dest_end(dma->channel, destendptr);
    set_DMACHn_control_word(dma->channel, INC_BYTE, SIZE_BYTE,
                            INC_NONE, SIZE_BYTE, ARBSIZE_4,
                            size, BURSTMODE_DIS, TRANSFER_MODE_BASIC);
    enable_DMACHn_requests(dma->channel);
    enable_DMACHn(dma->channel);
}

uint16_t remaining_dma_transfer_count(struct DMA_MODULE *dma) {
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
