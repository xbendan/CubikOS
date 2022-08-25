#pragma once

#include <fs/vfs.h>
#include <proc/activity.h>
#include <utils/list.h>
#include <utils/spinlock.h>
#include <macros.h>

#ifdef ARCH_X86_64
    #include <x86_64/cpu.h>
#elif ARCH_AARCH64

#elif ARCH_RISCV

#endif

typedef uint32_t tid_t;
typedef uint16_t pid_t;

enum TaskPriority
{
    PriorityIdle = -1,
    PriorityLowest = 0,
    PriorityLow = 1,
    PriorityNormal = 2,
    PriorityHigh = 3,
    PriorityHighest = 4,
    PriorityRealTime = 5
};

enum TaskState
{
    TaskStateRunning = 0,
    TaskStateIdle = 1,
};

enum TaskType
{
    TaskTypeSystemProcess = 0,
    TaskTypeSystemDriver = 1,
    TaskTypeApplication = 2,
    TaskTypeService = 3,
    TaskTypeBackground = 4
};

enum ThreadState
{
    ThreadStateAbolished = 0,
    ThreadStateRunning = 1,
    ThreadStateBlocked = 2
};

struct Process
{
    char *m_Name;         /* Name of the process */
    char *m_Publisher;    /* Name of the publisher */
    //char *m_FileName;     /* Executable file name of this process */
    char *m_Package;      /* Package Name */
    pid_t m_ProcessId;    /* Process Id, 0~255 are reserved for kernel process */
    uint8_t m_Type;       /* Current process type */
    struct File *m_FilePtr;         /* Pointer to the source file, can be NULL */
    struct Activity *m_Activity;    /* Pointer to the Activity */
    union
    { // Flags
        uint32_t __flag_size__;
        struct
        {
            bool m_IsFocus: 1;
            bool m_IsIdle: 1;
            uint32_t __reserved__: 30;
        } __attribute__((packed));
    };
    uint16_t m_Threads;   /* Amount of threads kept by this process*/
    uint16_t m_Handles;   /* Register handles amount */
    //uint16_t cores;     /* CPU Cores currently running on */
    struct
    { // Memory
        uint64_t m_MemoryAllocated;     /* Memory allocated for this process */
        uint32_t m_MemoryCached;        /* Memory allocated and cached */
        uint32_t m_Pages;               /* Amount of 4K pages */
        uint32_t m_PresentPages;        /* Pages that is present now */
        uint32_t m_SwappedPages;        /* Pages that has been swapped into disks */
    };

    struct
    {
        spinlock_t m_Lock;
        spinlock_t m_HandleLock;
    };

    struct
    {
        uint32_t m_NextThreadId;
        struct Thread *m_MainThread;
        struct Thread *m_ChildrenThreadList;
    };

    uintptr_t m_EntryPoint;
    uintptr_t m_Heap;

    /* Architecture Fields */
    #ifdef ARCH_X86_64
    void *m_Pagemap;
    uint64_t** m_VirtuaPagesBitmap;
    #elif ARCH_AARCH64

    #elif ARCH_RISCV

    #endif
};

struct Thread
{
    lklist_node_t listnode;

    tid_t m_ThreadId;                  /* Thread ID, not duplicated in same progress */
    //struct Process *m_Parent;    /* Parent process, indicates the owner of this thread */
    spinlock_t m_Lock;            /* Thread lock */
    spinlock_t m_StateLock;       /* Thread state lock */

    struct
    {
        uint32_t esp0;
        uint32_t ss0;
    };

    void *m_UserStack;
    void *m_UserStackLimit;
    void *m_KernelStack;
    void *m_KernelStackLimit;

    struct RegisterContext m_Registers;  
    struct RegisterContext m_LastSyscall;

    uint8_t m_ThreadPriority;       /* The priority when scheduling */
    uint8_t m_ThreadState;          /* Thread state */
};

#ifdef ARCH_X86_64
    #include <x86_64/paging.h>
#elif ARCH_AARCH64

#elif ARCH_RISCV

#endif

/**
 * @brief Create a Process object
 * 
 * @return struct Process* Pointer to new process object
 */
struct Process *CreateProcess();

/**
 * @brief Create a process from existing file
 * 
 * @param file Source file, must be executable
 * @return struct Process* Pointer to new process object
 */
struct Process *CreateELFProcess(file_t *file);

/**
 * @brief Create a Process object with full arguments
 * 
 * A process will not be scheduled once it is created because the default
 * state of a process is TaskStateIdle.
 * Set it to TaskStateRunning to active it.
 * 
 * @param activity Activity the process belongs to
 * @param name Process name, use filename instead if its NULL, or <activity_name>-<pid>
 * @param file The file that the process created from
 * @return struct Process* Pointer to new process object
 */
struct Process *CreateProcessEx(activity_t *activity, enum TaskType type, file_t* file, const char *name);

/**
 * @brief Create a new thread object of specific process
 * 
 * @param process Parent process
 * @return struct Thread* Pointer to new thread
 */
struct Thread *CreateThread(struct Process *process);

/**
 * @brief Forcely kill a process and clean all resources
 * 
 * Killing process is an irreversible operation that it stops the
 * process immediately and release all the resources, including allocated
 * memory, opened files, and other resources locked by this process.
 * All of these operation will be done by the kernel, it would not ask
 * process to do anything. On the other words, process will be unable to
 * save any changes.
 * 
 * @param process Process that will be killed
 */
void KillProcess(struct Process *process);

/**
 * @brief Send a message to terminate a process
 * 
 * This function will insert a message of terminating to the message 
 * queue of targeting process. It generally tells the process to stop
 * working and save all changes. Resources will be released manually
 * by the process, so it's safe to stop a processunless the process 
 * cannot handle this message. In that situation, sending a terminate 
 * message won't do anything, call @code {.c}
 * KillProcess(struct Process *process)
 * @endcode instead.
 * 
 * @param process 
 */
void TerminateProcess(struct Process *process);