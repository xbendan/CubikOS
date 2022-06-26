#include <stdint.h>

#include <utils/LinkedList.h>
#include <Types.h>

#define PROCESS_PRIORITY_AMOUNT 6

namespace Process
{
    typedef class Activity activity_t;

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

    /**
     * @brief process model
     * 
     * 44 bytes
     */
    typedef class Process
    {
        pid_t pid; /* Random process id 0~65535 */
        char* name;
        char* publisher; /* The name of publisher of this running */
        activity_t* owner; /* Pointer to the owner Activity */
        TaskPriority priority; /* Indicates the priority of this process (Will influence all threads under this process) */
        TaskState state; /* Indicates whether this process is running or not */
        bool focused;

        void exit(int exitValue);
    } proc_t;

    typedef class Thread
    {
        tid_t tid;
        proc_t* owner;
    } thread_t;

    typedef struct ProcessListHead
    {
        linked_list_node_t listnode;
        proc_t process;
    } proc_head_t;
}