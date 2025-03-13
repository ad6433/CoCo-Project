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
    // twinBufferDebug(&B);
    
    // FOR TESTING getNextChar() in lexer2
    // char currentChar;
    // while ((!feof(fp)) && ((currentChar=getNextChar(&B))!=EOF)) {
    //     printf("///////////////////////////////////////////////////\n");
    //     // printf("%c", currentChar);
    //     printf("currentChar: %c\n", currentChar);
    //     twinBufferDebug(&B);
    // }

    tokenInfo token;
    HashMap lookupTable;
    initializeLookupTable(&lookupTable);
    int tokenCount=0;
    while (1) {
        // printf("Here\n");
        token=getNextToken(&B, &lookupTable);
        // tokenInfoDebug(&token);
        printf("Line no. %d\t Lexeme %s\t \tToken %s\n", token.lineNumber, token.lexeme, token.type);
        ++tokenCount;
        // twinBufferDebug(&B);
        if (token.end)
            break;
    }
    printf("tokenCount = %d\n", tokenCount);

    //    FOR TESTING getNextChar()
    //    while (1)
    //    {
    //		char c=getNextChar(&B,fp);
    //		printf("%c",c);
    ////        twinBufferDebug(&B);
    //        if (feof(fp)) break;
    //    }

    // 	while (1) {
    //         tokenInfo token=getNextToken(&B, fp);
    //         if (token.type[0]=='\0' && token.lexeme[0]=='\0') {
    //             if (feof(fp)) {
    //             	printf("Reached EOF\n");
    // //            	break;
    //             }
    //             else continue;
    //         }
    //         tokenInfoDebug(&token);
    //         twinBufferDebug(&B);
    // 	}

    // Testing removeComments
    removeComments("test_ct1.txt", "clean_ct1.txt");

    return 0;
}