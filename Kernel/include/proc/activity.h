#include <utils/list.h> 

typedef struct activity
{
    char *name;
    lklist_node_t listnode;
} activity_t;

/**
 * @brief 
 * 
 * @param name 
 * @return activity_t* 
 */
activity_t *NewActivity(const char* name);