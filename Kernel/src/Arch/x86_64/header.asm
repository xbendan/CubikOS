extern kload_stivale2
extern __stack_top

section .multiboot_header
header_start:
	; magic number
	dd 0xe85250d6 ; multiboot2
	; architecture
	dd 0 ; protected mode i386
	; header length
	dd header_end - header_start
	; checksum
	dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

	

	; end tag
	dw 0
	dw 0
	dd 8
header_end:

section .stivale2hdr
bits 64
align 4
__stivale2_header:
	dq kload_stivale2 ; Use a different entry point for stivale2
	dq __stack_top ; Bootloader does not need to set up stack for us
	dq 0
	dq __stivale2_framebuffer_tag

__stivale2_framebuffer_tag:
    dq 0x3ecc1bc43d0f7971
    dq 0
    dw 0
    dw 0
    dw 32