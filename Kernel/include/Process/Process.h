namespace Process
{
    enum ProcessPriority
    {
        Lowest = 0,
        Low = 1,
        Normal = 2,
        High = 3,
        Highest = 4,
        RealTime = 5
    };

    enum ProcessState
    {
        Running,
        Idle,
    };

    typedef struct Process
    {
        const char* name;
        uint16_t pid;
        ProcessPriority priority;
    } process_t;
}