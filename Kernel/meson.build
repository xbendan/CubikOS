add_languages('c', 'cpp')

KERNEL_CFLAGS = [
    '-Wno-write-strings', 
    '-Wno-unused-parameter', 
    '-Wno-sign-compare',
    '-Damd64',
    '-ffreestanding', 
    '-nostdlib',
    '-mcmodel=large', 
    '-mno-red-zone', 
    '-fno-pic',
    '-mno-mmx', 
    '-mno-sse', 
    '-mno-sse2',
    '-fno-stack-protector', 
    '-fno-builtin',
    '-finline-functions',
]

KERNEL_INCLUDES = [
    include_directory('arch'),
    include_directory('include')
]

KERNEL_CFILES = [
    'src/mem/page.c',
    'src/utils/list.c',
    'src/panic.c',
    'src/startup.c'
]

KERNEL_CFILES_AMD64 = [
    'arch/x86/amd64/bootx64.c',
    'arch/x86/amd64/gdt.c',
    'arch/x86/amd64/idt.c',
    'arch/x86/amd64/paging.c'
]

KERNEL_ASMFILES_AMD64 = [
    'arch/x86/amd64/boot.asm',
    'arch/x86/amd64/header.asm',
    'arch/x86/amd64/interrupts.asm',
    'arch/x86/amd64/stack.asm',
    'arch/x86/amd64/wrapper.asm'
]

KERNEL_LDFLAGS = [
    '-m64',
    '-T', 'target/linkerscript-x86_amd64.ld',
    '-z', 'max-page-size=0x1000'
]