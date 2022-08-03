#include <stdint.h>

static inline uint8_t port_readin_byte(uint16_t port)
{
    uint8_t data;
    asm volatile("inb %1, %0"
                 : "=a"(data)
                 : "d"(port));
    return data;
}

static inline uint16_t port_readin_word(uint16_t port)
{
    uint16_t data;
    asm volatile("inw %1, %0"
                 : "=a"(data)
                 : "d"(port));
    return data;
}

static inline uint32_t port_readin_dword(uint16_t port)
{
    uint32_t data;
    asm volatile("inl %1, %0"
                 : "=a"(data)
                 : "d"(port));
    return data;
}

static inline void port_writeout_byte(uint16_t port, uint8_t data)
{
    asm volatile("outb %0, %1"
                 :
                 : "a"(data), "d"(port));
}

static inline void port_writeout_word(uint16_t port, uint16_t data)
{
    asm volatile("outw %0, %1"
                 :
                 : "a"(data), "d"(port));
}

static inline void port_writeout_dword(uint16_t port, uint32_t data)
{
    asm volatile("outl %0, %1"
                 :
                 : "a"(data), "d"(port));
}