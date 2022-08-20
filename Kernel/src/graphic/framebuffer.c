#include <graphic/framebuffer.h>

uint32_t *buffer;

void set_framebuffer(uint64_t address)
{
    buffer = (uint32_t *)(address);
}

void draw_rect()
{
    for (unsigned i = 0; i < 768; i++)
    {
        for (unsigned j = 0; j < 1024; j++)
        {
            buffer[(i * 1024) + j] = 0x33333333;
        }
    }
}