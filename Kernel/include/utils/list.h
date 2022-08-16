#pragma once

typedef struct linked_list_node
{
    struct linked_list_node *prev;
    struct linked_list_node *next;
} lklist_head_t;

typedef struct linked_list_item
{
    lklist_head_t listnode;
    void *ptr;
} lklist_item_t;

void lklist_remove(lklist_head_t* node);
void lklist_append(lklist_head_t* current, lklist_head_t* node);