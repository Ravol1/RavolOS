ENTRY(_start)

SECTIONS
{
    . = 0x100000; 
    _kernel_start =.;
    
    .text : ALIGN(4K) {
        _code_start = .;
        *(.multiboot)
        *(.text)
        _code_end = .;
    }
    
    
    .rodata ALIGN(4K) : {
        _rodata_start = .;
        *(.rodata)
        _rodata_end = .;
    }
    
    .data ALIGN(4K) : {
        _data_start = .;
        *(.data)
        _data_end = .;
    }
    
    .bss ALIGN(4K) : {
        _bss_start = .;
        *(.bss)
        *(COMMON)

        . = ALIGN(4K);
        _stack_guard = .;

        . += 4K;

        . = ALIGN(16);
        _stack_bottom = .;
        . += 0x4000;
        _stack_top = .;

        _bss_end = .;
    }
    _kernel_end = .;
}