%macro INTR_NAME 1
dd __interrupt%1
%endmacro

%macro INTR_ERR 1
__interrupt%1:
    hlt
    push %1
    jmp __IntrCommonHandler
%endmacro

%macro INTR_NO_ERR 1
__interrupt%1:
    hlt
    push 0
    push %1
    jmp __IntrCommonHandler
%endmacro

%macro INTR_SYSCALL 1
__interrupt%1:
    push 0
    push %1
    jmp __IntrCommonHandler
%endmacro

%macro _PUSHA_ 0
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro _POPA_ 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

extern DispatchInterrupt

__IntrCommonHandler:
    cld

    pushad

    push ds
    push es
    push fs
    push gs

    mov ax, 0x10

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call DispatchInterrupt
    mov esp, eax

    pop gs
    pop fs
    pop es
    pop ds

    popad

    add esp, 8 ; pop errcode and int number

    iret

INTR_NO_ERR 0
INTR_NO_ERR 1
INTR_NO_ERR 2
INTR_NO_ERR 3
INTR_NO_ERR 4
INTR_NO_ERR 5
INTR_NO_ERR 6
INTR_NO_ERR 7
INTR_ERR   8
INTR_NO_ERR 9
INTR_ERR   10
INTR_ERR   11
INTR_ERR   12
INTR_ERR   13
INTR_ERR   14
INTR_NO_ERR 15
INTR_NO_ERR 16
INTR_ERR 17
INTR_NO_ERR 18
INTR_NO_ERR 19
INTR_NO_ERR 20
INTR_NO_ERR 21
INTR_NO_ERR 22
INTR_NO_ERR 23
INTR_NO_ERR 24
INTR_NO_ERR 25
INTR_NO_ERR 26
INTR_NO_ERR 27
INTR_NO_ERR 28
INTR_NO_ERR 29
INTR_ERR   30
INTR_NO_ERR 31

INTR_NO_ERR 32
INTR_NO_ERR 33
INTR_NO_ERR 34
INTR_NO_ERR 35
INTR_NO_ERR 36
INTR_NO_ERR 37
INTR_NO_ERR 38
INTR_NO_ERR 39
INTR_NO_ERR 40
INTR_NO_ERR 41
INTR_NO_ERR 42
INTR_NO_ERR 43
INTR_NO_ERR 44
INTR_NO_ERR 45
INTR_NO_ERR 46
INTR_NO_ERR 47

INTR_NO_ERR 127
INTR_SYSCALL 128

global isrTable

isrTable:
    INTR_NAME 0
    INTR_NAME 1
    INTR_NAME 2
    INTR_NAME 3
    INTR_NAME 4
    INTR_NAME 5
    INTR_NAME 6
    INTR_NAME 7
    INTR_NAME 8
    INTR_NAME 9
    INTR_NAME 10
    INTR_NAME 11
    INTR_NAME 12
    INTR_NAME 13
    INTR_NAME 14
    INTR_NAME 15
    INTR_NAME 16
    INTR_NAME 17
    INTR_NAME 18
    INTR_NAME 19
    INTR_NAME 20
    INTR_NAME 21
    INTR_NAME 22
    INTR_NAME 23
    INTR_NAME 24
    INTR_NAME 25
    INTR_NAME 26
    INTR_NAME 27
    INTR_NAME 28
    INTR_NAME 29
    INTR_NAME 30
    INTR_NAME 31

    INTR_NAME 32
    INTR_NAME 33
    INTR_NAME 34
    INTR_NAME 35
    INTR_NAME 36
    INTR_NAME 37
    INTR_NAME 38
    INTR_NAME 39
    INTR_NAME 40
    INTR_NAME 41
    INTR_NAME 42
    INTR_NAME 43
    INTR_NAME 44
    INTR_NAME 45
    INTR_NAME 46
    INTR_NAME 47

    INTR_NAME 127
    INTR_NAME 128
