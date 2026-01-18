#include "pic.h"
#include "stdint.h"
#include "../drivers/io.h"

typedef enum {
    PIC_MASTER = 0x20,
    PIC_SLAVE = 0xA0,	
    PIC_MASTER_DATA = 0x21,
    PIC_SLAVE_DATA = 0xA1
} PIC_interface_type;

typedef enum {
    PIC_ICW1_ICW4 = 0x01,
    PIC_ICW1_SINGLE = 0x02,
    PIC_ICW1_INTERVAL4 = 0x04,
    PIC_ICW1_LEVEL = 0x08,
    PIC_ICW1_INIT = 0x10,

    PIC_ICW4_8086 = 0x01,
    PIC_ICW4_AUTO = 0x02,
    PIC_ICW4_BUF_SLAVE = 0x08,
    PIC_IC4_BUF_MASTER = 0x0C,
    PIC_ICW4_SFNM = 0x10,

    PIC_READ_IRR = 0x0a,
    PIC_READ_RSR = 0x0b,

    PIC_EOI = 0x20
} PIC_instruction_type;


void remap_pic(int offset1, int offset2){
    outb(PIC_MASTER, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    io_wait();
    outb(PIC_SLAVE, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    io_wait();

    outb(PIC_MASTER_DATA, offset1);
    io_wait();
    outb(PIC_SLAVE_DATA, offset2);
    io_wait();

    outb(PIC_MASTER_DATA, 1 << 2);
    io_wait();
    outb(PIC_SLAVE_DATA, 2);
    io_wait();

    outb(PIC_MASTER_DATA, PIC_ICW4_8086);
    io_wait();
    outb(PIC_SLAVE_DATA, PIC_ICW4_8086);

    outb(PIC_MASTER_DATA, 0x00);   // Unmasks master PIC
    outb(PIC_SLAVE_DATA, 0x00);    // Unmasks slave PIC
}


void disable_pic(){
    outb(PIC_MASTER_DATA, 0x00);
    io_wait();
    outb(PIC_SLAVE_DATA, 0x00);
}

void IRQ_set_mask(uint8_t IRQline){
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) port = PIC_MASTER_DATA;
    else port = PIC_SLAVE_DATA;

    value = inb(port) | (1 << IRQline);
    outb(port, value);
}


void PIC_sendEOI(uint32_t irq)
{
	if((irq & 0xffff) >= 8)
		outb(PIC_SLAVE, PIC_EOI);
	
	outb(PIC_MASTER, PIC_EOI);
}