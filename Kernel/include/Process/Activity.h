namespace 
{
    enum ActivityType
    {
        Application,
        Background,
        Service,
        System
    }

    typedef struct Activity
    {
        const char* name;
        process_t pOwned[256];
    } activity_t;
}