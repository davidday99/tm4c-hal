#include "dma.h"
#include "tm4c123gh6pm.h"

struct DMA DMA = {
    &UDMA_CFG_R,
    &UDMA_CTLBASE_R,
    &UDMA_ALTBASE_R,
    &UDMA_SWREQ_R,
    &UDMA_USEBURSTSET_R,
    &UDMA_USEBURSTCLR_R,
    &UDMA_REQMASKSET_R,
    &UDMA_REQMASKCLR_R,
    &UDMA_ENASET_R,
    &UDMA_ENACLR_R,
    &UDMA_ALTSET_R,
    &UDMA_ALTCLR_R,
    &UDMA_PRIOSET_R,
    &UDMA_PRIOCLR_R,
    &UDMA_ERRCLR_R,
    &UDMA_CHIS_R,
    &UDMA_CHMAP0_R,
    &UDMA_CHMAP1_R,
    &UDMA_CHMAP2_R,
    &UDMA_CHMAP3_R
};

struct DMA_CONTROL_STRUCTURE_T CS[2*DMA_NUMBER_OF_CHANNELS] __attribute__ ((aligned (1024)));

void init_DMACHn(enum DMA_CHANNEL ch, uint8_t priority) {
    *(DMA.DMACFG) |=  1 << ch;
    *(DMA.DMACTLBASE) = CS;

    set_DMACHn_priority(ch, priority);
    select_DMACHn_control_structure(ch, 0);
    select_DMACHn_request_mode(ch, 0);
    enable_DMACHn_requests(ch);   
}

void select_DMACHn_request_mode(enum DMA_CHANNEL ch, uint8_t burst) {
 *(DMA.DMAUSEBURSTSET) |= burst << ch;
 *(DMA.DMAUSEBURSTCLR) |= ~burst << ch;
}

void set_DMACHn_priority(enum DMA_CHANNEL ch, uint8_t pri) {
    *(DMA.DMAPRIOSET) |= pri << ch;
    *(DMA.DMAPRIOCLR) |= ~pri << ch;
}

void select_DMACHn_control_structure(enum DMA_CHANNEL ch, uint8_t alt) {
 *(DMA.DMAALTSET) |= alt << ch;
 *(DMA.DMAALTCLR) |= !alt << ch;
}

uint8_t get_DMA_bus_error_status(void) {
    return *(DMA.DMAERRCLR);
}

void clear_DMA_bus_error_status(void) {
    *(DMA.DMAERRCLR) |= 1;
}

void enable_DMACHn_requests(enum DMA_CHANNEL ch) {
    *(DMA.DMAREQMASKSET) |= 1 << ch;
}

void disable_DMACHn_requests(enum DMA_CHANNEL ch) {
    *(DMA.DMAREQMASKCLR) |= 1 << ch;
}

void enable_DMACHn(enum DMA_CHANNEL ch) {
    *(DMA.DMAENASET) |= 1 << ch;
}

void disable_DMACHn(enum DMA_CHANNEL ch) {
    *(DMA.DMAENACLR) |= 1 << ch;
}

void set_DMACHn_source_end(enum DMA_CHANNEL ch, uint32_t *srcendptr) {
    CS[ch].srcendptr = srcendptr;
}

void set_DMACHn_dest_end(enum DMA_CHANNEL ch, uint32_t *destendptr) {
    CS[ch].srcendptr = destendptr;
}

void set_DMACHn_control_word(enum DMA_CHANNEL ch, uint32_t cw) {
    CS[ch].ctrlword = cw;
}