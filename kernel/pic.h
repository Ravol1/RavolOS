#include "stdint.h"

void remap_pic(int offset1, int offset2);
void disable_pic(void);
void IRQ_set_mask(uint8_t IRQline);
void PIC_sendEOI(uint32_t irq);
