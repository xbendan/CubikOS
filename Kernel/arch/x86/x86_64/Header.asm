MULTIBOOT_MAGIC       equ 0xE85250D6
MULTIBOOT_ARCH        equ 0 ; x86
MULTIBOOT_LENGTH      equ (multiboot_header_end - multiboot_header_start)
MULTIBOOT_CHECKSUM    equ -(MULTIBOOT_MAGIC + MULTIBOOT_ARCH + MULTIBOOT_LENGTH)

section .boot.text
multiboot_header_start:
    dd MULTIBOOT_MAGIC
    dd MULTIBOOT_ARCH
    dd MULTIBOOT_LENGTH
    dd MULTIBOOT_CHECKSUM

align 8
mb_tags_start:
    dw 1
    dw 0
    dd request_tag_end - request_tag_start
    dd 1 ; require cmd line
    dd 2 ; require bootloader name
    dd 4 ; require basic memory info
    dd 6 ; require memory map
mb_tags_end:

align 8
    ; Module tag
    dw 6
    dw 0
    dd 8

    ; Eng tag
    dw 0
    dw 0
    dd 8
multiboot_header_end: