kernel_source_files := $(shell find ./src -name *.c)
kernel_object_files := $(patsubst ./src/%.c, ./build/%.o, $(kernel_source_files))

x86_64_cpp_source := $(shell find ./src/Arch/x86_64 -name *.cpp)
x86_64_cpp_object := $(patsubst ./src/Arch/x86_64/%.cpp, build/x86_64/%.o, $(x86_64_cpp_source_files))

x86_64_asm_source := $(shell find ./src/Arch/x86_64 -name *.asm)
x86_64_asm_object := $(patsubst ./src/Arch/x86_64/%.asm, build/x86_64/%.o, $(x86_64_asm_source_files))

cpp_root_source := $(shell find ./src -maxdepth 1 -name *.cpp)
cpp_root_object := $(patsubst ./src/%.cpp, ./build/share/%.o, $(cpp_root_source))

cpp_graphic_source := $(shell find ./src/Graphic -maxdepth 1 -name *.cpp)
cpp_graphic_object := $(patsubst ./src/Graphic/%.cpp, ./build/share/Graphic/%.o, $(cpp_graphic_source))

cpp_memory_source := $(shell find ./src/Memory -maxdepth 1 -name *.cpp)
cpp_memory_object := $(patsubst ./src/Memory/%.cpp, ./build/share/Memory/%.o, $(cpp_memory_source))

cflags = -g \
	-fno-pic                       \
	-no-pie \
	-mavx \
	-msse \
	-Wall \
	-MD \
	-MMD \
	-O2 \
	-mno-80387                     \
	-mno-red-zone                  \
	-mcmodel=kernel \
	-fno-exceptions \
	-ffreestanding                 \
	-fno-stack-protector           \
	-fno-omit-frame-pointer        \
	-fno-isolate-erroneous-paths-attribute \
	-fno-delete-null-pointer-checks \

cppflags = -ffreestanding \
	-mno-red-zone \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-o2 \
	-c

ldflags = -nostdlib \
	-O2 \
	-z max-page-size=0x1000
	
includes = \
	-Iinclude \
	-Iinclude/Memory \
	-Iinclude/Arch/x86_64 
	
nasmtool = nasm -f elf64
cpptool = x86_64-elf-g++ -march=x86-64 -m64
linktool = x86_64-elf-ld -m elf_x86_64

$(x86_64_asm_object): ./build/x86_64/%.o : ./src/Arch/x86_64/%.asm
	mkdir -p $(dir $@) && \
	$(nasmtool) $(patsubst ./build/x86_64/%.o, ./src/Arch/x86_64/%.asm, $@) -o $@

$(x86_64_cpp_object): ./build/x86_64/%.o : ./src/%.cpp
	mkdir -p $(dir $@) && \
	$(cpptool) -c $(includes) $(cppflags) $(patsubst ./build/x86_64/%.o, ./src/Arch/x86_64/%.cpp, $@) -o $@

$(cpp_root_object): ./build/share/%.o : ./src/%.cpp
	mkdir -p $(dir $@) && \
	$(cpptool) -c $(includes) $(cppflags) $(patsubst build/share/%.o, src/%.cpp, $@) -o $@
	
$(cpp_graphic_object): ./build/share/%.o : ./src/%.cpp
	mkdir -p $(dir $@) && \
	$(cpptool) -c $(includes) $(cppflags) $(patsubst build/share/Graphic/%.o, src/%.cpp, $@) -o $@

$(cpp_memory_object): ./build/share/%.o : ./src/%.cpp
	mkdir -p $(dir $@) && \
	$(cpptool) -c $(includes) $(cppflags) $(patsubst build/share/Memory/%.o, src/Memory/%.cpp, $@) -o $@

x86_64_object_files := $(x86_64_cpp_object) $(x86_64_asm_object)

.PHONY: x86-64
x86-64: $(x86_64_asm_object) $(x86_64_cpp_object) $(cpp_root_object) $(cpp_graphic_object) $(cpp_memory_object)
	mkdir -p dist/x86_64 && \
	mkdir -p build/x86_64 && \
	mkdir -p build/share && \
	$(nasmtool) ./src/Arch/x86_64/stack.asm -o ./build/x86_64/stack.o
	$(nasmtool) ./src/Arch/x86_64/start.asm -o ./build/x86_64/start.o
	$(nasmtool) ./src/Arch/x86_64/header.asm -o ./build/x86_64/header.o
	$(nasmtool) ./src/Arch/x86_64/ASMFILE -o ./build/x86_64/ASMFILE.o
	$(cpptool) -c $(includes) -Iinclude/Arch/x86_64 $(cppflags) ./src/Arch/x86_64/Initialize.cpp -o ./build/x86_64/Initialize.o
	$(cpptool) -c $(includes) -Iinclude/Arch/x86_64 $(cppflags) ./src/Arch/x86_64/CPUID.cpp -o ./build/x86_64/CPUID.o
	$(cpptool) -c $(includes) -Iinclude/Arch/x86_64 $(cppflags) ./src/Arch/x86_64/COMPort.cpp -o ./build/x86_64/COMPort.o
	$(linktool) -n -o dist/x86_64/atrikrnl.elf $(ldflags) -O2 -T targets/x86_64/linker.ld $(shell find ./build/share -name *.o) $(shell find ./build/x86_64 -name *.o)
	cp dist/x86_64/atrikrnl.elf targets/x86_64/iso/boot/atrikrnl.elf
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/paper.iso targets/x86_64/iso
	
limine:
	cp ./dist/x86_64/atrikrnl.elf ./targets/x86_64/limine/atrikrnl.elf
	xorriso -as mkisofs -b limine-cd.bin \
	-no-emul-boot -boot-load-size 4 -boot-info-table \
	--efi-boot limine-eltorito-efi.bin \
	-efi-boot-part --efi-boot-image --protective-msdos-label \
	./targets/x86_64/limine -o image.iso
	./targets/limine/bin/limine-install ./targets/x86_64/image.iso
	cp ./dist/x86_64/image.iso ./targets/x86_64/image.iso

run:
	qemu-system-x86_64 -cdrom ./dist/x86_64/paper.iso
