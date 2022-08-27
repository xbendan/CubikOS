CC = ${TARGET}-elf-gcc
CXX = ${TARGET}-elf-g++
AS = ${TARGET}-elf-as
OC = ${TARGET}-elf-objcopy
LD = ${TARGET}-elf-ld
NASM = nasm -f elf64

KERNEL_CFLAGS = -ffreestanding \
	-DARCH_X86_64 \
	-D__DEBUG__ \
	-O2 \
	-std=gnu11 \
	-g \
	-static \
	-mcmodel=large \
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

ARCH_ASM_FILES_X64_IN 			:=  $(shell find ./arch/x86_64 -name *.asm)
ARCH_C_FILES_X64_IN 			:=  $(shell find ./arch/x86_64 -name *.c)

ARCH_ASM_FILES_X64_OUT			:=  $(patsubst ./arch/x86_64/%.asm, ./build/x86_64/%.o, $(ARCH_ASM_FILES_X64_IN))
ARCH_C_FILES_X64_OUT			:=  $(patsubst ./arch/x86_64/%.c, ./build/x86_64/%.o, $(ARCH_C_FILES_X64_IN))

ARCH_x86_64_OUTPUT 				= 	$(ARCH_ASM_FILES_X64_OUT) $(ARCH_C_FILES_X64_OUT)

KERNEL_INCLUDES = -Iinclude -Iarch

KERNEL_FILES_IN 				:= $(shell find ./src -name *.c)
KERNEL_FILES_OUT				:= $(patsubst ./src/%.c, ./build/kernel/%.o, $(KERNEL_FILES_IN))

$(ARCH_ASM_FILES_X64_OUT): build/x86_64/%.o : arch/x86_64/%.asm
	mkdir -p $(dir $@) && \
	$(NASM) $(patsubst build/x86_64/%.o, arch/x86_64/%.asm, $@) -o $@
$(ARCH_C_FILES_X64_OUT): build/x86_64/%.o : arch/x86_64/%.c
	mkdir -p $(dir $@) && \
	$(CC) $(KERNEL_INCLUDES) $(KERNEL_CFLAGS) -c $(patsubst build/x86_64/%.o, arch/x86_64/%.c, $@) -o $@

$(KERNEL_FILES_OUT): build/kernel/%.o : src/%.c
	mkdir -p $(dir $@) && \
	$(CC) $(KERNEL_INCLUDES) $(KERNEL_CFLAGS) -c $(patsubst build/kernel/%.o, src/%.c, $@) -o $@

.PHONY: build
build: $(ARCH_x86_64_OUTPUT) $(KERNEL_FILES_OUT)
	mkdir -p ./dist/${TARGET}
	mkdir -p ./build/${TARGET}
	mkdir -p ./build/kernel
	$(CC) $(KERNEL_CFLAGS) $(KERNEL_LDFLAGS) -n -o ./dist/${TARGET}/atrikrnl.elf -O2 -T ./target/linkerscript-${TARGET}.ld $(ARCH_x86_64_OUTPUT) $(KERNEL_FILES_OUT)
	cp ./dist/${TARGET}/atrikrnl.elf ./target/${TARGET}/iso_limine/atrikrnl.elf
	xorriso -as mkisofs -b limine-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot limine-cd-efi.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        ./target/${TARGET}/iso_limine -o ./dist/${TARGET}/cubik.iso
	../../limine/bin/limine-deploy ./dist/${TARGET}/cubik.iso
	cp ./dist/${TARGET}/cubik.iso "/mnt/f/CubikOSProject/dist/${TARGET}/cubik.iso"

.PHONY: run
run:
	qemu-system-x86_64.exe -smp cpus=2,cores=2 -cdrom F:/CubikOSProject/dist/${TARGET}/cubik.iso -m 512m

.PHONY: clean
clean:
	rm -rf ./build
	rm -rf ./dist