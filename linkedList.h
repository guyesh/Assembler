

/* linked list - will be used to contain the data image on the first pass*/
typedef struct node {
    signed long int val; /* bit filed on long int isn't allowed on c90, thus I wont use a bit filed but i do make sure in the code the data doesn't pass 24 bits -(int might be sorter then 24, long is at least 32) */
    struct node * next;
} node_t;

void push(node_t * head, int val);
void freeList(node_t * head);
void print_list(node_t * head);

