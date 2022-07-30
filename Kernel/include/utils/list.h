typedef struct linked_list_node
{
    struct linked_list_node *prev, *next;
} lklist_head_t;

void lklist_remove(lklist_head_t* node);
void lklist_append(lklist_head_t* current, lklist_head_t* node);