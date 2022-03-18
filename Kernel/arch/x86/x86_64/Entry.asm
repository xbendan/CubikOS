bits 32
section .boot.data
GDT64:
    .Null:

section .text
start_mb32:
    cli ; Disable interrupts

    ; Save multiboot information from registers
    mov dword [mb_addr], ebx
    mov dword [mb_magic], eax

    call check_cpuid
    call check_long_mode

    ; Set PAE bit
    mov eax, 10100000b
    mov cr4, eax

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

check_long_mode:
    mov eax, 0x80000000
	cpuid
	cmp eax, 0x80000001
	jb .fail
	mov eax, 0x80000001
	cpuid
	test edx, 1 << 29
	jz .fail
	ret
.fail:
	mov al, "1"
	jmp _boot_fatal

_boot_fatal:
    mov dword [0xb8000], 0x4f524f45
	mov dword [0xb8004], 0x4f3a4f52
	mov dword [0xb8008], 0x4f204f20
	mov byte  [0xb800a], al
	hlt

bits 64
mb_addr:
    dd 0
    dd 0
mb_magic:
    dd 0

start_jmp64:
