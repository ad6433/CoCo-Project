// FILE FOR TESTING LEXICAL ANALYZER. NOT TO BE SUBMITTED.

#include "lexer.h"

int main()
{
    FILE *fp;
    fp = fopen("test_ct1.txt", "r");
    if (fp == NULL)
    {
        printf("Error in opening file\n");
        exit(1);
    }

    freopen("output.txt", "w", stdout);

    twinBuffer B;
    initializeTwinBuffer(&B, fp);
    getStream(&B);

    tokenInfo token;
    HashMap lookupTable;
    initializeLookupTable(&lookupTable);
    int tokenCount=0;
    while (1) {
        // printf("Here\n");
        token=getNextToken(&B, &lookupTable);
        // tokenInfoDebug(&token);
        if (strcmp(token.type, "TK_ERROR"))
            printf("Line no. %d\t Lexeme %s\t \tToken %s\n", token.lineNumber, token.lexeme, token.type);
        ++tokenCount;
        // twinBufferDebug(&B);
        if (token.end)
            break;
    }
    printf("tokenCount = %d\n", tokenCount);

    // Testing removeComments
    removeComments("test_t1.txt", "clean_t1.txt");

    return 0;
}