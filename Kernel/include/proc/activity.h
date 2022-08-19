#include <proc/proc.h>
#include <utils/list.h> 

typedef struct activity
{
    char *name;
    lklist_node_t listnode;
} activity_t;