CC = ${TARGET}-gcc
CXX = ${TARGET}-g++
AS = ${TARGET}-as
OC = ${TARGET}-objcopy
NASM = nasm -f elf64

KERNEL_CFLAGS = -ffreestanding \
	-O2 \
	-std=gnu11 \
	-g \
	-static \
	-mcmodel=large \
	-Wall \
	-Wextra \
	-Wno-unused-function \
	-Wno-unused-parameter \
	-Wwrite-strings \
	-mno-red-zone \
	-fno-omit-frame-pointer \
	-mfsgsbase \
	-mgeneral-regs-only \
	-z max-page-size=0x1000 \
	-nostdlib

KERNEL_LDFLAGS = -nostdlib \
	-O2 \
	-z max-page-size=0x1000

ARCH_SHARED_ASM_FILES_X86_IN	:= 	$(shell find ./arch/x86 -maxdepth 1 -name *.asm)
ARCH_SHARED_C_FILES_X86_IN 		:= 	$(shell find ./arch/x86 -maxdepth 1 -name *.c)
ARCH_ASM_FILES_X64_IN 			:=  $(shell find ./arch/x86/amd64 -name *.asm)
ARCH_C_FILES_X64_IN 			:=  $(shell find ./arch/x86/amd64 -name *.c)

ARCH_SHARED_ASM_FILES_X86_OUT 	:=  $(patsubst ./arch/x86/%.asm, ./build/x86/%.o, $(ARCH_SHARED_ASM_FILES_X86_IN))
ARCH_SHARED_C_FILES_X86_OUT   	:=  $(patsubst ./arch/x86/%.c, ./build/x86/%.o, $(ARCH_SHARED_C_FILES_X86_IN))
ARCH_ASM_FILES_X64_OUT			:=  $(patsubst ./arch/x86/amd64/%.asm, ./build/x86_amd64/%.o, $(ARCH_ASM_FILES_X64_IN))
ARCH_C_FILES_X64_OUT			:=  $(patsubst ./arch/x86/amd64/%.c, ./build/x86_amd64/%.o, $(ARCH_C_FILES_X64_IN))

ARCH_x86_amd64_OUTPUT 			= 	$(ARCH_ASM_FILES_X64_OUT) $(ARCH_C_FILES_X64_OUT) $(ARCH_SHARED_ASM_FILES_X86_OUT) $(ARCH_SHARED_C_FILES_X86_OUT)

KERNEL_INCLUDES = -Iinclude -Iarch

KERNEL_FILES_IN 				:= $(shell find ./src -name *.c)
KERNEL_FILES_OUT				:= $(patsubst ./src/%.c, ./build/kernel/%.o, $(KERNEL_FILES_IN))

$(ARCH_SHARED_ASM_FILES_X86_OUT): build/x86/%.o : arch/x86/%.asm
	mkdir -p $(dir $@) && \
	$(NASM) $(patsubst build/x86/%.o, arch/x86/%.asm, $@) -o $@
$(ARCH_SHARED_C_FILES_X86_OUT): build/x86/%.o : arch/x86/%.c
	mkdir -p $(dir $@) && \
	$(CC) $(KERNEL_INCLUDES) $(KERNEL_CFLAGS) $(patsubst build/x86/%.o, arch/x86/%.c, $@) -o $@
$(ARCH_ASM_FILES_X64_OUT): build/x86_amd64/%.o : arch/x86/amd64/%.asm
	mkdir -p $(dir $@) && \
	$(NASM) $(patsubst build/x86_amd64/%.o, arch/x86/amd64/%.asm, $@) -o $@
$(ARCH_C_FILES_X64_OUT): build/x86_amd64/%.o : arch/x86/amd64/%.c
	mkdir -p $(dir $@) && \
	$(CC) $(KERNEL_INCLUDES) $(KERNEL_CFLAGS) $(patsubst build/x86_amd64/%.o, arch/x86/amd64/%.c, $@) -o $@

$(KERNEL_FILES_OUT): build/kernel/%.o : src/%.c
	mkdir -p $(dir $@) && \
	$(CC) $(KERNEL_INCLUDES) $(KERNEL_CFLAGS) $(patsubst build/kernel/%.o, src/%.c, $@) -o $@

.PHONY: build
build: $(ARCH_x86_amd64_OUTPUT) $(KERNEL_FILES_OUT)
	mkdir -p ./dist/${PLATFORM}
	mkdir -p ./build/${PLATFORM}
	mkdir -p ./build/kernel
	$(CC) $(KERNEL_CFLAGS) $(KERNEL_LDFLAGS) -n -o ./dist/${PLATFORM}/atrikrnl.elf -O2 -T ./target/linkerscript-${PLATFORM}.ld $(ARCH_x86_amd64_OUTPUT) $(KERNEL_FILES_OUT)
	cp ./dist/${PLATFORM}/atrikrnl.elf ./target/${PLATFORM}/iso_limine/atrikrnl.elf
	xorriso -as mkisofs -b limine-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot limine-cd-efi.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        ./target/${PLATFORM}/iso_limine -o ./dist/${PLATFORM}/cubik.iso
	../../limine/bin/limine-deploy ./dist/${PLATFORM}/cubik.iso
	cp ./dist/${PLATFORM}/cubik.iso "/mnt/f/CubikOSProject/dist/${PLATFORM}/cubik.iso"

.PHONY: run
run:
	qemu-system-x86_64.exe -cdrom F:/CubikOSProject/dist/${PLATFORM}/cubik.iso -m 512m

.PHONY: clean
clean:
	rm -rf ./build
	rm -rf ./dist
