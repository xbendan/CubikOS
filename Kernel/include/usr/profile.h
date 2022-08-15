#include <macros.h>

typedef uint16_t uid_t;

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
    uid_t uid;
    enum UserAccountState state;
    struct
    {
        uint32_t threads;
        uint16_t processes;
        uint32_t handles;
    };
} profile_t;