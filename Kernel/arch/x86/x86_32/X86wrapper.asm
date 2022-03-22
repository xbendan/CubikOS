global __flushGDT
__flushGDT:
    mov eax, [esp + 4]
    lgdt [eax]

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov gs, ax
    jmp 0x08:._flushGDT

._flushGDT:
    mov ax, 0x28
    ltr ax
    ret

global __flushIDT
__flushIDT:
    mov eax, [esp + 4]
    lidt [eax]
    ret