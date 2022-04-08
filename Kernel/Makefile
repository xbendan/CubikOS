x86_asm_source := $(shell find ./arch/x86 -maxdepth 1 -name *.asm)
x86_cpp_source := $(shell find ./arch/x86 -maxdepth 1 -name *.cpp)
x86_32_asm_source := $(shell find ./arch/x86/x86_32 -name *.asm)
x86_32_cpp_source := $(shell find ./arch/x86/x86_32 -name *.cpp)
x86_64_asm_source := $(shell find ./arch/x86/x86_64 -name *.asm)
x86_64_cpp_source := $(shell find ./arch/x86/x86_64 -name *.cpp)

x86_asm_object = $(patsubst ./arch/x86/%.asm, ./build/x86/%.o, $(x86_asm_source))
x86_cpp_object = $(patsubst ./arch/x86/%.cpp, ./build/x86/%.o, $(x86_cpp_source))
x86_32_asm_object = $(patsubst ./arch/x86/x86_32/%.asm, ./build/x86/x86_32/%.o, $(x86_32_asm_source))
x86_32_cpp_object = $(patsubst ./arch/x86/x86_32/%.cpp, ./build/x86/x86_32/%.o, $(x86_32_cpp_source))
x86_64_asm_object = $(patsubst ./arch/x86/x86_64/%.asm, ./build/x86/x86_64/%.o, $(x86_64_asm_source))
x86_64_cpp_object = $(patsubst ./arch/x86/x86_64/%.cpp, ./build/x86/x86_64/%.o, $(x86_64_cpp_source))

x86_object = $(x86_asm_object) $(x86_cpp_object)
x86_32_object = $(x86_32_asm_object) $(x86_32_cpp_object) $(x86_object)
x86_64_object = $(x86_64_asm_object) $(x86_64_cpp_object) $(x86_object)

x86_include = -I./arch/x86
x86_32_include = -I./arch/x86 -I./arch/x86/x86_32 
x86_64_include = -I./arch/x86 -I./arch/x86/x86_64 

shared_asm_source := $(shell find ./src -name *.asm)
shared_cpp_source := $(shell find ./src -name *.cpp)

shared_asm_object = $(patsubst ./src/%.asm, build/shared/%.o, $(shared_asm_source))
shared_cpp_object = $(patsubst ./src/%.cpp, build/shared/%.o, $(shared_cpp_source))

shared_object = $(shared_asm_object) $(shared_cpp_object)

shared_include = -Iinclude \
	-Iinclude/Memory \
	-Iinclude/Graphic \
	-Iinclude/FileSystem \
	-Iinclude/Process  \
	-Iinclude/SecurityControl \
	-Iinclude/Device \
	-Iinclude/Interrupts \
	-Iinclude/System \
	-Iinclude/Utils

cppflags = -ffreestanding \
	-fomit-frame-pointer \
	-mno-red-zone \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-std=c++20 \
	-O2 \
	-c \
	-lgcc

ldflags = -nostdlib \
	-O2 \
	-z max-page-size=0x1000

nasmtool = nasm -f elf64
cpptool = x86_64-elf-g++ -march=x86-64 -m64
linktool = x86_64-elf-ld -m elf_x86_64

$(x86_asm_object): build/x86/%.o : arch/x86/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf32 $(patsubst build/x86/%.o, arch/x86/%.asm, $@) -o $@
$(x86_cpp_object): build/x86/%.o : arch/x86/%.cpp
	mkdir -p $(dir $@) && \
	x86_64-elf-g++ -march=i386 -m32 $(x86_include) $(shared_include) $(cppflags) $(patsubst build/x86/%.o, arch/x86/%.cpp, $@) -o $@

$(x86_32_asm_object): build/x86/x86_32/%.o : arch/x86/x86_32/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf32 $(patsubst build/x86/x86_32/%.o, arch/x86/x86_32/%.asm, $@) -o $@
$(x86_32_cpp_object): build/x86/x86_32/%.o : arch/x86/x86_32/%.cpp
	mkdir -p $(dir $@) && \
	x86_64-elf-g++ -march=i386 -m32 $(x86_32_include) $(shared_include) $(cppflags) $(patsubst build/x86/x86_32/%.o, arch/x86/x86_32/%.cpp, $@) -o $@

$(x86_64_asm_object): ./build/x86/x86_64/%.o : ./arch/x86/x86_64/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf64 $(patsubst ./build/x86/x86_64/%.o, ./arch/x86/x86_64/%.asm, $@) -o $@
$(x86_64_cpp_object): ./build/x86/x86_64/%.o : ./arch/x86/x86_64/%.cpp
	mkdir -p $(dir $@) && \
	x86_64-elf-g++ -march=i386 -m32 $(x86_64_include) $(cppflags) $(subst ./build/x86/x86_64/%.o, ./arch/x86/x86_64/%.cpp, $@) -o $@

$(shared_cpp_object): build/shared/%.o : src/%.cpp
	mkdir -p $(dir $@) && \
	x86_64-elf-g++ -march=i386 -m32 $(shared_include) $(cppflags) $(patsubst build/shared/%.o, src/%.cpp, $@) -o $@

.PHONY: build-x86
build-x86: $(x86_32_object) $(shared_object)
	mkdir -p ./dist/x86_32 && \
	mkdir -p ./build/x86_32 && \
	mkdir -p ./build/share && \
	x86_64-elf-gcc -march=i386 -m32 -n -o ./dist/x86_32/atrikrnl.elf $(ldflags) -O2 -T ./arch/x86/x86_32/LinkerScript-x86_32.ld $(x86_32_object) $(shared_object)
	cp ./dist/x86_32/atrikrnl.elf ./targets/x86_32/iso/boot/atrikrnl.elf
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_32/cubik.iso targets/x86_32/iso
	cp ./dist/x86_32/cubik.iso /mnt/f/CubikOSProject/dist/x86_32

.PHONY: build-x64-mb
build-x64-mb: $(x86_64_object) $(shared_object)
	mkdir -p ./dist/x86_64 && \
	mkdir -p ./build/x86_64 && \
	mkdir -p ./build/share && \
	$(linktool) -n -o ./dist/x86_32/atrikrnl.elf $(ldflags) -O2 -T ./arch/x86_32/LinkerScript-x86_32.ld $(x86_64_object) $(shared_object)
	cp ./dist/x86_32/atrikrnl.elf ./targets/x86_32/iso/boot/atrikrnl.elf
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_32/cubik.iso targets/x86_32/iso
	cp ./dist/x86_32/cubik.iso /mnt/f/CubikOSProject/dist/x86_32

.PHONY: clean
clean: 
	rm -rf ./build
	rm -rf ./dist

.PHONY: run-x86
run-x86:
	qemu-system-x86_64.exe -cdrom F:/CubikOSProject/dist/x86_32/cubik.iso