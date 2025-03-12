/*
 @Group No.:    27
 @Authors:
    Aditya Agrawal    2022A7PS0160P
    Samarth           2022A7PS0096P
    Shashank Jha      2022A7PS1186P
    Shikhar Singh     2022A7PS1170P
*/

#include "lexer.h"

void twinBufferDebug(twinBuffer *B)
{
    printf("------------------BUFFER STATUS BEGINS----------------------\n");
    printf("buffer[0]:\t%s\n",B->buffer[0]);
    printf("buffer[1]:\t%s\n",B->buffer[1]);
    printf("currentBuffer:\t%d\n",B->currentBuffer);
    printf("currentPosition:\t%d\n",B->currentPosition);
    printf("lineNumber:\t%d\n",B->lineNumber);
    printf("------------------BUFFER STATUS ENDS------------------------\n");
}

void tokenInfoDebug(tokenInfo *token)
{
    printf("------------------TOKEN STATUS BEGINS-----------------------------------------\n");
    printf("lineNumber:\t%d\n",token->lineNumber);
    printf("type:\t%s\n",token->type);
    printf("lexeme:\t%s\n",token->lexeme);
    printf("------------------TOKEN STATUS ENDS-------------------------------------------\n");
}

FILE *getStream(FILE *fp, twinBuffer *B) {
    if (fp==NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    char* buf=fgets(B->buffer[B->currentBuffer], MAX_BUFFER_SIZE, fp);
    if (buf==NULL) {
        if (feof(fp)) {
            printf("Reached EOF\n");
        }
        else {
            printf("Error reading file\n");
            exit(1);
        }
    }
    return fp;
}

void initializeTwinBuffer(twinBuffer *B) {
    B->currentBuffer = 0;
    memset(B->buffer[B->currentBuffer], '\0', MAX_BUFFER_SIZE);
    memset(B->buffer[B->currentBuffer^1], '\0', MAX_BUFFER_SIZE);
    B->currentPosition = 0;
    B->lineNumber = 1;
}

char getNextChar(twinBuffer *B, FILE* fp) {
    if (B->currentPosition >= MAX_BUFFER_SIZE) {
        B->currentBuffer^=1;
        fp=getStream(fp, B);
        B->currentPosition = 0;
    }
    char c=B->buffer[B->currentBuffer][B->currentPosition++];
    if (c == '\0') {
        B->currentBuffer^=1;
        fp=getStream(fp, B);
        B->currentPosition = 0;
    }
    if (c == '\n') {
        B->lineNumber++;
    }
    return c;
}

tokenInfo getNextToken(twinBuffer *B, FILE* fp) {
    tokenInfo token;
    token.lexeme[0] = '\0';
    token.type[0] = '\0';
    int state=0;
    char currentChar;
    while (1) {
        currentChar=getNextChar(B, fp);
        if (currentChar == '\0') {
            return token;
        }
        strcat(token.lexeme, &currentChar);
        if (currentChar == '\n') {
//            B->lineNumber++;
        }
        switch (state) {
            case 0:
                if (currentChar == '\n' || currentChar == ' ' || currentChar == '\t') {
                    token.lexeme[0] = '\0';
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
                while (currentChar != '\n') {
                    currentChar = getNextChar(B, fp);
//                    twinBufferDebug(B);
//                    if (currentChar == '\0') {
//                        if (feof(fp)) {
//                            token.lexeme[0] = '\0';
//                			strcat(token.type, "TK_COMMENT");
//                            token.lineNumber = B->lineNumber;
//                            return token;
//                        }
//                    }
                }
                token.lexeme[0] = '\0';
                strcat(token.type, "TK_COMMENT");
                token.lineNumber = B->lineNumber-1;
                return token;
                break;
            case 36:

        }
    }
    return token;
}