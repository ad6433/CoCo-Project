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
    printf("lineNumber:\t%d\n", token->lineNumber);
    printf("type:\t%s\n", token->type);
    printf("lexeme:\t%s\n", token->lexeme);
    printf("end:\t%d\n", token->end);
    printf("------------------TOKEN STATUS ENDS-------------------------------------------\n");
}

FILE *getStream(twinBuffer *B) {
    int targetBuffer = B->currentBuffer;
    // memset(B->buffer[targetBuffer], '\0', MAX_BUFFER_SIZE);
    size_t bytesRead = fread(B->buffer[targetBuffer], sizeof(char), MAX_BUFFER_SIZE-1, B->fp);
    // char* buf=fgets(B->buffer[targetBuffer], MAX_BUFFER_SIZE, B->fp);
    B->buffer[targetBuffer][bytesRead]='\0';
    return B->fp;
}

void initializeTwinBuffer(twinBuffer *B, FILE *fp) {
    B->fp=fp;
    B->currentPosition=0;
    B->currentBuffer=0;
    B->lineNumber=1;
    memset(B->buffer[0], '\0', MAX_BUFFER_SIZE);
    memset(B->buffer[1], '\0', MAX_BUFFER_SIZE);
}

char getNextChar(twinBuffer *B) {
    // Check if buffer needs refill (end of current buffer reached)
    if (B->currentPosition >= MAX_BUFFER_SIZE-1) {
        B->currentBuffer = 1 - B->currentBuffer;
        // Refill the INACTIVE buffer (twin buffer pattern)
        getStream(B);
        
        B->currentPosition = 0;
    }

    // Get current character from active buffer
    char c = B->buffer[B->currentBuffer][B->currentPosition++];

    // Handle newline tracking
    if (c == '\n') {
        B->lineNumber++;
    }

    return c;
}

tokenInfo getNextToken(twinBuffer *B) {
    int state=0;
    tokenInfo token;
    token.end=false;
    token.lexeme[0]='\0';
    token.type[0]='\0';
    token.lineNumber=B->lineNumber;
    char currentChar;

    while (1) {
        currentChar=getNextChar(B);
        if (currentChar=='\0') {
            token.end=true;
        }
        strncat(token.lexeme, &currentChar, 1);
        switch (state) {
            case 0:
                if (currentChar=='\0') {
                    return token;
                }
                else if (currentChar=='\n' || currentChar=='\t' || currentChar==' ') {
                    return token;
                }
                else if (currentChar==']') {
                    strcat(token.type, "TK_SQR");
                    return token;
                }
                else if (currentChar=='[') {
                    strcat(token.type, "TK_SQL");
                    return token;
                }
                else if (currentChar==',') {
                    strcat(token.type, "TK_COMMA");
                    return token;
                }
                else if (currentChar==';') {
                    strcat(token.type, "TK_SEM");
                    return token;
                }
                else if (currentChar==':') {
                    strcat(token.type, "TK_COLON");
                    return token;
                }
                else if (currentChar=='.') {
                    strcat(token.type, "TK_DOT");
                    return token;
                }
                else if (currentChar==')') {
                    strcat(token.type, "TK_CL");
                    return token;
                }
                else if (currentChar=='(') {
                    strcat(token.type, "TK_OP");
                    return token;
                }
                else if (currentChar=='~') {
                    strcat(token.type, "TK_NOT");
                    return token;
                }
                else if (currentChar=='+') {
                    strcat(token.type, "TK_PLUS");
                    return token;
                }
                else if (currentChar=='-') {
                    strcat(token.type, "TK_MINUS");
                    return token;
                }
                else if (currentChar=='*') {
                    strcat(token.type, "TK_MUL");
                    return token;
                }
                else if (currentChar=='/') {
                    strcat(token.type, "TK_DIV");
                    return token;
                }
                else if (currentChar=='%') {
                    strcat(token.type, "TK_COMMENT");
                    return token;
                }
                else if (currentChar=='@') {
                    state=36;
                }
                else if (currentChar=='&') {
                    state=33;
                }
                else if (currentChar=='>') {
                    state=30;
                }
                else if (currentChar=='_') {
                    state=26;
                }
                else if (currentChar=='#') {
                    state=23;
                }
                else if (currentChar=='<') {
                    state=1;
                }
                else if (currentChar=='a' || (currentChar>='e' && currentChar<='z')) {
                    state=7;
                }
                else if (currentChar>='b' && currentChar<='d') {
                    state=9;
                }
                else if (currentChar>='0' && currentChar<='9') {
                    state=13;
                }
                else if (currentChar=='!') {
                    state=39;
                }
                else if (currentChar=='=') {
                    state=41;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                }
                break;
            case 1:
                if (currentChar=='-') {
                    state=2;
                }
                else if (currentChar=='=') {
                    strcat(token.type, "TK_LE");
                    return token;
                }
                else {
                    strcat(token.type, "TK_LT");
                    (B->currentPosition)--;
                    return token;
                }
                break;
            case 7:
                if (currentChar>='a' && currentChar<='z') {
                    state=7;
                }
                else {
                    // REQUIRES IMPLEMENTING SYMBOL TABLE
                    // ACTION 8
                }
                break;
            case 9:
                if (currentChar>='a' && currentChar<='z') {
                    state=7;
                }
                else if (currentChar>='2' && currentChar<='7') {
                    state=10;
                }
                else {
                    // REQUIRES IMPLEMENTING SYMBOL TABLE
                    // ACTION 8
                }
                break;
            case 10:
                if (currentChar>='b' && currentChar<='d') {
                    state=10;
                }
                else if (currentChar>='2' && currentChar<='7') {
                    state=12;
                }
                else {
                    strcat(token.type, "TK_ID");
                    (B->currentPosition)--;
                    return token;
                }
                break;
            case 12:
                if (currentChar>='2' && currentChar<='7') {
                    state=12;
                }
                else {
                    strcat(token.type, "TK_ID");
                    (B->currentPosition)--;
                    return token;
                }
                break;
            case 13:
                if (currentChar>='0' && currentChar<='9') {
                    state=13;
                }
                else if (currentChar=='.') {
                    state=15;
                }
                else {
                    strcat(token.type, "TK_NUM");
                    (B->currentPosition)--;
                    return token;
                }
                break;
            case 15:
                if (currentChar>='0' && currentChar<='9') {
                    state=16;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    return token;
                }
                break;
            case 16:
                if (currentChar>='0' && currentChar<='9') {
                    state=17;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    return token;
                }
                break;
            case 17:
                if (currentChar=='E') {
                    state=19;
                }
                else {
                    strcat(token.type, "TK_RNUM");
                    (B->currentPosition)--;
                    return token;
                }
                break;
            case 19:
                if (currentChar=='+' || currentChar=='-') {
                    state=20;
                }
                else if (currentChar>='0' && currentChar<='9') {
                    state=21;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    return token;
                }
                break;
            case 20:
                if (currentChar>='0' && currentChar<='9') {
                    state=21;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    return token;
                }
                break;
            case 21:
                if (currentChar>='0' && currentChar<='9') {
                    strcat(token.type, "TK_RNUM");
                    return token;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    return token;
                }
                break;
            case 23:
                if (currentChar>='a' && currentChar<='z') {
                    state=24;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    return token;
                }
                break;
            case 24:
                if (currentChar>='a' && currentChar<='z') {
                    state=24;
                }
                else {
                    strcat(token.type, "TK_RUID");
                    (B->currentPosition)--;
                    return token;
                }
                break;
            case 26:
                if ((currentChar>='a' && currentChar<='z') || (currentChar>='A' && currentChar<='Z')) {
                    state=27;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    return token;
                }
                break;
            case 27:
                if ((currentChar>='a' && currentChar<='z') || (currentChar>='A' && currentChar<='Z')) {
                    state=27;
                }
                else if (currentChar>='0' && currentChar<='9') {
                    state=28;
                }
                else {
                    // REQUIRES IMPLEMENTING SYMBOL TABLE
                    // ACTION 29
                }
                break;
            case 28:
                if (currentChar>='0' && currentChar<='9') {
                    state=28;
                }
                else {
                    // REQUIRES IMPLEMENTING SYMBOL TABLE
                    // ACTION 29
                }
                break;
            case 30:
                if (currentChar=='=') {
                    strcat(token.type, "TK_GE");
                    return token;
                }
                else {
                    strcat(token.type, "TK_GT");
                    (B->currentPosition)--;
                    return token;
                }
                break;
            case 33:
                if (currentChar=='&') {
                    state=34;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    return token;
                }
                break;
            case 34:
                if (currentChar=='&') {
                    strcat(token.type, "TK_AND");
                    return token;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    return token;
                }
                break;
            case 36:
                if (currentChar=='@') {
                    state=37;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    return token;
                }
                break;
            case 37:
                if (currentChar=='@') {
                    strcat(token.type, "TK_OR");
                    return token;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    return token;
                }
                break;
            case 39:
                if (currentChar=='=') {
                    strcat(token.type, "TK_NE");
                    return token;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    return token;
                }
                break;
            case 41:
                if (currentChar=='=') {
                    strcat(token.type, "TK_EQ");
                    return token;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    return token;
                }
                break;
            
        }
    }
    return token;
}