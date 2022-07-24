OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

ENTRY(start_mb32)

SECTIONS {
    . = 0x100000;

    ld_kmstart = .;

    .boot.text : 
    {
        KEEP(*(.boot.text))
    }

    .boot.data :
    {
        KEEP(*(.boot.data))
        . = ALIGN(4096);
    }

    .text : ALIGN(4096)
    {
        KEEP(*(.text))
    }

    .data : ALIGN(4096)
    {
        KEEP(*(.data))
    }

    .rodata : ALIGN(4096)
    {
        KEEP(*(.rodata))
    }

    .eh_frame : ALIGN(4096)
    {
        *(eh_frame)
    }

    .bss : ALIGN(4096)
    {
        *(.bss)
        *(COMMON)
    }

    ld_kmend = .;

    /DISCARD/ :
    {
        *(.comment)
    }
}

