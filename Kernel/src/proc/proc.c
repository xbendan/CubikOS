#include <proc/proc.h>
#include <mm/malloc.h>

struct Process *CreateProcess()
{

}

struct Process *CreateELFProcess(file_t *file)
{

}

struct Process *CreateProcessEx(activity_t *activity, file_t *file, const char *name)
{
    
}

struct Thread *CreateThread(struct Process *process)
{
    if (process == NULL)
        return NULL;

    struct Thread *newThread = (struct Thread *) KernelAllocateStruct(KernelStructThread);
}