#include <proc/proc.h>

struct Process *PR_GetKernelProcess();
struct Process *PR_GetCurrentProcess();


/**
 * @brief 
 * 
 * @param name 
 * @return activity_t* 
 */
activity_t *NewActivity(const char* name);

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
struct Process *CreateProcessEx(activity_t *activity, file_t* file, const char *name);

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
 * 
 * @param process 
 */
void TerminateProcess(struct Process *process);

void SchedulerInitialize();