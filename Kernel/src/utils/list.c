#include <utils/list.h>

void lklist_remove(lklist_head_t* node)
{
    lklist_head_t *prev_node = node->prev, *next_node = node->next;

    if(prev_node != nullptr) prev_node->next = next_node;
    if(next_node != nullptr) next_node->prev = prev_node;
}

void lklist_append(lklist_head_t* current, lklist_head_t* node)
{
    if(current == nullptr || node == nullptr)
        return;
    else
    {
        node->prev = current;
        if(current->next != nullptr)
        {
            node->next = current->next;
            current->next->prev = node;
        }
        current->next = node;
    }
}