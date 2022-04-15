#include <stdint.h>

#define PROCESS_PRIORITY_AMOUNT 6

namespace Process
{
    typedef class Activity activity_t;

    typedef enum ProcessPriority
    {
        PriorityLowest = 0,
        PriorityLow = 1,
        PriorityNormal = 2,
        PriorityHigh = 3,
        PriorityHighest = 4,
        PriorityRealTime = 5
    } process_priority_t;

    typedef enum ProcessState
    {
        ProcessStateRunning,
        ProcessStateIdle,
    } process_state_t;

    /**
     * @brief process model
     * 
     * 522 bytes
     */
    typedef class Process
    {
        linked_list_node_t listNode;
        char* name;
        char* publisher; /* The name of publisher of this running */
        activity_t* owner; /* Pointer to the owner Activity */
        uint32_t pid; /* Random process id 0~65535 */
        process_priority_t priority; /* Indicates the priority of this process (Will influence all threads under this process) */
        process_state_t state; /* Indicates whether this process is running or not */
    } process_t;
}