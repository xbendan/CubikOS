#include <x86_64/gdt.h>
#include <x86_64/idt.h>

typedef struct processor
{
    uint32_t id;
    struct gdt_ptr gdtPtr;
    struct idt_ptr idtPtr;
}