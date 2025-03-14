/*
 @Group No.:    27
 @Authors:
    Aditya Agrawal    2022A7PS0160P
    Samarth           2022A7PS0096P
    Shashank Jha      2022A7PS1186P
    Shikhar Singh     2022A7PS1170P
*/

#ifndef LEXER_H
#define LEXER_H

#include "lexerDef.h"

FILE *getStream(twinBuffer *B);
void initializeTwinBuffer(twinBuffer *B, FILE* fp);
char getNextChar(twinBuffer *B);
tokenInfo getNextToken(twinBuffer *B, HashMap* lookupTable);
void removeComments(char* testcaseFileName, char* cleanFileName);
void twinBufferDebug(twinBuffer *B);
void tokenInfoDebug(tokenInfo *token);
void setNode(Node myNode, char* key, char* value);
int hash(HashMap mp, char* key);
void insert_HM(HashMap mp, char* key, char* value);
char* search(HashMap* mp, char* key);
void initializeLookupTable(HashMap* lookupTable);
void initializeHashMap(HashMap* mp);
void retract(twinBuffer *B, int n);
void printfile(FILE *fp);

#endif