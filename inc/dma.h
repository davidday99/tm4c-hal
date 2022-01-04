#ifndef _DMA_H_
#define _DMA_H_

#include <stdint.h>
#include "common.h"

#define DMACHCTL_DSTINC_BYTE(x) (x & ~0xC0000000)
#define DMACHCTL_DSTINC_HALFWORD(x) (DMACHCTL_DSTINC_BYTE(x) | 0x40000000)
#define DMACHCTL_DSTINC_WORD(x) (DMACHCTL_DSTINC_BYTE(x) | 0x80000000)
#define DMACHCTL_DSTINC_NOINC(x) (DMACHCTL_DSTINC_BYTE(x) | 0xC0000000)

#define DMACHCTL_DSTSIZE_BYTE(x) (x & ~0x30000000)
#define DMACHCTL_DSTSIZE_HALFWORD(x) (DMACHCTL_DSTSIZE_BYTE(x) | 0x10000000)
#define DMACHCTL_DSTSIZE_WORD(x) (DMACHCTL_DSTSIZE_BYTE(x) | 0x20000000)

#define DMACHCTL_SRCINC_BYTE(x) (x & ~0xC000000)
#define DMACHCTL_SRCINC_HALFWORD(x) (DMACHCTL_SRCINC_BYTE(x) | 0x4000000)
#define DMACHCTL_SRCINC_WORD(x) (DMACHCTL_SRCINC_BYTE(x) | 0x8000000)
#define DMACHCTL_SRCINC_NOINC(x) (DMACHCTL_SRCINC_BYTE(x) | 0xC000000)

#define DMACHCTL_SRCSIZE_BYTE(x) (x & ~0x3000000)
#define DMACHCTL_SRCSIZE_HALFWORD(x) (DMACHCTL_SRCSIZE_BYTE(x) | 0x1000000)
#define DMACHCTL_SRCSIZE_WORD(x) (DMACHCTL_SRCSIZE_BYTE(x) | 0x2000000)

#define DMACHCTL_ARBSIZE_1(x) (x & ~0x3C000)
#define DMACHCTL_ARBSIZE_2(x) (DMACHCTL_ARBSIZE_1(x) | 0x4000)
#define DMACHCTL_ARBSIZE_4(x) (DMACHCTL_ARBSIZE_1(x) | 0x8000)
#define DMACHCTL_ARBSIZE_8(x) (DMACHCTL_ARBSIZE_1(x) | 0xC000)
#define DMACHCTL_ARBSIZE_16(x) (DMACHCTL_ARBSIZE_1(x) | 0x10000)
#define DMACHCTL_ARBSIZE_32(x) (DMACHCTL_ARBSIZE_1(x) | 0x14000)
#define DMACHCTL_ARBSIZE_64(x) (DMACHCTL_ARBSIZE_1(x) | 0x18000)
#define DMACHCTL_ARBSIZE_128(x) (DMACHCTL_ARBSIZE_1(x) | 0x1C000)
#define DMACHCTL_ARBSIZE_256(x) (DMACHCTL_ARBSIZE_1(x) | 0x20000)
#define DMACHCTL_ARBSIZE_512(x) (DMACHCTL_ARBSIZE_1(x) | 0x24000)
#define DMACHCTL_ARBSIZE_1024(x) (DMACHCTL_ARBSIZE_1(x) | 0x28000)

#define DMACHCTL_XFERSIZE(x, size) ((x & ~0x3FF0) | ((size - 1) << 4))

#define DMACHCTL_NXTUSEBURST_ENABLE(x) (x | 8)
#define DMACHCTL_NXTUSEBURST_DISABLE(x) (x & ~8)

#define DMACHCTL_XFERMODE_STOP(x) (x & ~7)
#define DMACHCTL_XFERMODE_BASIC(x) (DMACHCTL_XFERMODE_STOP(x) | 1)
#define DMACHCTL_XFERMODE_AUTO(x) (DMACHCTL_XFERMODE_STOP(x) | 2)
#define DMACHCTL_XFERMODE_PINGPONG(x) (DMACHCTL_XFERMODE_STOP(x) | 3)
#define DMACHCTL_XFERMODE_MEMSCATGAT(x) (DMACHCTL_XFERMODE_STOP(x) | 4)
#define DMACHCTL_XFERMODE_ALTSCATGAT(x) (DMACHCTL_XFERMODE_STOP(x) | 5)
#define DMACHCTL_XFERMODE_PERIPHSCATGAT(x) (DMACHCTL_XFERMODE_STOP(x) | 6)
#define DMACHCTL_XFERMODE_ALTPERIPHSCATGAT(x) (DMACHCTL_XFERMODE_STOP(x) | 7)

struct DMA {
    volatile uint32_t *DMASTAT;
    volatile uint32_t *DMACFG;
    volatile uint32_t *DMACTLBASE;
    volatile uint32_t *DMAALTBASE;
    volatile uint32_t *DMASWREQ;
    volatile uint32_t *DMAUSEBURSTSET;
    volatile uint32_t *DMAUSEBURSTCLR;
    volatile uint32_t *DMAREQMASKSET;
    volatile uint32_t *DMAREQMASKCLR;
    volatile uint32_t *DMAENASET;
    volatile uint32_t *DMAENACLR;
    volatile uint32_t *DMAALTSET;
    volatile uint32_t *DMAALTCLR;
    volatile uint32_t *DMAPRIOSET;
    volatile uint32_t *DMAPRIOCLR;
    volatile uint32_t *DMAERRCLR;
    volatile uint32_t *DMACHIS;
    volatile uint32_t *DMACHMAP0;
    volatile uint32_t *DMACHMAP1;
    volatile uint32_t *DMACHMAP2;
    volatile uint32_t *DMACHMAP3;
};

struct DMA_CONTROL_STRUCTURE_T {
    uint8_t *srcendptr;
    uint8_t *destendptr;
    uint32_t ctrlword;
    uint32_t unused;
};

#define DMA_NUMBER_OF_CHANNELS 32

void init_DMACHn(enum DMA_CHANNEL ch, uint8_t priority);
void select_DMACHn_request_mode(enum DMA_CHANNEL ch, uint8_t burst);
void set_DMACHn_priority(enum DMA_CHANNEL ch, uint8_t pri);
void select_DMACHn_control_structure(enum DMA_CHANNEL ch, uint8_t alt);
uint8_t get_DMA_bus_error_status(void);
void clear_DMA_bus_error_status(void);
void enable_DMACHn_requests(enum DMA_CHANNEL ch);
void disable_DMACHn_requests(enum DMA_CHANNEL ch);
void enable_DMACHn(enum DMA_CHANNEL ch);
void disable_DMACHn(enum DMA_CHANNEL ch);
void issue_DMACHn_software_request(enum DMA_CHANNEL ch);
uint32_t get_DMA_status(void);
void set_DMACHn_src_end(enum DMA_CHANNEL ch, uint8_t *srcendptr);
void set_DMACHn_dest_end(enum DMA_CHANNEL ch, uint8_t *destendptr);
void set_DMACHn_channel_source(enum DMA_CHANNEL ch, uint8_t src);
void set_DMACHn_control_word(enum DMA_CHANNEL ch, enum DMACHCTL_ADDRESS_INC dstinc,
                                enum DMACHCTL_DATA_SIZE dstsize, enum DMACHCTL_ADDRESS_INC srcinc,
                                enum DMACHCTL_DATA_SIZE srcsize, enum DMACHCTL_ARB_SIZE arbsize,
                                uint16_t transfer_size, enum DMACHCTL_NXTUSEBURSTMODE burst_mode,
                                enum DMACHCTL_TRANSFER_MODE transfer_mode);
#endif /* _DMA_H_ */