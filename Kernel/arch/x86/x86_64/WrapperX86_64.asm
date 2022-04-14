global __flushGDT
global __flushIDT
global __loadPaging

__flushGDT:
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

__flushIDT:
    lidt [rdi]
    ret


__loadPaging:
    mov cr3, rdi
    ret