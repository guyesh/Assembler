#include "linkedList.h"
#include "allfiles.h"

/*pushes a new value to a list - assumes there is at least one node*/
void push(node_t * head, int val) {
    node_t * current = head;
    while (current->next != NULL) {
        current = current->next;
    }

        /* now we can add a new variable */
    current->next = (node_t *) mallocAndCheck(sizeof(node_t));
    current->next->val = val;
    current->next->next = NULL;

    
   
}
/*freeing all the allocated memory in the list*/
void freeList(node_t * head)
{
   node_t * current = head;
    node_t* next=NULL;
     if(head!=NULL)
    {
        while (current->next != NULL) {
            next = current->next;
            free(current);
            current=next;
        }
        free(current);
    }
}
void print_list(node_t * head) {
    node_t * current = head;
    while (current != NULL) {
        printf("%ld\n", current->val);
        current = current->next;
    }
}