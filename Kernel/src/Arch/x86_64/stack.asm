global __stack_bottom
global __stack_top

section .bss
align 4096
__stack_bottom:
    resb 4096 * 4
    ;skip 16384 # 16 KiB
__stack_top: