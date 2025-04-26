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
#include "stdio.h"

typedef struct KeyValue {
    char key[256];   // e.g., "S a"
    void *value;     // production rule pointer (ProductionRule)
} KeyValue;


AssociativeArray newAssociativeArray(int (*hashFunction)(char*), int initialSize) {
    AssociativeArray A = (AssociativeArray) malloc(sizeof(struct associativeArray));
    A->keys = (char **) malloc(initialSize * sizeof(char *));
    A->values = (LinkedList *) malloc(initialSize * sizeof(LinkedList));
    A->size = initialSize;
    A->hashFunction = hashFunction;
    // Initialize each bucket to NULL.
    for (int i = 0; i < initialSize; i++) {
        A->values[i] = NULL;
    }
    return A;
}
void insertAA(AssociativeArray A, char *row, char *column, void *value) {
    char buffer[256];  // temporary buffer for key construction
    // Build the key as "row column" (nonterminal first, terminal second)
    sprintf(buffer, "%s %s", row, column);
    
    int rawHash = A->hashFunction(buffer);
    int index = ((rawHash % A->size) + A->size) % A->size;
    
    LinkedList L = A->values[index], prev = NULL;
    while (L != NULL) {
        KeyValue *kv = (KeyValue *) L->data;
        if (strcmp(kv->key, buffer) == 0) {
            // Key exists; update the value.
            kv->value = value;
            return;
        }
        prev = L;
        L = L->next;
    }
    // Key not found: create a new KeyValue.
    KeyValue *newKV = (KeyValue *) malloc(sizeof(KeyValue));
    strcpy(newKV->key, buffer);
    newKV->value = value;
    
    // Create a new linked list node and insert it.
    LinkedList newNode = (LinkedList) malloc(sizeof(struct linkedList));
    newNode->data = newKV;
    newNode->next = NULL;
    
    if (prev == NULL)
        A->values[index] = newNode;
    else
        prev->next = newNode;
}

// Revised get with debugging prints.
void *get(AssociativeArray A, char *row, char *column) {
    char buffer[256];
    // Construct the key exactly as in insertAA.
    sprintf(buffer, "%s %s", row, column);
    
    int rawHash = A->hashFunction(buffer);
    int index = ((rawHash % A->size) + A->size) % A->size;
    
    LinkedList L = A->values[index];
    while (L != NULL) {
        KeyValue *kv = (KeyValue *) L->data;
        if (strcmp(kv->key, buffer) == 0)
            return kv->value;
        L = L->next;
    }
    return NULL;
}


int poly_hash(char *s) {
    const int p = 31;
    const int m = 1000000009; // 1e9 + 9
    int hash_value = 0;
    int p_pow = 1;
    for (int i = 0; i < strlen(s); i++) {
        char c = s[i];
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}
