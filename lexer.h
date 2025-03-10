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

#endif //LEXER_H

#include "lexerDef.h"

FILE *getStream(FILE* fp, twinBuffer *B);
void initializeTwinBuffer(twinBuffer *B);
char getNextChar(twinBuffer *B, FILE* fp);
tokenInfo getNextToken(twinBuffer *B, FILE* fp);
void removeComments(char* testcaseFileName, char* cleanFileName);