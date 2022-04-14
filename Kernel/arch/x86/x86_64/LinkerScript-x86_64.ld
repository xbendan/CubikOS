OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

ENTRY(start_mb32)

SECTIONS {
    . = 0x100000;

    __kmstart__ = .;

    .boot.text : 
    {
        KEEP(*(.boot.text))
    }

    .boot.data :
    {
        KEEP(*(.boot.data))
        . = ALIGN(4096);
    }

    .stivalehdr ALIGN(4K) :
    {
        KEEP(*(.stivalehdr))
    }

    .stivale2hdr ALIGN(4K) :
    {
        KEEP(*(.stivale2hdr))
    }

    .text ALIGN(4K) :
    {
        KEEP(*(.text*))
    }

    .rodata ALIGN(4K) :
    {
        KEEP(*(.rodata*))
    }

    .data ALIGN(4K) :
    {
        KEEP(*(.data*))
    }

    .bss ALIGN(4K) :
    {
        __bss = .;
        KEEP(*(COMMON))
        KEEP(*(.bss*))
        __bss_end = .;
    }

    __kmend__ = .;
}