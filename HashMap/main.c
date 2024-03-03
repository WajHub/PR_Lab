#include<stdio.h>
#include<stdlib.h>
// TODO
// 1. Get i Remove dla hashmapy
// 2. Poprawienie kodu (szczegolnie przekazywanie funkcji) "typedef typ_zwracany (*nowa_nazwa_typu)(typy,argumentow,kolejnych);"

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
void insert_to_hashTable(struct HashTable* hashTable, void *node);
void insert_to_hashTable_new_hash(struct HashTable* hashTable,struct List *currentList, void *node);
void* get_from_hash_table(struct HashTable *hashTable, hash_t key);
void remove_from_hash_table(struct HashTable *hashTable, hash_t key);
void print_hashTable(struct HashTable *hashTable);
void destructor_hash_table(struct HashTable *hashTable);

hash_t hash(void *data){
    return *((int*)data)%10;
}

int compare_hash(hash_t hash1, hash_t hash2){
    if(hash1 == hash2)return 0;
    else if(hash1 > hash2)return 1;
    else return -1;
}

void print_element(void *element){
    printf("Value: %d",*((int*)element));    
}

typedef struct Node{
    void *value;
    hash_t key;
    struct Node *next;
    struct Node *prev;
}Node;

typedef struct List{
    Node *head;
    struct List *nextList;
    struct List *prevList;
    void *comparator;
    void *hashFunction;
    void *printer;
}List;

List *init_list(hash_t (*hash)(void *),int (*compare_hash)(hash_t, hash_t), void (*printer)(List*)){
    List *list =  (List *)malloc(sizeof(List));
    list ->head = NULL;
    list ->nextList = NULL;
    list ->prevList = NULL;
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
    Node *current = list->head;
    while(current!=NULL){
        printf("Key: (%d) ",current->key);
        ((void(*)(void*))print_element)(current->value);
        printf("->");
        current = current->next;
    }
    printf("\n");
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

typedef struct HashTable{
    List *headOfLists;
    void *comparator;
    void *hashFunction;
    void *printer;
}HashTable;

HashTable *init_hash_table(hash_t (*hash)(void *),int (*compare_hash)(hash_t, hash_t), void (*printer)(List*)){
    HashTable *hashTable =  (HashTable *)malloc(sizeof(HashTable));
    hashTable ->headOfLists = NULL;
    hashTable->comparator = compare_hash;
    hashTable->hashFunction = hash;
    hashTable->printer = printer;
    return hashTable;
}

void insert_to_hashTable(HashTable* hashTable, void *node){
    hash_t key = ((hash_t (*)(void *))hashTable->hashFunction)(node);
    List *list = hashTable->headOfLists;
    // Pusty HashTable
    if(list==NULL){
        list = init_list(hashTable->hashFunction, hashTable->comparator, hashTable->printer);
        insert(list, node);
        hashTable->headOfLists = list;
        return;
    }
    while(list->nextList!=NULL){
        // Dodanie elementu o tym samym hashu
        if(((int(*)(hash_t,hash_t))hashTable->comparator)(list->head->key,key)==0){
            insert(list, node);
            return;
        }
        // Dodanie elementu do nowej listy (nowy hash)
        if(((int(*)(hash_t,hash_t))hashTable->comparator)(list->head->key,key)<0){
            insert_to_hashTable_new_hash(hashTable, list, node);
            return;
        }
        list = list->nextList;
    }
    // Dodanie listy na przed ostatnie miejsce
    if(((int(*)(hash_t,hash_t))hashTable->comparator)(list->head->key,key)<0){
        insert_to_hashTable_new_hash(hashTable, list, node);
    }
    // Dodanie nowej listy na sam koniec
    else{
        List *newList = init_list(hashTable->hashFunction, hashTable->comparator, hashTable->printer);
        insert(newList, node);
        list->nextList = newList;
        newList->prevList = list;
    }
}

void insert_to_hashTable_new_hash(HashTable* hashTable, List *currentList, void *node){
    List *newList = init_list(hashTable->hashFunction, hashTable->comparator, hashTable->printer);
    insert(newList, node);
    // Dodanie na poczatek
    if(currentList->prevList==NULL){
        newList->nextList = currentList;
        currentList->prevList = newList;
        hashTable->headOfLists = newList; 
    }
    // Dodanie przed currentList
    else{
        List *temp = currentList;
        List *prev = currentList->prevList;
        newList->nextList = temp;
        newList->prevList = prev;
        prev->nextList = newList;
        temp->prevList = newList;
    }
}

void print_hashTable(HashTable *hashTable){
    List *list = hashTable->headOfLists;
    while(list!=NULL){
        printf("V: ");
        print_list(list);
        printf("\n");
        list = list->nextList;
    }
}

void destructor_hash_table(HashTable *hashTable){
    List *list = hashTable->headOfLists;
    while(list!=NULL){
        List *temp = list->nextList;
        destructor_list(list);
        list = temp;
    }
    free(hashTable);
}

int main()
{
    HashTable *hashTable = init_hash_table(hash, compare_hash, print_element);

    int a = 10;
    int b = 11;
    int c = 69;
    int d = 1;
    insert_to_hashTable(hashTable, &a);
    insert_to_hashTable(hashTable, &b);
    insert_to_hashTable(hashTable, &c);
    insert_to_hashTable(hashTable, &d);
    print_hashTable(hashTable);

    destructor_hash_table(hashTable);

    return 0;
}