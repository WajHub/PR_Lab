#include<stdio.h>
int main()
{
    HashTable *hashTable = init_hash_table(hash, compare_hash, print_element);

    int a = 10;
    int b = 11;
    int c = 69;
    int d = 1;
    int e = 17;
    int f = 9;
    int g = 18;
    int h = 247;
    if(insert_to_hashTable(hashTable, &a)==1) printf("Error");
    insert_to_hashTable(hashTable, &b);
    insert_to_hashTable(hashTable, &c);
    insert_to_hashTable(hashTable, &d);
    insert_to_hashTable(hashTable, &e);
    insert_to_hashTable(hashTable, &f);
    insert_to_hashTable(hashTable, &g);
    insert_to_hashTable(hashTable, &h);
    print_element(get_from_hash_table(hashTable, 1));
    printf("\nBefore remove\n");
    print_hashTable(hashTable);
    remove_from_hash_table(hashTable, 9);
    printf("\nAfter remove\n");
    print_hashTable(hashTable);

    remove_from_hash_table(hashTable, 1);
    printf("\nAfter remove2\n");
    print_hashTable(hashTable);


    remove_from_hash_table(hashTable, 9);
    printf("\nAfter remove3\n");
    print_hashTable(hashTable);

    destructor_hash_table(hashTable);

    return 0;
}