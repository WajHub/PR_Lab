#ifndef LIST
#define LIST
typedef int hash_t;
hash_t hash(void *data);
int compare_hash(hash_t hash1, hash_t hash2);
void print_element(void *element);
typedef struct Node{
    void *value;
    struct Node *next;
    struct Node *prev;
}Node;

typedef struct List{
    Node *head;
    hash_t key;
    struct List *nextList;
    struct List *prevList;
}List;

struct List *init_list(hash_t key);
int insert(struct List* list, void *node);
void* get_from_list(struct List *list, hash_t key);
void remove_from_list(struct List *list, hash_t key);
void print_list(struct List *list);
void destructor_list(struct List *list);



#endif /* LIST */