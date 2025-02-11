#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "lexerDef.h"

FILE *getStream(FILE *fp);

Tokeninfo getNextToken(TwinBuffer B);

void removeComments(char *testcaseFile, char *cleanFile);

#endif