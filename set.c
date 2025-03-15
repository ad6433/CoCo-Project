/*
 @Group No.:    27
 @Authors:
    Aditya Agrawal    2022A7PS0160P
    Samarth           2022A7PS0096P
    Shashank Jha      2022A7PS1186P
    Shikhar Singh     2022A7PS1170P
*/

#include "set.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

void addToSet(Set* set, char* symbol) {
    for (int i = 0; i < set->count; i++) {
        if (strcmp(set->symbols[i], symbol) == 0) return;
    }
    set->symbols[set->count++] = strdup(symbol);
}

bool isInSet(Set* set, char* symbol) {
    for (int i = 0; i < set->count; i++) {
        if (strcmp(set->symbols[i], symbol) == 0) return true;
    }
    return false;
}

void unionSets(Set* dest, Set* src) {
    for (int i = 0; i < src->count; i++) {
        addToSet(dest, src->symbols[i]);
    }
}

void printSet(Set* set) {
    for (int i = 0; i < set->count; i++) {
        printf("%s ", set->symbols[i]);
    }
    printf("\n");
}