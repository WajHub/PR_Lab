#include"hashTable.h"
#include<stdio.h>
#include<stdlib.h>

HashTable *init_hash_table(hash_t (*hash)(void *),int (*compare_hash)(hash_t, hash_t), void (*printer)(void*)){
    HashTable *hashTable =  (HashTable *)malloc(sizeof(HashTable));
    hashTable ->headOfLists = NULL;
    hashTable->comparator = compare_hash;
    hashTable->hashFunction = hash;
    hashTable->printer = printer;
    return hashTable;
}

int insert_to_hashTable(HashTable* hashTable, void *node){
    hash_t key = ((hash_t (*)(void *))hashTable->hashFunction)(node);
    List *list = hashTable->headOfLists;
    // Pusty HashTable
    if(list==NULL){
        list = init_list(key);
        if(insert(list, node)==1) return 1;
        hashTable->headOfLists = list;
        return 0;
    }
    while(list->nextList!=NULL){
        // Dodanie elementu o tym samym hashu
        if(((int(*)(hash_t,hash_t))hashTable->comparator)(list->key,key)==0){
            if(insert(list, node)==1) return 1;
            return 0;
        }
        // Dodanie elementu do nowej listy (nowy hash)
        if(((int(*)(hash_t,hash_t))hashTable->comparator)(list->key,key)<0){
            if(insert_to_hashTable_new_hash(hashTable, list, node)==1) return 1;
            return 0;
        }
        list = list->nextList;
    }
    // Dodanie listy na przed ostatnie miejsce
    if(((int(*)(hash_t,hash_t))hashTable->comparator)(list->key,key)<0){
        if(insert_to_hashTable_new_hash(hashTable, list, node)==1) return 1;
    }
    // Dodanie nowej listy na sam koniec
    else{
        List *newList = init_list( key);
        if(insert(newList, node)==1) return 1;
        list->nextList = newList;
        newList->prevList = list;
    }
    return 0;
}

int insert_to_hashTable_new_hash(HashTable* hashTable, List *currentList, void *node){
    List *newList = init_list(((hash_t (*)(void *))hashTable->hashFunction)(node));
    if(insert(newList, node)==1) return 1;
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
    return 0;
}

void* get_from_hash_table(struct HashTable *hashTable, hash_t key){
    List *list = hashTable->headOfLists;
    while(list!=NULL){
        if(((int(*)(hash_t,hash_t))hashTable->comparator)(list->key,key)==0){
            return get_from_list(list, key);
        }
        list = list->nextList;
    }
    return NULL;
}

void remove_from_hash_table(struct HashTable *hashTable, hash_t key){
    List *list = hashTable->headOfLists;
    while(list!=NULL){
        if(((int(*)(hash_t,hash_t))hashTable->comparator)(list->key,key)==0){
            remove_from_list(list, key);
            if(list->head==NULL){
                List *prev = list->prevList;
                List *next = list->nextList;
                if(prev!=NULL){
                    prev->nextList = next;
                }
                if(next!=NULL){
                    next->prevList = prev;
                }
                if(hashTable->headOfLists->head == NULL){
                    hashTable->headOfLists = next;
                }
                free(list);
            }
            return;
        }
        list = list->nextList;
    }
}

void print_hashTable(HashTable *hashTable){
    List *list = hashTable->headOfLists;
    while(list!=NULL){
        printf("V: ");
        printf("%d ", list->key);
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