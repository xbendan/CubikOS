#pragma once

typedef struct LinkedListNode
{
    struct LinkedListNode *prev;
    struct LinkedListNode *next;
} lklist_node_t;

typedef struct LinkedListItem
{
    lklist_node_t listnode;
    void *ptr;
} lklist_item_t;

void LinkedListRemove(lklist_node_t* node);
void LinkedListAppend(lklist_node_t* current, lklist_node_t* node);