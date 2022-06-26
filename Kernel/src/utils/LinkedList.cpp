#include <utils/LinkedList.h>

namespace Utils::LinkedList
{
    void llRemove(struct LinkedListNode* node)
    {
        struct LinkedListNode* pNode = node->prev;
        struct LinkedListNode* nNode = node->next;

        if(pNode != nullptr)
            pNode->next = nNode;
        if(nNode != nullptr)
            nNode->prev = pNode;
        node->prev = nullptr;
        node->next = nullptr;
    }

    void llInsertAtNext(struct LinkedListNode* node0, struct LinkedListNode* node1)
    {
        if(node0 == nullptr || node1 == nullptr)
            return;
        else
        {
            node1->prev = node0;
            if(node0->next != nullptr)
            {
                node1->next = node0->next;
                node1->next->prev = node1;
            }
            node0->next = node1;
        }
    }
}