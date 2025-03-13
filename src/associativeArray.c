#include "associativeArray.h"
#include "stdlib.h"
#include "string.h"

AssociativeArray newAssociativeArray(int (*hashFunction)(char*), int initialSize)
{
    AssociativeArray A = (AssociativeArray) malloc(sizeof(struct associativeArray));
    A->keys = (char **) malloc(initialSize * sizeof(char *));
    A->hashFunction = hashFunction;
    A->values = (LinkedList *) malloc(initialSize * sizeof(LinkedList));
    A->size = initialSize;
    return A;
}

void insert(AssociativeArray A, char *row, char *column, void *value)
{
    int index = A->hashFunction(strcat(strcat(row, " "), column)) % A->size;
    LinkedList L = A->values[index], prev = NULL;
    while (L != NULL)
    {
        if (strcmp(L->data, column) == 0)
        {
            L->data = value;
            return;
        }
        prev = L;
        L = L->next;
    }
    L = (LinkedList) malloc(sizeof(struct linkedList));
    L->data = value;
    L->next = NULL;
    if (prev == NULL)
        A->values[index] = L;
    else
        prev->next = L;
}  

int poly_hash(char *s) {
    const int p = 31;
    const int m = 1e9 + 9;
    int hash_value = 0;
    int p_pow = 1;
    for (int i = 0; i < strlen(s); i++) {
        char c = s[i];
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}