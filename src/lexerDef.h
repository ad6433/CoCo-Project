#ifndef LEXERDEF_H
#define LEXERDEF_H

typedef struct tokenInfo* Tokeninfo;
struct tokenInfo
{
    char *token;
    char *lexeme;
};

Tokeninfo makeToken (char *token, char *lexeme);

typedef struct twinBuffer* TwinBuffer;
struct twinBuffer
{
    int bufferSize;

    int lexemeBegin;
    int forward;
    
    char *buffer1;
    char *buffer2;
};

TwinBuffer makeBuffer (int bufferSize);
void freeBuffer (TwinBuffer B);
void reloadBuffer (TwinBuffer B, FILE *fp);

#endif