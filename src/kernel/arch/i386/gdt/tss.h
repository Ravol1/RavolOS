#ifndef TSS_H
#define TSS_H


struct tss_entry {
    uint32_t link;
    uint32_t esp0, ss0;
    uint32_t esp1, ss1;
    uint32_t esp2, ss2;
    uint32_t cr3;
    uint32_t eip, eflags;
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs;
    uint32_t ldtr;
    uint32_t iopb;
    uint32_t ssp;
} __attribute__((packed));

typedef struct tss_entry tss_t;

extern tss_t cs_tss;
extern tss_t df_tss;


void tss_init(void);

#endif