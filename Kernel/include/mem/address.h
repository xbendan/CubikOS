#define USER_SPACE              0x0000000000000000
#define KERNEL_ADDR_SPACE       0xFFFF800000000000 
#define KERNEL_PHYSICAL_PAGES   0xFFFF808000000000 // ~0xFFFF809FFFFFFFFF 64GiB
#define KERNEL_VIRTUAL_PAGES    0xFFFF80A000000000 // ~0xFFFF80A1FFFFFFFF 8GiB
#define KERNEL_FRAMEBUFFER_BASE 0xFFFF80B000000000 // ~0xFFFF80B000FFFFFF 4GiB

#ifndef KERNEL_VIRTUAL_BASE
#define KERNEL_VIRTUAL_BASE     0xFFFFFFFF80000000
#endif