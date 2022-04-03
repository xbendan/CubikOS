#include <LinkedList.h>

namespace Utils::LinkedList
{
    void Remove(struct LinkedListNode node)
    {
        struct LinkedListNode* pNode = node.prev;
        struct LinkedListNode* nNode = node.next;

        if(pNode != nullptr)
            pNode->next = nNode;
        if(nNode != nullptr)
            nNode->prev = pNode;
        node.prev = nullptr;
        node.next = nullptr;
    }

    void InsertAtNext(struct LinkedListNode node0, struct LinkedListNode node1)
    {
        node1.prev = &node0;
        node1.next = node0.next;
        node1.next->prev = &node1;
        node0.next = &node1;
    }
}