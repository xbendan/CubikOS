#define USER_SPACE              0x0000000000000000
#define KERNEL_ADDR_SPACE       0xFFFFFF8000000000 
#define KERNEL_PHYSICAL_PAGES   0xFFFFFF8000000000 // ~0xFFFFFF8FFFFFFFFF 64GiB
#define KERNEL_VIRTUAL_PAGES    0xFFFFFF80A0000000 // ~0xFFFF80A1FFFFFFFF 8GiB
#define KERNEL_FRAMEBUFFER_BASE 0xFFFFFF80B0000000 // ~0xFFFF80B000FFFFFF 4GiB

#ifndef KERNEL_VIRTUAL_BASE
#define KERNEL_VIRTUAL_BASE     0xFFFFFFFF80000000
#endif