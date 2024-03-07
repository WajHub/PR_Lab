typedef struct HashTable{
    List *headOfLists;
    void *comparator;
    void *hashFunction;
    void *printer;
}HashTable;


struct HashTable *init_hash_table(hash_t (*hash)(void *),int (*compare_hash)(hash_t, hash_t), void (*printer)(void*));
int insert_to_hashTable(struct HashTable* hashTable, void *node);
int insert_to_hashTable_new_hash(struct HashTable* hashTable,struct List *currentList, void *node);
void* get_from_hash_table(struct HashTable *hashTable, hash_t key);
void remove_from_hash_table(struct HashTable *hashTable, hash_t key);
void print_hashTable(struct HashTable *hashTable);
void destructor_hash_table(struct HashTable *hashTable);
