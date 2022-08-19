# **Naming Rules**  

|Type|Style|Abbreviation|Notes|Example|  
|----|----|----|----|----|
|Header File Name|Snake Case|Yes||proc.h|
|Source File Name|Snake Case|Yes||page.c|
|Directory/Path Name|Lower Case|Yes||mem|
|----||||
|Assembly Function|Snake Case|Yes|Start with "asmw(assembly wrapped)" if refered in C|asmw_get_page|  
|Linker Variable|Upper Case with Underline|Yes||KERNEL_ADDR_START = .;|
|Keyword *#define*|Upper Case with Underline|Better Not|Use brackets to wrap words if there are any operators|#define ARCH_PAGE_SIZE 4096|  
|Keyword *#include*|||Use <> rather than ""|  
|*typedef* Item|Snake Case|Yes||pageframe_list_t|  
|Function|\<type>_\<purpose>|Long word only|"*type*" defines which part of this function in. For instance, function of virtual memory management (paging) should start with [**VM**]. the "*purpose*" defines the work the function do, it should use Upper Camel Case. A full function name would seems like [**VM_MapVirtualAddress**]. This function is used for virtual memory management, and map addresses from phyical to virtual when it was called.|VM_MapVirtualAddress|  
|Function Arguments|Lower Camel Case|Yes||uint32_t index|
|Global Variable|Lower Camel Case|No|Start with **g_** (letter 'g' and an underline)|g_bootInfo|
|Local Variable|Lower Camel Case|No||localVariable|
|Struct Name|Upper Camel Case without Underline|No||struct ExampleStruct|
|Field in Struct|Snake Case|Better Not|Do NOT follow this if a field is already defined, such as register contexts. Prefer 1 or 2 word(s) only. If a bunch of field needs to be put together, add a prefix, such as "**slab**_inuse"|uint32_t example;|
|Enumeration Name|Upper Camel Case without Underline|Better Not||enum VariableType|
|Enumeration Item|Upper Camel Case without Underline|No||MemoryMapEntryTypeAvailable = 0|
