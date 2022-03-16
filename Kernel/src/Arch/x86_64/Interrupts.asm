extern __exception
extern __fatal

bits 64
%macro intr_name 1
dq isr_stub_%1
%endmacro

%macro isr_err_stub 1
isr_stub_%+%1:
    push  %1
    jmp __interrupt_fatal
    iretq
%endmacro
; if writing for 64-bit, use iretq instead
%macro isr_no_err_stub 1
isr_stub_%+%1:
    push  0
    push  %1
    jmp __interrupt_common
    iretq
%endmacro

%macro __pusha 0
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

%macro __popa 0
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

__interrupt_common:
    cld
    __pusha
    mov rdi, rsp
    call __exception
    mov rsp, rax
    __popa
    add rsp, 16 ; pop errcode and int number
    iretq

__interrupt_fatal:
    cld
    __pusha
    mov rdi, rsp
    call __fatal
    mov rsp, rax
    __popa
    add rsp, 16 ; pop errcode and int number
    iretq

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

isr_no_err_stub 32
isr_no_err_stub 33
isr_no_err_stub 34
isr_no_err_stub 35
isr_no_err_stub 36
isr_no_err_stub 37
isr_no_err_stub 38
isr_no_err_stub 39
isr_no_err_stub 40
isr_no_err_stub 41
isr_no_err_stub 42
isr_no_err_stub 43
isr_no_err_stub 44
isr_no_err_stub 45
isr_no_err_stub 46
isr_no_err_stub 47

isr_no_err_stub 127
isr_no_err_stub 128

section .rodata
global isr_stub_table
isr_stub_table:
    intr_name 0
    intr_name 1
    intr_name 2
    intr_name 3
    intr_name 4
    intr_name 5
    intr_name 6
    intr_name 7
    intr_name 8
    intr_name 9
    intr_name 10
    intr_name 11
    intr_name 12
    intr_name 13
    intr_name 14
    intr_name 15
    intr_name 16
    intr_name 17
    intr_name 18
    intr_name 19
    intr_name 20
    intr_name 21
    intr_name 22
    intr_name 23
    intr_name 24
    intr_name 25
    intr_name 26
    intr_name 27
    intr_name 28
    intr_name 29
    intr_name 30
    intr_name 31
    intr_name 32
    intr_name 33
    intr_name 34
    intr_name 35
    intr_name 36
    intr_name 37
    intr_name 38
    intr_name 39
    intr_name 40
    intr_name 41
    intr_name 42
    intr_name 43
    intr_name 44
    intr_name 45
    intr_name 46
    intr_name 47
    
    intr_name 127
    intr_name 128