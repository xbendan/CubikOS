global stack_bottom
global stack_top

section .bss
align 4096
stack_bottom:
    resb 4096 * 8
    ;skip 32768 # 32 KiB
stack_top: