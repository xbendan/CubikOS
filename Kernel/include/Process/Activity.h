namespace Process
{
    typedef class Process process_t;

    enum ActivityType
    {
        Application,
        Background,
        Service,
        System
    };

    typedef class Activity
    {
        char* name;
        uint8_t pAmount; /* Indicates the amount of processes currently contained */
        process_t* pOwned; /*  */
    } activity_t;
}