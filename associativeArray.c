/*
 @Group No.:    27
 @Authors:
    Aditya Agrawal    2022A7PS0160P
    Samarth           2022A7PS0096P
    Shashank Jha      2022A7PS1186P
    Shikhar Singh     2022A7PS1170P
*/

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

void insertAA(AssociativeArray A, char *row, char *column, void *value)
{
    int index = A->hashFunction(strcat(strcat(row, " "), column)) % A->size;
    LinkedList L = A->values[index], prev = NULL;
    while (L != NULL)
    {
        if (strcmp(L->key, row) == 0)
        {
            L->data = value;
            return;
        }
        prev = L;
        L = L->next;
    }
    L = (LinkedList) malloc(sizeof(struct linkedList));
    L->data = value;
    L->key = strcpy((char *) malloc(50 * sizeof(char)), row);
    L->next = NULL;
    if (prev == NULL)
        A->values[index] = L;
    else
        prev->next = L;
}

void *get(AssociativeArray A, char *row, char *column)
{
    int index = A->hashFunction(strcat(strcat(row, " "), column)) % A->size;
    LinkedList L = A->values[index];
    while (L != NULL)
    {
        if (strcmp(L->data, column) == 0)
            return L->data;
        L = L->next;
    }
    return NULL;
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