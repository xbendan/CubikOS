project('Paper OS', default_options : ['warning_level=2', 'cpp_std=c++2a', 'optimization=0', 'werror=true'])
add_languages('c', 'cpp')

add_global_arguments(['-msse3', '-mssse3', '-msse4.1', '-msse4.2', '-mpopcnt', '-mcx16', '-msahf'], language: ['c', 'cpp'])

nasm = find_program('nasm')
asmg = generator(nasm,
    output : '@BASENAME@.asm.o',
    arguments : [
        '-f', 'elf64',
        '-g', '-F', 'dwarf', '-w+gnu-elf-extensions',
        '@INPUT@',
        '-o', '@OUTPUT@'])
