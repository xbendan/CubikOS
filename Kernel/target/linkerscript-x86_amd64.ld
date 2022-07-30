OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(start_mb32)
SECTIONS
{
    KERNEL_VMA = 0xFFFFFFFF80000000;
    . = 0x100000;

    .boot.text : {
        *(.boot.text)
    }

    .boot.data : {
        *(.boot.data)
        . = ALIGN(4096);
    }

    . += KERNEL_VMA;
    KERNEL_START_ADDR = .;
    KERNEL_END_ADDR = .;

    .stivale2hdr : AT(ADDR(.stivale2hdr) - KERNEL_VMA)
    {
        KEEP(*(.stivale2hdr))
    }

    .text : AT(ADDR(.text) - KERNEL_VMA)
    {
        *(.text)
        . = ALIGN(4096);
    }

    .rodata : AT(ADDR(.rodata) - KERNEL_VMA)
    {
        *(.rodata)
        . = ALIGN(4096);
    }

    .data : AT(ADDR(.data) - KERNEL_VMA)
    {
        *(.data)
        . = ALIGN(4096);
    }

    .eh_frame : AT(ADDR(.eh_frame) - KERNEL_VMA)
    {
        *(.eh_frame)
        . = ALIGN(4096);
    }

   .bss : AT(ADDR(.bss) - KERNEL_VMA)
    {
        _bss = .;
        *(.bss)

        *(COMMON)
        . = ALIGN(4096);
        _bss_end = .;
    }

    /DISCARD/ :
    {
        *(.comment)
    }
}