#include<stdio.h>
#include<stdlib.h>

typedef int hash_t;
hash_t hash(void *data);
int compareHash(hash_t hash1, hash_t hash2);

typedef struct Node;
typedef struct List;
struct List *initList();
void insertToList(struct List* list, void *node);
void getFromList(struct List *list, hash_t key);
void remove(struct List *list, hash_t key);
void printList(struct List *list);
void destructorList(struct List *list);

typedef struct HashTable;
struct HashTable *initHashTable();
void insertToHashTable(struct HashTable *hashTable, void *node);
void getFromHashTable(struct HashTable *hashTable, hash_t key);
void removeFromHashTable(struct HashTable *hashTable, hash_t key);
void printHashTable(struct HashTable *hashTable);
void destructorHashTable(struct HashTable *hashTable);

hash_t hash(void *data){
    return *((int*)data)%13*7;
}

int compareHash(hash_t hash1, hash_t hash2){
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
}List;

List *initList(){
    List *list =  (List *)malloc(sizeof(List));
    list ->head = NULL;
    return list;
}

void insert(List* list, void *node){
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->value = node;
    newNode->key = hash(node);
    Node *current = list->head;
    // Pusta Lista
    if(current == NULL){
        list->head = newNode;
        newNode->next = NULL;
        newNode->prev = NULL;
    }
    // Nowy element na poczatek
    else if(compareHash(current->key,newNode->key)<0){
        newNode->next = current;
        current->prev = newNode;
        newNode->prev = NULL;
        list->head = newNode;
    }else{
        while(current->next!=NULL && compareHash(current->next->key,newNode->key)>0){
            current = current->next;
        }
        Node *nextNode = current->next;
        current->next = newNode;
        newNode->prev = current;
        newNode->next = nextNode;
        newNode->prev = newNode;
    }
}

void printList(List *list){
    Node *temp = list->head;
    while(temp!=NULL){
        printf("Vale: %p(Key:%d)->",*((int*)temp->value), temp->key);
        temp = temp->next;
    }
}


void destructorList(List *list){
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
    List *list = initList();

    int a = 10;
    int b = 11;
    int c = 69;
    int d = 1;
    insert(list, &a);
    insert(list, &b);
    insert(list, &c);
    insert(list, &d);
    printList(list);

    destructorList(list);


    return 0;
}