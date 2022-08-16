#include <proc/proc.h>

typedef struct activity
{
    char *name;
    lklist_head_t process;
} activity_t;