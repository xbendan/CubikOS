#include <utils/list.h>
#include <macros.h>

void LinkedListRemove(lklist_node_t* node)
{
    lklist_node_t *prev_node = node->prev, *next_node = node->next;

    if(prev_node != NULL) prev_node->next = next_node;
    if(next_node != NULL) next_node->prev = prev_node;
}

void LinkedListAppend(lklist_node_t* current, lklist_node_t* node)
{
    if(current == NULL || node == NULL)
        return;
    else
    {
        node->prev = current;
        if(current->next != NULL)
        {
            node->next = current->next;
            current->next->prev = node;
        }
        current->next = node;
    }
}