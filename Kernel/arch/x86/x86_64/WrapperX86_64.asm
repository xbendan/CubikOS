global asmw_flush_gdt
global asmw_flush_idt
global asmw_load_paging

asmw_flush_gdt:
    lgdt [rdi]
    push rbp
    mov rbp, rsp

    push qword 0x10
    push rbp
    pushf
    push qword 0x8
    push .trampoline
    iretq
.trampoline:
    pop rbp

    mov ax, 0x10

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov ax, 0x28
    ltr ax

    ret

asmw_flush_idt:
    lidt [rdi]
    ret


asmw_load_paging:
    mov cr3, rdi
    ret