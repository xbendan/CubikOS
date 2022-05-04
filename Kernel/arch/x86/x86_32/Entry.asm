extern kload_multiboot2
extern stack_top
global _boot_fatal
global start_mb32

bits 32
section .boot.data
GDT32:

section .text
start_mb32:
    cli
    cld

    mov esp, stack_top
    xor ebp, ebp

    ; Save multiboot information from registers
    ; mov dword [mb_addr], ebx
    ; mov dword [mb_magic], eax

    ; call check_cpuid

    push eax
    push ebx
    call kload_multiboot2

    cli
    hlt

check_cpuid:
    pushfd
    pop eax
    mov ecx, eax
    xor eax, 1 << 21
	push eax
	popfd
	pushfd
	pop eax
	push ecx
	popfd
	cmp eax, ecx
    je .fail
    ret
.fail:
    mov al, "0"
    jmp _boot_fatal

_boot_fatal:
    mov dword [0xb8000], 0x4f524f45
	mov dword [0xb8004], 0x4f3a4f52
	mov dword [0xb8008], 0x4f204f20
	mov byte  [0xb800a], al
	hlt

mb_addr:
    dd 0
    dd 0
mb_magic:
    dd 0