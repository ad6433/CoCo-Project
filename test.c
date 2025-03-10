// FILE FOR TESTING LEXICAL ANALYZER. NOT TO BE SUBMITTED.

#include "lexer.h"

void twinBufferDebug(twinBuffer *B)
{
    printf("------------------BUFFER BEGINS------------------------\n");
    printf("buffer[0]:\t%s\n",B->buffer[0]);
    printf("buffer[1]:\t%s\n",B->buffer[1]);
    printf("currentBuffer:\t%d\n",B->currentBuffer);
    printf("currentPosition:\t%d\n",B->currentPosition);
    printf("lineNumber:\t%d\n",B->lineNumber);
    printf("------------------BUFFER ENDS------------------------\n");
}

void tokenInfoDebug(tokenInfo *token)
{
    printf("------------------TOKEN BEGINS------------------------\n");
    printf("lineNumber:\t%d\n",token->lineNumber);
    printf("type:\t%s\n",token->type);
    printf("lexeme:\t%s\n",token->lexeme);
    printf("------------------TOKEN ENDS------------------------\n");
}

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
    initializeTwinBuffer(&B);
    fp=getStream(fp, &B);
    while (1)
    {
        tokenInfo token=getNextToken(&B, fp);
        tokenInfoDebug(&token);
    }
    return 0;
}