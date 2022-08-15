enum UserAccountState
{
    UserAccountActive = 0;
    UserAccountLocked = 1;
    UserAccountInactive = 2;
    UserAccountSleep = 3;
}

typedef struct UserProfile
{
    char* name;
    enum UserAccountState state;
    struct
    {
        uint32_t threads;
        uint16_t processes;
        uint32_t handles;
    };
} profile_t;