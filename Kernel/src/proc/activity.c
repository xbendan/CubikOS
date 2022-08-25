#include <mm/malloc.h>
#include <proc/proc.h>
#include <proc/activity.h>


struct Activity *NewActivity(const char* name)
{
    struct Activity *newActivity = (struct Activity *) KernelAllocateObject(sizeof(struct Activity));

    *newActivity = (struct Activity)
    {
        .m_Name = (char *) KernelAllocateObject(strlen(name)),
    };

    strcpy(newActivity->m_Name, name);

    return newActivity;
}