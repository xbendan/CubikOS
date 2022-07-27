enum user_state
{
    UserAccountActive = 0;
    UserAccountLocked = 1;
    UserAccountInactive = 2;
    UserAccountSleep = 3;
}

typedef struct user
{
    char* name;
    uint8_t state;
    struct 
    {
        uint16_t runnings;
        uint32_t threads;
        uint32_t handles;
    } proc;
} user_t;