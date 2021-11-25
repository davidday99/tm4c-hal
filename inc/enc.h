#ifndef _ENC_H_
#define _ENC_H_

#include "ethernet.h"

typedef struct ENC {
    void *model;
} ENC;

void enc_init(ENC *enc);
void enc_write_frame(ENC *enc, struct enet_frame *e);
void enc_read_frame(ENC *enc, struct enet_frame *e);

#endif /* _ENC_H_ */