/*
 @Group No.:    27
 @Authors:
    Aditya Agrawal    2022A7PS0160P
    Samarth           2022A7PS0096P
    Shashank Jha      2022A7PS1186P
    Shikhar Singh     2022A7PS1170P
*/

#ifndef SET_H
#define SET_H

#define MAX_SYMBOLS 100

typedef struct set Set;
struct set {
    char* symbols[MAX_SYMBOLS];
    int count;
};

void addToSet(Set* set, char* symbol);
bool isInSet(Set* set, char* symbol);
void unionSets(Set* dest, Set* src);

#endif
