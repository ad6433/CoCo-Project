#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "parserDef.h"

FirstAndFollow computeFirstAndFollowSets(Grammar G);

void createParseTable(FirstAndFollow F, Table T);

ParseTree parseInputSourceCode(char *testcaseFile, Table T);

void printParseTree(ParseTree PT, char *outfile);
// FORMAT: lexeme CurrentNode lineno tokenName valueIfNumber parentNodeSymbol isLeafNode(yes/no) NodeSymbol

#endif