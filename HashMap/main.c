#include<stdio.h>
#include<stdlib.h>
// TODO
// 2. Zaimplementowanie HashMapy
// 3. Poprawienie kodu (szczegolnie przekazywanie funkcji) "typedef typ_zwracany (*nowa_nazwa_typu)(typy,argumentow,kolejnych);"
// 4. Poprawienie printowania (zrobienie funkcji do printowania elementu)

typedef int hash_t;
hash_t hash(void *data);
int compare_hash(hash_t hash1, hash_t hash2);

typedef struct Node;
typedef struct List;
struct List *init_list();
void insert_to_list(struct List* list, void *node);
void* get_from_list(struct List *list, hash_t key);
void remove_from_list(struct List *list, hash_t key);
void print_list(struct List *list);
void destructor_list(struct List *list);

typedef struct HashTable;
struct HashTable *init_hash_table();
void insert_to_hash_table(struct HashTable *hashTable, void *node);
void* get_from_hash_table(struct HashTable *hashTable, hash_t key);
void remove_from_hash_table(struct HashTable *hashTable, hash_t key);
void print_hash_table(struct HashTable *hashTable);
void destructor_hash_table(struct HashTable *hashTable);

hash_t hash(void *data){
    return *((int*)data)%13*7;
}

int compare_hash(hash_t hash1, hash_t hash2){
    if(hash1 == hash2)return 0;
    else if(hash1 > hash2)return 1;
    else return -1;
}

typedef struct Node{
    void *value;
    hash_t key;
    struct Node *next;
    struct Node *prev;
}Node;

typedef struct List{
    Node *head;
    void *comparator;
    void *hashFunction;
    void *printer;
}List;

List *init_list(hash_t (*hash)(void *),int (*compare_hash)(hash_t, hash_t), void (*printer)(List*)){
    List *list =  (List *)malloc(sizeof(List));
    list ->head = NULL;
    list->comparator = compare_hash;
    list->hashFunction = hash;
    list->printer = printer;
    return list;
}

void insert(List* list, void *node){
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->value = node;
    newNode->key = ((hash_t (*)(void *))list->hashFunction)(node);
    Node *current = list->head;
    // Pusta Lista
    if(current == NULL){
        list->head = newNode;
        newNode->next = NULL;
        newNode->prev = NULL;
    }
    // Nowy element na poczatek
    else if((((int(*)(hash_t,hash_t))list->comparator))(current->key,newNode->key)<0){
        newNode->next = current;
        current->prev = newNode;
        newNode->prev = NULL;
        list->head = newNode;
    }else{
        while(current->next!=NULL && (((int(*)(hash_t,hash_t))list->comparator))(current->key,newNode->key)>0){
            current = current->next;
        }
        Node *nextNode = current->next;
        current->next = newNode;
        newNode->prev = current;
        newNode->next = nextNode;
        newNode->prev = newNode;
    }
}

void* get_from_list(List *list, hash_t key){
    Node *current = list->head;
    while(current!=NULL){
        if(current->key == key){
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

void remove_from_list(List *list, hash_t key){
    Node *current = list->head;
    while(current!=NULL){
        if(current->key == key){
            Node *prev = current->prev;
            Node *next = current->next;
            if(prev!=NULL){
                prev->next = next;
            }else{
                list->head = next;
            }
            if(next!=NULL){
                next->prev = prev;
            }
            free(current);
            return;
        }
        current = current->next;
    }
}

void print_list(List *list){
    Node *temp = list->head;
    while(temp!=NULL){
        printf("Vale: %p(Key:%d)->",*((int*)temp->value), temp->key);
        temp = temp->next;
    }
}

void destructor_list(List *list){
    Node *temp = list->head;
    while(temp!=NULL){
        Node *temp2 = temp->next;
        free(temp);
        temp = temp2;
    }
    free(list);
}

int main()
{
    List *list = init_list(hash, compare_hash, print_list);

    

    int a = 10;
    int b = 11;
    int c = 69;
    int d = 1;
    insert(list, &a);
    insert(list, &b);
    insert(list, &c);
    insert(list, &d);
    ((void(*)(List*))list->printer)(list);
    printf("\n");
    remove_from_list(list, 70);
   ((void(*)(List*))list->printer)(list);

    destructor_list(list);

    return 0;
}