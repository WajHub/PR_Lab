#include "list.h"
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


List *init_list(hash_t newKey){
    List *list =  (List *)malloc(sizeof(List));
    list ->head = NULL;
    list ->nextList = NULL;
    list ->prevList = NULL;
    list->key = newKey;
    return list;
}

int insert(List* list, void *node){
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode==NULL) return 1;
    newNode->value = node;
    Node *current = list->head;
    // Pusta Lista
    if(current == NULL){
        list->head = newNode;
        newNode->next = NULL;
        newNode->prev = NULL;
    }
    else{
        while(current->next!=NULL){
            current = current->next;
            }
            Node *nextNode = current->next;
            current->next = newNode;
            newNode->prev = current;
            newNode->next = nextNode;
            newNode->prev = newNode;
        }
    return 0;
}

void* get_from_list(List *list, hash_t key){
    Node *current = list->head;
    if(current!=NULL){
        return current->value;
    }
    return NULL;
}

void remove_from_list(List *list, hash_t key){
    Node *current = list->head;
    if(current!=NULL){
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
        current = current->next;
    }
}

void print_list(List *list){
    Node *current = list->head;
    while(current!=NULL){
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
