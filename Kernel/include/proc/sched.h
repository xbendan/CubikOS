#include <proc/proc.h>

proc_t *GetKernelProcess();
proc_t *GetCurrentProcess();


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
 * @return proc_t* 
 */
proc_t *CreateProcess();
proc_t *CreateELFProcess(file_t *file);
proc_t *CreateProcessEx(activity_t *activity, const char *name);
thread_t *CreateThread(proc_t *process);

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
void KillProcess(proc_t *process);

/**
 * @brief Send a message to terminate a process
 * 
 * This function will insert a message of terminating to the message 
 * queue of targeting process. It generally tells the process to stop
 * working and save all changes. Resources will be released manually
 * by the process, so it's safe to stop a processunless the process 
 * cannot handle this message. In that situation, sending a terminate 
 * message won't do anything, call @code {.c}
 * KillProcess(proc_t *process)
 * @endcode instead.
 * 
 * 
 * @param process 
 */
void TerminateProcess(proc_t *process);