#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "parserDef.h"

Grammar readGrammar(FILE *fp);
/* FORMAT:
    numTerminals numNonTerminals numProductionRules startSymbol \n
    \n
    Terminals: (str \n)*
    \n
    Non-Terminals: (str \n)*
    \n
    Production Rules: (str (str )* \n)*
*/

FirstAndFollow computeFirstAndFollowSets(Grammar G);

void createParseTable(FirstAndFollow F, Table T);

ParseTree parseInputSourceCode(char *testcaseFile, Table T);

void printParseTree(ParseTree PT, char *outfile);
// FORMAT: lexeme CurrentNode lineno tokenName valueIfNumber parentNodeSymbol isLeafNode(yes/no) NodeSymbol

#endif