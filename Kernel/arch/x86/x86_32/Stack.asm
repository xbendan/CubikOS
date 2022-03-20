global stack_top
global stack_bottom

section .bss
align 4096
stack_bottom:
    resb 4096 * 8
stack_top: