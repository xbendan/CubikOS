#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define __VER__ "0.0.001"
 
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/**
 * Naming Rules
 * Namespace: Pascal, Start with parent namespace Kernel
 * Class Name: Pascal
 * Method Name: Pascal
 * Global Variable: Camel, no slash
 * Parameters: Camel, no slash in center
 */

/**
 * This is an experimental operating system kernel ATRI
 * It's going to support x86_64, arm(aarch64 ONLY) and risc-v (probably LoongArch?)
 * Boot from Limine and GRUB (x86, x86_64), OpenSBI (risc-v)
 * These codes follow the naming rule of C# (dotnet) rather than C/C++
 * If you are a C/C++ developer who dislike this naming rule, I suggest you find another project.
 * 
 * 这是一份实验性的操作系统内核 ATRI
 * 它最终将会支持 x86_64, ARM(仅64位)和RISC-V (或许还有龙芯？)
 * 在 x86 环境下，它使用 GRUB 或者 Limine 启动
 * 在 RISC-V 环境下，它使用 OpenSBI 启动
 * 这份内核源代码的命名规则遵循 C# (.NET) 而非传统 C/C++
 * 如果你是一名 C/C++ 开发者并不喜欢这种格式，请移步到其它项目
 * @author RichardC1ay
 */

void KernelEntry()
{
    for(;;)
    {
        asm("hlt");
    }
}