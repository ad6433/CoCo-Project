/*
 @Group No.:    27
 @Authors:
    Aditya Agrawal    2022A7PS0160P
    Samarth           2022A7PS0096P
    Shashank Jha      2022A7PS1186P
    Shikhar Singh     2022A7PS1170P
*/

#include "lexer.h"

// ------------------------------------------------- Debugging functions ------------------------------------------------

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


// ------------------------------------------------- Lookup table implementation ----------------------------------------

void setNode(Node myNode, char* key, char* value) {
    myNode->key = malloc(sizeof(char*));
    *(myNode->key) = strdup(key);
    myNode->value = strdup(value);
    myNode->next = NULL;
}

void initializeHashMap(HashMap* mp) {
    *mp=(HashMap)malloc(sizeof(struct hashMap));
    (*mp)->capacity=250;
    (*mp)->numberOfElements=0;
    (*mp)->arr=(Node*)malloc(sizeof(Node)*(*mp)->capacity);
}

int hash(HashMap mp, char* key) {
    int sum=0, factor=31;
    for (int i=0; i<strlen(key); ++i) {
        // sum = key[0]*factor+key[1]*factor**2+key[2]*factor**3+...
        // with suitable modulus, which is mp->capacity in our case
        sum=(sum+(key[i]*factor)%(mp->capacity))%(mp->capacity);
        factor=(factor*31)%(mp->capacity);
    }
    return sum;
}

void insert(HashMap mp, char* key, char* value) {
    int index=hash(mp, key);
    Node myNode=(Node)malloc(sizeof(struct node));
    setNode(myNode, key, value);
    // no collision
    if (mp->arr[index]==NULL) {
        mp->arr[index]=myNode;
    }
    // collision
    else {
        myNode->next=mp->arr[index];
        mp->arr[index]=myNode;
    }
}

char* search(HashMap* mp, char* key) {
    int index=hash(*mp, key);
    Node head=(*mp)->arr[index];
    while (head!=NULL) {
        if (!strcmp(*(head->key), key)) {
            return head->value;
        }
        head=head->next;
    }
    return "Not found";
}

void initializeLookupTable(HashMap* lookupTable) {
    initializeHashMap(lookupTable);
    insert(*lookupTable, "with", "TK_WITH");
    insert(*lookupTable, "parameters", "TK_PARAMETERS");
    insert(*lookupTable, "end", "TK_END");
    insert(*lookupTable, "while", "TK_WHILE");
    insert(*lookupTable, "union", "TK_UNION");
    insert(*lookupTable, "endunion", "TK_ENDUNION");
    insert(*lookupTable, "definetype", "TK_DEFINETYPE");
    insert(*lookupTable, "as", "TK_AS");
    insert(*lookupTable, "type", "TK_TYPE");
    insert(*lookupTable, "_main", "TK_MAIN");
    insert(*lookupTable, "global", "TK_GLOBAL");
    insert(*lookupTable, "parameter", "TK_PARAMETER");
    insert(*lookupTable, "list", "TK_LIST");
    insert(*lookupTable, "input", "TK_INPUT");
    insert(*lookupTable, "output", "TK_OUTPUT");
    insert(*lookupTable, "int", "TK_INT");
    insert(*lookupTable, "real", "TK_REAL");
    insert(*lookupTable, "endwhile", "TK_ENDWHILE");
    insert(*lookupTable, "if", "TK_IF");
    insert(*lookupTable, "then", "TK_THEN");
    insert(*lookupTable, "endif", "TK_ENDIF");
    insert(*lookupTable, "read", "TK_READ");
    insert(*lookupTable, "write", "TK_WRITE");
    insert(*lookupTable, "return", "TK_RETURN");
    insert(*lookupTable, "call", "TK_CALL");
    insert(*lookupTable, "record", "TK_RECORD");
    insert(*lookupTable, "endrecord", "TK_ENDRECORD");
    insert(*lookupTable, "else", "TK_ELSE");
}


// ------------------------------------------------- Lexer functions ----------------------------------------------------

// Global line number tracker
int currentLineNumber = 1;

FILE *getStream(twinBuffer *B) {
    int targetBuffer = B->currentBuffer;
    // memset(B->buffer[targetBuffer], '\0', MAX_BUFFER_SIZE);
    size_t bytesRead = fread(B->buffer[targetBuffer], sizeof(char), MAX_BUFFER_SIZE-1, B->fp);
    // char* buf=fgets(B->buffer[targetBuffer], MAX_BUFFER_SIZE, B->fp);
    B->buffer[targetBuffer][bytesRead]='\0';
    return B->fp;
}

// void initializeTwinBuffer(twinBuffer *B, FILE *fp) {
//     B->fp=fp;
//     B->currentPosition=0;
//     B->currentBuffer=0;
//     B->lineNumber=1;
//     memset(B->buffer[0], '\0', MAX_BUFFER_SIZE);
//     memset(B->buffer[1], '\0', MAX_BUFFER_SIZE);
// }

void initializeTwinBuffer(twinBuffer *B, FILE *fp) {
    B->fp = fp;
    B->currentPosition = 0;
    B->currentBuffer = 0;
    B->lineNumber = 1;
    currentLineNumber = 1; // Initialize global line number
    memset(B->buffer[0], '\0', MAX_BUFFER_SIZE);
    memset(B->buffer[1], '\0', MAX_BUFFER_SIZE);
}

// char getNextChar(twinBuffer *B) {
//     // Check if buffer needs refill (end of current buffer reached)
//     if (B->currentPosition >= MAX_BUFFER_SIZE-1) {
//         B->currentBuffer = 1 - B->currentBuffer;
//         // Refill the INACTIVE buffer (twin buffer pattern)
//         getStream(B);
        
//         B->currentPosition = 0;
//     }

//     // Get current character from active buffer
//     char c = B->buffer[B->currentBuffer][B->currentPosition++];

//     // Handle newline tracking
//     if (c == '\n') {
//         B->lineNumber++;
//     }

//     return c;
// }

char getNextChar(twinBuffer *B) {
    // Check if buffer needs refill
    if (B->currentPosition >= MAX_BUFFER_SIZE-1) {
        B->currentBuffer = 1 - B->currentBuffer;
        getStream(B);
        B->currentPosition = 0;
    }
    
    // Get current character
    char c = B->buffer[B->currentBuffer][B->currentPosition++];
    
    // Handle newline tracking
    if (c == '\n') {
        B->lineNumber++;
        currentLineNumber++; // Update global line number
    }
    
    return c;
}

// void retract(twinBuffer *B, int n) {
//     // Handle invalid retraction amount
//     if (n <= 0) return;
    
//     int currentPos = B->currentPosition;
//     int currentBuf = B->currentBuffer;
    
//     // If we can retract within the current buffer
//     if (currentPos >= n) {
//         B->currentPosition -= n;
//         return;
//     }
    
//     // We need to switch buffers
//     // First retract what we can in the current buffer
//     n -= currentPos;
    
//     // Switch to the other buffer
//     B->currentBuffer = 1 - currentBuf;
    
//     // Position at the end of the other buffer
//     // MAX_BUFFER_SIZE-1 because the last position is for '\0'
//     B->currentPosition = MAX_BUFFER_SIZE - 1 - n;
    
//     // Ensure we don't go below 0
//     if (B->currentPosition < 0) {
//         B->currentPosition = 0;
//     }
// }

void retract(twinBuffer *B, int n) {
    // Handle invalid retraction amount
    if (n <= 0) return;
    
    int currentPos = B->currentPosition;
    int currentBuf = B->currentBuffer;
    int newlinesRetracted = 0;
    
    // If we can retract within the current buffer
    if (currentPos >= n) {
        // Count newlines in the retraction range
        for (int i = currentPos - n; i < currentPos; i++) {
            if (i >= 0 && B->buffer[currentBuf][i] == '\n') {
                newlinesRetracted++;
            }
        }
        B->currentPosition -= n;
    } else {
        // We need to switch buffers
        // First count newlines in current buffer's retraction range
        for (int i = 0; i < currentPos; i++) {
            if (B->buffer[currentBuf][i] == '\n') {
                newlinesRetracted++;
            }
        }
        
        // Retract what we can in current buffer
        n -= currentPos;
        
        // Switch to other buffer
        B->currentBuffer = 1 - currentBuf;
        
        // Position at end of other buffer minus remaining retraction
        B->currentPosition = MAX_BUFFER_SIZE - 1 - n;
        if (B->currentPosition < 0) {
            B->currentPosition = 0;
        }
        
        // Count newlines in other buffer's retraction range
        for (int i = B->currentPosition; i < MAX_BUFFER_SIZE - 1; i++) {
            if (B->buffer[B->currentBuffer][i] == '\n') {
                newlinesRetracted++;
            }
        }
    }
    
    // Adjust both line number trackers based on newlines retracted
    B->lineNumber -= newlinesRetracted;
    currentLineNumber -= newlinesRetracted;
}

tokenInfo getNextToken(twinBuffer *B, HashMap* lookupTable) {
    int state = 0;
    tokenInfo token;
    token.end = false;
    token.lexeme[0] = '\0';
    token.type[0] = '\0';
    
    // Record the starting line number for this token
    token.lineNumber = currentLineNumber;
    
    char currentChar;
    while (1) {
        // printf("state=%d\n", state);
        currentChar=getNextChar(B);
        if (currentChar=='\0') {
            token.end=true;
        }
        strncat(token.lexeme, &currentChar, 1);
        switch (state) {
            case 0:
                if (currentChar=='\0') {
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else if (currentChar=='\n' || currentChar=='\t' || currentChar==' ') {
                    token.lexeme[strlen(token.lexeme)-1]='\0';
                    if (strlen(token.lexeme)) {
                        token.lineNumber = currentLineNumber;
                        return token;
                    }
                    state = 0;
                }
                else if (currentChar==']') {
                    strcat(token.type, "TK_SQR");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else if (currentChar=='[') {
                    strcat(token.type, "TK_SQL");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else if (currentChar==',') {
                    strcat(token.type, "TK_COMMA");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else if (currentChar==';') {
                    strcat(token.type, "TK_SEM");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else if (currentChar==':') {
                    strcat(token.type, "TK_COLON");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else if (currentChar=='.') {
                    strcat(token.type, "TK_DOT");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else if (currentChar==')') {
                    strcat(token.type, "TK_CL");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else if (currentChar=='(') {
                    strcat(token.type, "TK_OP");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else if (currentChar=='~') {
                    strcat(token.type, "TK_NOT");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else if (currentChar=='+') {
                    strcat(token.type, "TK_PLUS");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else if (currentChar=='-') {
                    strcat(token.type, "TK_MINUS");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else if (currentChar=='*') {
                    strcat(token.type, "TK_MUL");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else if (currentChar=='/') {
                    strcat(token.type, "TK_DIV");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else if (currentChar=='%') {
                    state=101;
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
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 1:
                if (currentChar=='-') {
                    state=2;
                }
                else if (currentChar=='=') {
                    strcat(token.type, "TK_LE");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else {
                    strcat(token.type, "TK_LT");
                    token.lexeme[strlen(token.lexeme)-1]='\0';
                    retract(B, 1);
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 2:
                if (currentChar=='-') {
                    state=3;
                }
                else {
                    token.lexeme[strlen(token.lexeme)-2]='\0';
                    strcat(token.type, "TK_LT");
                    retract(B, 2);
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 3:
                if (currentChar=='-') {
                    strcat(token.type, "TK_ASSIGNOP");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else {
                    token.lexeme[strlen(token.lexeme)-2]='\0';
                    strcat(token.type, "TK_LT");
                    retract(B, 3);
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 7:
                if (currentChar>='a' && currentChar<='z') {
                    state=7;
                }
                else {
                    token.lexeme[strlen(token.lexeme)-1]='\0';
                    retract(B, 1);
                    char* value=search(lookupTable, token.lexeme);
                    if (strcmp(value, "Not found")) {
                        strcat(token.type, value);
                        token.lineNumber = currentLineNumber;
                        return token;
                    }
                    else {
                        strcat(token.type, "TK_FIELDID");
                        insert(*lookupTable, token.lexeme, "TK_FIELDID");
                        token.lineNumber = currentLineNumber;
                        return token;
                    }
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
                    token.lexeme[strlen(token.lexeme)-1]='\0';
                    retract(B, 1);
                    char* value=search(lookupTable, token.lexeme);
                    if (strcmp(value, "Not found")) {
                        strcat(token.type, value);
                        token.lineNumber = currentLineNumber;
                        return token;
                    }
                    else {
                        strcat(token.type, "TK_FIELDID");
                        insert(*lookupTable, token.lexeme, "TK_FIELDID");
                        token.lineNumber = currentLineNumber;
                        return token;
                    }
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
                    token.lexeme[strlen(token.lexeme)-1]='\0';
                    retract(B, 1);
                    if (strlen(token.lexeme)<=20) {
                        strcat(token.type, "TK_ID");
                        token.lineNumber = currentLineNumber;
                        return token;
                    }
                    else {
                        strcat(token.type, "TK_ERROR");
                        token.lineNumber = currentLineNumber;
                        return token;
                    }
                }
                break;
            case 12:
                if (currentChar>='2' && currentChar<='7') {
                    state=12;
                }
                else {
                    token.lexeme[strlen(token.lexeme)-1]='\0';
                    retract(B, 1);
                    if (strlen(token.lexeme)<=20) {
                        strcat(token.type, "TK_ID");
                        token.lineNumber = currentLineNumber;
                        return token;
                    }
                    else {
                        strcat(token.type, "TK_ERROR");
                        token.lineNumber = currentLineNumber;
                        return token;
                    }
                    token.lineNumber = currentLineNumber;
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
                    token.lexeme[strlen(token.lexeme)-1]='\0';
                    retract(B, 1);
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 15:
                if (currentChar>='0' && currentChar<='9') {
                    state=16;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 16:
                if (currentChar>='0' && currentChar<='9') {
                    state=17;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    token.lexeme[strlen(token.lexeme)-1]='\0';
                    retract(B, 1);
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 17:
                if (currentChar=='E') {
                    state=19;
                }
                else {
                    strcat(token.type, "TK_RNUM");
                    token.lexeme[strlen(token.lexeme)-1]='\0';
                    retract(B, 1);
                    token.lineNumber = currentLineNumber;
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
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 20:
                if (currentChar>='0' && currentChar<='9') {
                    state=21;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 21:
                if (currentChar>='0' && currentChar<='9') {
                    strcat(token.type, "TK_RNUM");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 23:
                if (currentChar>='a' && currentChar<='z') {
                    state=24;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 24:
                if (currentChar>='a' && currentChar<='z') {
                    state=24;
                }
                else {
                    strcat(token.type, "TK_RUID");
                    token.lexeme[strlen(token.lexeme)-1]='\0';
                    retract(B, 1);
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 26:
                if ((currentChar>='a' && currentChar<='z') || (currentChar>='A' && currentChar<='Z')) {
                    state=27;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    token.lineNumber = currentLineNumber;
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
                    token.lexeme[strlen(token.lexeme)-1]='\0';
                    retract(B, 1);
                    char* value=search(lookupTable, token.lexeme);
                    if (strcmp(value, "Not found")) {
                        strcat(token.type, value);
                        token.lineNumber = currentLineNumber;
                    return token;
                    }
                    else {
                        strcat(token.type, "TK_FUNID");
                        insert(*lookupTable, token.lexeme, "TK_FUNID");
                        token.lineNumber = currentLineNumber;
                    return token;
                    }
                }
                break;
            case 28:
                if (currentChar>='0' && currentChar<='9') {
                    state=28;
                }
                else {
                    token.lexeme[strlen(token.lexeme)-1]='\0';
                    retract(B, 1);
                    char* value=search(lookupTable, token.lexeme);
                    if (strcmp(value, "Not found")) {
                        strcat(token.type, value);
                        token.lineNumber = currentLineNumber;
                    return token;
                    }
                    else {
                        strcat(token.type, "TK_FUNID");
                        insert(*lookupTable, token.lexeme, "TK_FUNID");
                        token.lineNumber = currentLineNumber;
                    return token;
                    }
                }
                break;
            case 30:
                if (currentChar=='=') {
                    strcat(token.type, "TK_GE");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else {
                    strcat(token.type, "TK_GT");
                    token.lexeme[strlen(token.lexeme)-1]='\0';
                    retract(B, 1);
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 33:
                if (currentChar=='&') {
                    state=34;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    token.lexeme[strlen(token.lexeme)-1]='\0';
                    retract(B, 1);
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 34:
                if (currentChar=='&') {
                    strcat(token.type, "TK_AND");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    token.lexeme[strlen(token.lexeme)-1]='\0';
                    retract(B, 1);
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 36:
                if (currentChar=='@') {
                    state=37;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 37:
                if (currentChar=='@') {
                    strcat(token.type, "TK_OR");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 39:
                if (currentChar=='=') {
                    strcat(token.type, "TK_NE");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else {
                    strcat(token.type, "TK_ERROR");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 41:
                if (currentChar=='=') {
                    strcat(token.type, "TK_EQ");
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                else {
                    token.lexeme[strlen(token.lexeme)-1]='\0';
                    strcat(token.type, "TK_ERROR");
                    retract(B,1);
                    token.lineNumber = currentLineNumber;
                    return token;
                }
                break;
            case 101:
                if (currentChar=='\n') {
                    // token.lexeme[strlen(token.lexeme)-1]='\0';
                    token.lexeme[1]='\0';
                    strcat(token.type, "TK_COMMENT");
                    token.lineNumber = currentLineNumber-1;
                    return token;
                }
                else {
                    state=101;
                }
                break;

        }
    }
    return token;
}

void removeComments(char *testcaseFileName, char *cleanFileName)
{
    FILE *inputFile, *outputFile;
    char currentChar;

    // Open source code file for reading
    if ((inputFile = fopen(testcaseFileName, "r")) == NULL)
    {
        perror("Error opening input file");
        return;
    }

    // Open clean code file for writing
    if ((outputFile = fopen(cleanFileName, "w")) == NULL)
    {
        perror("Error opening output file");
        fclose(inputFile);
        return;
    }

    // int inComment = 0; // flag to indicate if we are inside a comment

    // while ((currentChar = fgetc(inputFile)) != EOF)
    // {
    //     if (inComment)
    //     {
    //         // Skip characters until end of line
    //         if (currentChar == '\n')
    //         {
    //             inComment = 0;                  // End of comment
    //             fputc(currentChar, outputFile); // Preserve newline for line numbering
    //         }
    //         continue;
    //     }

    //     if (currentChar == '%')
    //     {
    //         inComment = 1; // Start of comment detected
    //         continue;      // Skip the '%' character itself
    //     }

    //     // Write non-comment characters to output file
    //     fputc(currentChar, outputFile);
    // }
    char line[MAX_LINE_SIZE];
    while (fgets(line, MAX_LINE_SIZE, inputFile) != NULL)
    {
        char *commentStart = strchr(line, '%');

        // If comment exists, terminate the line at the comment start
        if (commentStart != NULL)
            *commentStart = '\0';

        // Remove trailing whitespace
        int len = strlen(line);
        while (len > 0 && isspace((unsigned char)line[len - 1]))
            line[--len] = '\0';

        // Check if line is empty after removing comments and whitespace
        int isEmpty = 1;
        for (int i = 0; line[i] != '\0'; i++)
        {
            if (!isspace((unsigned char)line[i]))
            {
                isEmpty = 0;
                break;
            }
        }

        // Write non-empty lines to output file
        if (!isEmpty)
            fprintf(outputFile, "%s\n", line);
    }
    fclose(inputFile);
    fclose(outputFile);
}

void printFile(FILE *fp)
{
    char buff[1000];
    while (fgets(buff, sizeof(buff), fp) != NULL)
        printf("%s", buff);
}