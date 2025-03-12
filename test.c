// FILE FOR TESTING LEXICAL ANALYZER. NOT TO BE SUBMITTED.

#include "lexer.h"

int main()
{
    FILE *fp;
    fp=fopen("test_ct1.txt","r");
    if (fp==NULL)
    {
        printf("Error in opening file\n");
        exit(1);
    }
    twinBuffer B;
    initializeTwinBuffer(&B, fp);
    getStream(&B);
    twinBufferDebug(&B);
    
    // FOR TESTING getNextChar() in lexer2
    // char currentChar;
    // while ((!feof(fp)) && ((currentChar=getNextChar(&B))!=EOF)) {
    //     printf("///////////////////////////////////////////////////\n");
    //     // printf("%c", currentChar);
    //     printf("currentChar: %c\n", currentChar);
    //     twinBufferDebug(&B);
    // }

    tokenInfo token;
    while (1) {
        token=getNextToken(&B);
        if (token.end) break;
        tokenInfoDebug(&token);
        twinBufferDebug(&B);
    }

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


    return 0;
}