/**
 * @brief Linked List
 * contains the pointer to the previous and next
 * node address
 */
#pragma once

struct LinkedListNode
{
    struct LinkedListNode *prev, *next;
};

namespace Utils::LinkedList
{
    void Remove(struct LinkedListNode node);
    void InsertAtNext(struct LinkedListNode node0, struct LinkedListNode node1);
}