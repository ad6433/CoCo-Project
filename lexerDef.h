/*
 @Group No.:    27
 @Authors:
    Aditya Agrawal    2022A7PS0160P
    Samarth           2022A7PS0096P
    Shashank Jha      2022A7PS1186P
    Shikhar Singh     2022A7PS1170P
*/

#ifndef LEXERDEF_H
#define LEXERDEF_H

#endif // LEXERDEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h> // for isspace in removeComments

#define MAX_LEXEME_SIZE 1000
#define MAX_TOKEN_SIZE 15
#define MAX_BUFFER_SIZE 10
#define MAX_LINE_SIZE 1024 // for removeComments

typedef struct
{
    int lineNumber;
    char type[MAX_TOKEN_SIZE];
    char lexeme[MAX_LEXEME_SIZE];
    bool end;
} tokenInfo;

typedef struct
{
    char buffer[2][MAX_BUFFER_SIZE];
    int currentBuffer;
    int currentPosition;
    int lineNumber;
    FILE *fp;
} twinBuffer;

typedef struct node *Node;
struct node
{
    char **key;
    char *value;
    Node next;
};

typedef struct hashMap *HashMap;
struct hashMap
{
    int numberOfElements;
    int capacity;
    Node *arr;
};
