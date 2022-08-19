#pragma once

typedef struct linked_list_node
{
    struct linked_list_node *prev;
    struct linked_list_node *next;
} lklist_node_t;

typedef struct linked_list_item
{
    lklist_node_t listnode;
    void *ptr;
} lklist_item_t;

void LinkedListRemove(lklist_node_t* node);
void LinkedListAppend(lklist_node_t* current, lklist_node_t* node);