#include <proc/proc.h>
#include <proc/sched.h>
#include <mm/malloc.h>
#include <string.h>

struct Process *CreateProcess()
{

}

struct Process *CreateELFProcess(file_t *file)
{
    return CreateProcessEx(NULL, TaskTypeBackground, file, file->m_Name);
}

struct Process *CreateProcessEx(activity_t *activity, enum TaskType type, file_t *file, const char *name)
{
    struct Process *newProc = (struct Process *) KernelAllocateObject(sizeof(struct Process));

    *newProc = (struct Process)
    {
        .m_Name = (char *) KernelAllocateObject(strlen(name)),
        .m_Publisher = NULL,
        .m_Package = NULL,
        .m_ProcessId = GetNextPID(),
        .m_Type = type,
        .m_FilePtr = file,
        .m_Activity = activity,
        .m_IsFocus = false,
        .m_IsIdle = true,
        .m_Threads = 1,
        .m_Handles = 0,
        .m_MemoryAllocated = 0,
        .m_MemoryCached = 0,
        .m_Pages = 0,
        .m_PresentPages = 0,
        .m_SwappedPages = 0,
        .m_NextThreadId = 0,
        .m_MainThread = NULL,
        .m_ChildrenThreadList = NULL,
        .m_EntryPoint = 0x0, // Need a default entry point here
        .m_Heap = 0x0,

        #ifdef ARCH_X86_64
        .m_Pagemap = VM_CreatePagemap(),
        .m_VirtuaPagesBitmap = NULL
        #elif ARCH_AARCH64

        #elif ARCH_RISCV

        #endif
    };
    
    strcpy(newProc->m_Name, name);
    
    if(activity == NULL)
        newProc->m_Activity = NewActivity(newProc->m_Name);

    InitLock(&newProc->m_Lock);
    InitLock(&newProc->m_HandleLock);

    newProc->m_MainThread = newProc->m_ChildrenThreadList = CreateThread(newProc);
}

struct Thread *CreateThread(struct Process *proc)
{
    if (proc == NULL)
        return NULL;

    struct Thread *newThread = (struct Thread *) KernelAllocateStruct(KernelStructThread);

    *newThread = (struct Thread)
    {
        .m_ThreadId = proc->m_NextThreadId++,
        //.m_Parent = proc,
        .m_ThreadPriority = PriorityNormal,
        .m_ThreadState = TaskStateIdle
    };

    newThread->m_ThreadId = proc->m_NextThreadId++;
}