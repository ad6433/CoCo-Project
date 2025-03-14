/*
 @Group No.:    27
 @Authors:
    Aditya Agrawal    2022A7PS0160P
    Samarth           2022A7PS0096P
    Shashank Jha      2022A7PS1186P
    Shikhar Singh     2022A7PS1170P
*/

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

void createParseTable(FirstAndFollow F, Table T, Grammar G);

ParseTree parseInputSourceCode(char *testcaseFile, Table T, Grammar G);

void printParseTree(ParseTree PT, char *outfile);
// FORMAT: lexeme CurrentNode lineno tokenName valueIfNumber parentNodeSymbol isLeafNode(yes/no) NodeSymbol

#endif