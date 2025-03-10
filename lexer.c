/*
 @Group No.:    27
 @Authors:
    Aditya Agrawal    2022A7PS0160P
    Samarth           2022A7PS0096P
    Shashank Jha      2022A7PS1186P
    Shikhar Singh     2022A7PS1170P
*/

#include "lexer.h"

FILE *getStream(FILE *fp, twinBuffer *B) {
//    printf("\n------ getStream ------\n");
    if (fp==NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    if (fgets(B->buffer[B->currentBuffer], MAX_BUFFER_SIZE, fp) == NULL) {
        printf("Error reading file\n");
        exit(1);
    }
    return fp;
}

void initializeTwinBuffer(twinBuffer *B) {
//    printf("\n------ initializeTwinBuffer ------\n");
    B->currentBuffer = 0;
    memset(B->buffer[B->currentBuffer], '\0', MAX_BUFFER_SIZE);
    memset(B->buffer[B->currentBuffer^1], '\0', MAX_BUFFER_SIZE);
    B->currentPosition = 0;
    B->lineNumber = 1;
}

char getNextChar(twinBuffer *B, FILE* fp) {
//    printf("\n------ getNextChar ------\n");
    if (B->currentPosition >= MAX_BUFFER_SIZE || B->buffer[B->currentBuffer][B->currentPosition] == '\0') {
        B->currentBuffer^=1;
        fp=getStream(fp, B);
        B->currentPosition = 0;
    }
    if (B->buffer[B->currentBuffer][B->currentPosition] == '\n') {
        B->lineNumber++;
    }
    return B->buffer[B->currentBuffer][B->currentPosition++];
}

tokenInfo getNextToken(twinBuffer *B, FILE* fp) {
//    printf("\n------ getNextToken ------\n");
    tokenInfo token;
    token.lexeme[0] = '\0';
    token.type[0] = '\0';
    int state=0;
    char currentChar;
    while (1) {
        currentChar=getNextChar(B, fp);
        strcat(token.lexeme, &currentChar);
        switch (state) {
            case 0:
                if (currentChar == '\0') {
                    return token;
                }
                else if (currentChar == ']') {
                    strcat(token.type, "TK_SQR");
                    token.lineNumber = B->lineNumber;
                    return token;
                }
                else if (currentChar == '[') {
                    strcat(token.type, "TK_SQL");
                    token.lineNumber = B->lineNumber;
                    return token;
                }
                else if (currentChar == ',') {
                    strcat(token.type, "TK_COMMA");
                    token.lineNumber = B->lineNumber;
                    return token;
                }
                else if (currentChar == ';') {
                    strcat(token.type, "TK_SEM");
                    token.lineNumber = B->lineNumber;
                    return token;
                }
                else if (currentChar == ':') {
                    strcat(token.type, "TK_COLON");
                    token.lineNumber = B->lineNumber;
                    return token;
                }
                else if (currentChar == '.') {
                    strcat(token.type, "TK_DOT");
                    token.lineNumber = B->lineNumber;
                    return token;
                }
                else if (currentChar == ')') {
                    strcat(token.type, "TK_CL");
                    token.lineNumber = B->lineNumber;
                    return token;
                }
                else if (currentChar == '(') {
                    strcat(token.type, "TK_OP");
                    token.lineNumber = B->lineNumber;
                    return token;
                }
                else if (currentChar == '~') {
                    strcat(token.type, "TK_NOT");
                    token.lineNumber = B->lineNumber;
                    return token;
                }
                else if (currentChar == '+') {
                    strcat(token.type, "TK_PLUS");
                    token.lineNumber = B->lineNumber;
                    return token;
                }
                else if (currentChar == '-') {
                    strcat(token.type, "TK_MINUS");
                    token.lineNumber = B->lineNumber;
                    return token;
                }
                else if (currentChar == '*') {
                    strcat(token.type, "TK_MUL");
                    token.lineNumber = B->lineNumber;
                    return token;
                }
                else if (currentChar == '/') {
                    strcat(token.type, "TK_DIV");
                    token.lineNumber = B->lineNumber;
                    return token;
                }
//                COMMENTS NEED TO BE HANDLED IN A STATE OF THEIR OWN.
//                else if (currentChar == '%') {
//                    strcat(token.type, "TK_COMMENT");
//                    token.lineNumber = B->lineNumber;
//                    return token;
//                }
                else if (currentChar == '@') {
                    state = 36;
                }
                else if (currentChar == '&') {
                    state = 33;
                }
                else if (currentChar == '>') {
                    state = 30;
                }
                else if (currentChar == '_') {
                    state = 26;
                }
                else if (currentChar == '#') {
                    state = 23;
                }
                else if (currentChar == '<') {
                    state = 1;
                }
                else if (currentChar == 'a' || (currentChar >= 'e' && currentChar <= 'z')) {
                    state = 7;
                }
                else if (currentChar >= 'b' && currentChar <= 'd') {
                    state = 9;
                }
                else if (currentChar >= '0' && currentChar <= '9') {
                    state = 13;
                }
                else if (currentChar == '!') {
                    state = 39;
                }
                else if (currentChar == '=') {
                    state = 41;
                }
                else if (currentChar == '%') {
                    state = 101;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                }
                break;
            case 101:
                if (currentChar=='\n') {
                    strcat(token.type, "TK_COMMENT");
                    token.lineNumber = B->lineNumber;
                    return token;
                }
        }
    }
    return token;
}