global _start
global exception
extern __stack_top
extern __stack_bottom
extern kload_multiboot2
extern kload_stivale2
extern KernelEntry

section .text
bits 32
_start:
    cli                              ; Clear interrupts, we don't want them as long as we are in the loader
	mov esp, __stack_top           ; Set up a valid stack
	mov ebp, __stack_top

    call check_multiboot
    call check_cpuid
    call check_long_mode

	push ebx                      ; Push pointer to multiboot info structure
    call kload_multiboot2
    hlt

check_multiboot:
    cmp eax, 0x36D76289
    jne .check_multiboot_failed
    ret
.check_multiboot_failed:
    mov al, "M"
    jmp exception

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
	je .no_cpuid
	ret
.no_cpuid:
	mov al, "C"
	jmp exception

check_long_mode:
	mov eax, 0x80000000
	cpuid
	cmp eax, 0x80000001
	jb .no_long_mode

	mov eax, 0x80000001
	cpuid
	test edx, 1 << 29
	jz .no_long_mode
	
	ret
.no_long_mode:
	mov al, "L"
	jmp exception

exception:
    ; print "ERR: X" where X is the error code
	mov dword [0xb8000], 0x4f524f45
	mov dword [0xb8004], 0x4f3a4f52
	mov dword [0xb8008], 0x4f204f20
	mov byte  [0xb800a], al
	hlt

section .data
align 16
GDT64:
    .Null: equ $ - GDT64
    dw 0xFFFF
    dw 0
    db 0
    db 0
    db 0
    db 0
    .Code: equ $ - GDT64
    dw 0
    dw 0
    db 0
    db 10011010b
    db 00100000b
    db 0
    .Data: equ $ - GDT64
    dw 0
    dw 0
    db 0
    db 10010010b
    db 00000000b
    db 0
    .UserCode: equ $ - GDT64
    dw 0
    dw 0
    db 0
    db 11111010b
    db 00100000b
    db 0
    .UserData: equ $ - GDT64
    dw 0
    dw 0
    db 0
    db 11110010b
    db 00000000b
    db 0
    .TSS: ;equ $ - GDT64         ; TSS Descriptor
    .len:
    dw 108                       ; TSS Length - the x86_64 TSS is 108 bytes loong
    .low:
    dw 0                         ; Base (low).
    .mid:
    db 0                         ; Base (middle)
    db 10001001b                 ; Flags
    db 00000000b                 ; Flags 2
    .high:
    db 0                         ; Base (high).
    .high32:
    dd 0                         ; High 32 bits
    dd 0                         ; Reserved
	.PTR:                    ; The GDT-pointer.
    dw $ - GDT64 - 1             ; Limit.
    dq GDT64                     ; Base.
