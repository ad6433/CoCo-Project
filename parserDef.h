/*
 @Group No.:    27
 @Authors:
    Aditya Agrawal    2022A7PS0160P
    Samarth           2022A7PS0096P
    Shashank Jha      2022A7PS1186P
    Shikhar Singh     2022A7PS1170P
*/

#ifndef PARSERDEF_H
#define PARSERDEF_H

#include <stdbool.h>
#include "lexerDef.h"
#include "associativeArray.h"
#include "set.h"

typedef struct productionRule* ProductionRule;
struct productionRule
{
    char *LHS;
    char **RHS;
    int RHSLength;
};

typedef struct grammar* Grammar;
struct grammar
{
    int numNonTerminals, numTerminals, numProductionRules;
    char startSymbol[100];
    char **T;
    char **N;
    ProductionRule *P;
};

typedef struct firstAndFollow* FirstAndFollow;
struct firstAndFollow
{
    Set **first;
    Set **follow;
};

typedef struct table* Table;
struct table
{
    AssociativeArray associativeArray;
};

typedef struct parseTreeNode* ParseTreeNode;
struct parseTreeNode
{
    char *symbol;
    bool isLeafNode;
    tokenInfo *token;
    ParseTreeNode parent;
    ParseTreeNode *children;
    int numChildren;
};

typedef struct parseTree* ParseTree;
struct parseTree
{
    ParseTreeNode root;
};

#endif
