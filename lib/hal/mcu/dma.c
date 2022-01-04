#include "dma.h"
#include "common.h"
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

static void set_control_word_destinc(enum DMACHCTL_ADDRESS_INC inc, uint32_t *cw);
static void set_control_word_destsize(enum DMACHCTL_DATA_SIZE size, uint32_t *cw);
static void set_control_word_srcinc(enum DMACHCTL_ADDRESS_INC inc, uint32_t *cw);
static void set_control_word_srcsize(enum DMACHCTL_DATA_SIZE size, uint32_t *cw);
static void set_control_word_arbsize(enum DMACHCTL_ARB_SIZE size, uint32_t *cw);
static void set_control_word_nxtuseburst(enum DMACHCTL_NXTUSEBURSTMODE mode, uint32_t *cw);
static void set_control_word_xfersize(uint16_t size, uint32_t *cw);
static void set_control_word_xfermode(enum DMACHCTL_TRANSFER_MODE mode, uint32_t *cw);

struct DMA_CONTROL_STRUCTURE_T CS[2*DMA_NUMBER_OF_CHANNELS] __attribute__ ((aligned (1024)));

void init_DMACHn(enum DMA_CHANNEL ch, uint8_t priority) {
    *(DMA.DMACFG) |=  1 << ch;
    *(DMA.DMACTLBASE) = (uint32_t) CS;

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

void set_DMACHn_map(enum DMA_CHANNEL ch, uint8_t src) {
    if (ch <= 7)
        *(DMA.DMACHMAP0) = (*(DMA.DMACHMAP0) & ~(0xF << ch)) | (src << ch);
    else if (ch <= 15)
        *(DMA.DMACHMAP1) = (*(DMA.DMACHMAP1) & ~(0xF << (ch - 8))) | (src << (ch - 8));
    else if (ch <= 23)
        *(DMA.DMACHMAP2) = (*(DMA.DMACHMAP2) & ~(0xF << (ch - 16))) | (src << (ch - 16));
    else if (ch <= 31)
        *(DMA.DMACHMAP3) = (*(DMA.DMACHMAP3) & ~(0xF << (ch - 24))) | (src << (ch - 24));
}

void set_DMACHn_control_word(enum DMA_CHANNEL ch, enum DMACHCTL_ADDRESS_INC dstinc,
                                enum DMACHCTL_DATA_SIZE dstsize, enum DMACHCTL_ADDRESS_INC srcinc,
                                enum DMACHCTL_DATA_SIZE srcsize, enum DMACHCTL_ARB_SIZE arbsize,
                                uint16_t transfer_size, enum DMACHCTL_NXTUSEBURSTMODE burst_mode,
                                enum DMACHCTL_TRANSFER_MODE transfer_mode) {
    uint32_t cw = 0;
    set_control_word_destinc(dstinc, &cw);
    set_control_word_destsize(dstsize, &cw);
    set_control_word_srcinc(srcinc, &cw);
    set_control_word_srcsize(srcsize, &cw);
    set_control_word_arbsize(arbsize, &cw);
    set_control_word_xfersize(transfer_size, &cw);
    set_control_word_nxtuseburst(burst_mode, &cw);
    set_control_word_xfermode(transfer_mode, &cw);

    CS[ch].ctrlword = cw;
}

static void set_control_word_destinc(enum DMACHCTL_ADDRESS_INC inc, uint32_t *cw) {
    switch (inc) {
        case INC_BYTE:
            *cw = DMACHCTL_DSTINC_BYTE(*cw);
            break;
        case INC_HALFWORD:
            *cw = DMACHCTL_DSTINC_HALFWORD(*cw);
            break;
        case INC_WORD:
            *cw = DMACHCTL_DSTINC_WORD(*cw);
            break;
        case INC_NONE:
            *cw = DMACHCTL_DSTINC_NOINC(*cw);
            break;
        default:
            break;
    }
}

static void set_control_word_destsize(enum DMACHCTL_DATA_SIZE size, uint32_t *cw) {
    switch (size) {
        case SIZE_BYTE:
            *cw = DMACHCTL_DSTSIZE_BYTE(*cw);
            break;
        case SIZE_HALFWORD:
            *cw = DMACHCTL_DSTSIZE_HALFWORD(*cw);
            break;
        case SIZE_WORD:
            *cw = DMACHCTL_DSTSIZE_WORD(*cw);
            break;
        default:
            break;
    }
}

static void set_control_word_srcinc(enum DMACHCTL_ADDRESS_INC inc, uint32_t *cw) {
    switch (inc) {
        case INC_BYTE:
            *cw = DMACHCTL_SRCINC_BYTE(*cw);
            break;
        case INC_HALFWORD:
            *cw = DMACHCTL_SRCINC_HALFWORD(*cw);
            break;
        case INC_WORD:
            *cw = DMACHCTL_SRCINC_WORD(*cw);
            break;
        case INC_NONE:
            *cw = DMACHCTL_SRCINC_NOINC(*cw);
            break;
    }
}

static void set_control_word_srcsize(enum DMACHCTL_DATA_SIZE size, uint32_t *cw) {
    switch (size) {
        case SIZE_BYTE:
            *cw = DMACHCTL_SRCSIZE_BYTE(*cw);
            break;
        case SIZE_HALFWORD:
            *cw = DMACHCTL_SRCSIZE_HALFWORD(*cw);
            break;
        case SIZE_WORD:
            *cw = DMACHCTL_SRCSIZE_WORD(*cw);
            break;
        default:
            break;
    }
}

static void set_control_word_arbsize(enum DMACHCTL_ARB_SIZE size, uint32_t *cw) {
    switch (size) {
        case ARBSIZE_1:
            *cw = DMACHCTL_ARBSIZE_1(*cw);
            break;
        case ARBSIZE_2:
            *cw = DMACHCTL_ARBSIZE_2(*cw);
            break;
        case ARBSIZE_4:
            *cw = DMACHCTL_ARBSIZE_4(*cw);
            break;
        case ARBSIZE_8:
            *cw = DMACHCTL_ARBSIZE_8(*cw);
            break;
        case ARBSIZE_16:
            *cw = DMACHCTL_ARBSIZE_16(*cw);
            break;
        case ARBSIZE_32:
            *cw = DMACHCTL_ARBSIZE_32(*cw);
            break;
        case ARBSIZE_64:
            *cw = DMACHCTL_ARBSIZE_64(*cw);
            break;
        case ARBSIZE_128:
            *cw = DMACHCTL_ARBSIZE_128(*cw);
            break;
        case ARBSIZE_256:
            *cw = DMACHCTL_ARBSIZE_256(*cw);
            break;
        case ARBSIZE_512:
            *cw = DMACHCTL_ARBSIZE_512(*cw);
            break;
        case ARBSIZE_1024:
            *cw = DMACHCTL_ARBSIZE_1024(*cw);
            break;
        default:
            break;
    }
}

static void set_control_word_xfersize(uint16_t size, uint32_t *cw) {
    size &= 0x3FF;
    *cw = DMACHCTL_XFERSIZE(*cw, size);
}

static void set_control_word_nxtuseburst(enum DMACHCTL_NXTUSEBURSTMODE mode, uint32_t *cw) {
    switch (mode) {
        case BURSTMODE_EN:
            *cw = DMACHCTL_NXTUSEBURST_ENABLE(*cw);
            break;
        case BURSTMODE_DIS:
            *cw = DMACHCTL_NXTUSEBURST_DISABLE(*cw);
            break;
        default:
            break;
    }
}

static void set_control_word_xfermode(enum DMACHCTL_TRANSFER_MODE mode, uint32_t *cw) {
    switch (mode) {
        case TRANSFER_MODE_STOP:
            *cw = DMACHCTL_XFERMODE_STOP(*cw);
            break;
        case TRANSFER_MODE_BASIC:
            *cw = DMACHCTL_XFERMODE_BASIC(*cw);
            break;
        case TRANSFER_MODE_AUTO:
            *cw = DMACHCTL_XFERMODE_AUTO(*cw);
            break;
        case TRANSFER_MODE_PINGPONG:
            *cw = DMACHCTL_XFERMODE_PINGPONG(*cw);
            break;
        case TRANSFER_MODE_MEMSCATGAT:
            *cw = DMACHCTL_XFERMODE_MEMSCATGAT(*cw);
            break;
        case TRANSFER_MODE_ALTSCATGAT:
            *cw = DMACHCTL_XFERMODE_ALTSCATGAT(*cw);
            break;
        case TRANSFER_MODE_PERIPHSCATGAT:
            *cw = DMACHCTL_XFERMODE_PERIPHSCATGAT(*cw);
            break;
        case TRANSFER_MODE_ALTPERIPHSCATGAT:
            *cw = DMACHCTL_XFERMODE_ALTPERIPHSCATGAT(*cw);
            break;
    }
}
