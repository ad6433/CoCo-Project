#ifndef PARSERDEF_H
#define PARSERDEF_H

#include <stdbool.h>
#include "lexerDef.h"
#include "associativeArray.h"

typedef struct firstAndFollow* FirstAndFollow;
struct firstAndFollow
{

};

typedef struct productionRule* ProductionRule;
struct productionRule
{
    char *LHS; // One Non-terminal
    char **RHS; // Array of Terminals and Non-terminals
};

/*
    Grammar -> 4 tuple (T, N, P, S)
*/
typedef struct grammar* Grammar;
struct grammar
{
    int numTerminals, numNonTerminals, numProductionRules;
    char startSymbol[100];
    char **T; // Terminals
    char **N; // Non-Terminals
    ProductionRule *P; // Production Rules
};

typedef enum errorItem
{
    SYN,
    ERR,
}
ErrorItem;

typedef struct
{
    bool isError;

    union {
        ProductionRule *rule;
        ErrorItem error;
    } item;
}
TableEntry;

typedef struct table* Table;
struct table
{
    AssociativeArray associativeArray;
};

typedef struct parseTree* ParseTree;
struct parseTree
{
    ParseTreeNode root;
};

typedef struct parseTreeNode* ParseTreeNode;
struct parseTreeNode
{
    TokenInfo token;
    ParseTreeNode parent;
    ParseTreeNode *children;
};

#endif