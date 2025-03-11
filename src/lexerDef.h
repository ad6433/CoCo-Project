#ifndef LEXERDEF_H
#define LEXERDEF_H

typedef struct tokenInfo* TokenInfo;
struct tokenInfo
{
    char *token;
    char *lexeme;
};

TokenInfo makeToken (char *token, char *lexeme);

typedef struct twinBuffer* TwinBuffer;
struct twinBuffer
{
    int bufferSize;

    int lexemeBegin;
    int forward;
    
    char *buffer1;
    char *buffer2;
};

TwinBuffer makeBuffer (int bufferSize)
{
    TwinBuffer B = (TwinBuffer) malloc(sizeof(struct twinBuffer));
    B->bufferSize = bufferSize;
    B->lexemeBegin = 0;
    B->forward = 0;
    B->buffer1 = (char *) malloc(sizeof(char) * bufferSize);
    B->buffer2 = (char *) malloc(sizeof(char) * bufferSize);
    return B;
}

void freeBuffer (TwinBuffer B)
{
    free(B->buffer1);
    free(B->buffer2);
    free(B);
}

void reloadBuffer (TwinBuffer B, FILE *fp)
{
    
}

#endif