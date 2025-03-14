/*
 @Group No.:    27
 @Authors:
    Aditya Agrawal    2022A7PS0160P
    Samarth           2022A7PS0096P
    Shashank Jha      2022A7PS1186P
    Shikhar Singh     2022A7PS1170P
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parser.h"
#include "lexer.h"
#include "set.h"

#define MAX_SYMBOLS 100
#define MAX_PRODUCTIONS 100

bool isTerminal(Grammar G, char* symbol)
{
    for (int i = 0; i < G->numTerminals; i++)
        if (strcmp(G->T[i], symbol) == 0) return true;
    return false;
}

bool isNonTerminal(Grammar G, char* symbol)
{
    for (int i = 0; i < G->numNonTerminals; i++)
        if (strcmp(G->N[i], symbol) == 0) return true;
    return false;
}

Grammar readGrammar(FILE *fp)
{
    Grammar G = (Grammar) malloc(sizeof(struct grammar));
    fscanf(fp, "%d %d %d %s\n", &G->numTerminals, &G->numNonTerminals, &G->numProductionRules, G->startSymbol);
    
    G->T = (char **) malloc(G->numTerminals * sizeof(char *));
    G->N = (char **) malloc(G->numNonTerminals * sizeof(char *));
    G->P = (ProductionRule *) malloc(G->numProductionRules * sizeof(ProductionRule));
    
    for (int i = 0; i < G->numTerminals; i++)
    {
        G->T[i] = (char *) malloc(100 * sizeof(char));
        fscanf(fp, "%s\n", G->T[i]);
    }
    
    fscanf(fp, "\n");
    for (int i = 0; i < G->numNonTerminals; i++)
    {
        G->N[i] = (char *) malloc(100 * sizeof(char));
        fscanf(fp, "%s\n", G->N[i]);
    }
    
    fscanf(fp, "\n");
    for (int i = 0; i < G->numProductionRules; i++)
    {
        G->P[i] = (ProductionRule) malloc(sizeof(struct productionRule));
        G->P[i]->RHS = (char **) malloc(100 * sizeof(char *));
        G->P[i]->LHS = (char *) malloc(100 * sizeof(char));
        fscanf(fp, "%s", G->P[i]->LHS);
        int j = 0;
        while (fscanf(fp, "%s", G->P[i]->RHS[j]) != EOF && strcmp(G->P[i]->RHS[j], "\n") != 0) j++;
        G->P[i]->RHSLength = j;
    }
    
    return G;
}

Set* computeFirst(Grammar G, char* symbol, Set** firstSets)
{
    if (isTerminal(G, symbol))
    {
        Set* first = malloc(sizeof(Set));
        first->count = 0;
        addToSet(first, symbol);
        return first;
    }

    if (firstSets[atoi(symbol)] != NULL)
    {
        return firstSets[atoi(symbol)];
    }

    Set* first = malloc(sizeof(Set));
    first->count = 0;
    firstSets[atoi(symbol)] = first;

    for (int i = 0; i < G->numProductionRules; i++)
    {
        if (strcmp(G->P[i]->LHS, symbol) == 0)
        {
            for (int j = 0; j < G->P[i]->RHSLength; j++)
            {
                Set* symbolFirst = computeFirst(G, G->P[i]->RHS[j], firstSets);
                unionSets(first, symbolFirst);
                if (!isInSet(symbolFirst, "EPS")) break;
                if (j == G->P[i]->RHSLength - 1) {
                    addToSet(first, "EPS");
                }
            }
        }
    }

    return first;
}

FirstAndFollow computeFirstAndFollowSets(Grammar G)
{
    FirstAndFollow F = (FirstAndFollow) malloc(sizeof(struct firstAndFollow));
    F->first = (Set**) malloc(G->numNonTerminals * sizeof(Set*));
    F->follow = (Set**) malloc(G->numNonTerminals * sizeof(Set*));

    // Initialize FIRST sets
    for (int i = 0; i < G->numNonTerminals; i++)
        F->first[i] = computeFirst(G, G->N[i], F->first);

    // Initialize FOLLOW sets
    for (int i = 0; i < G->numNonTerminals; i++)
    {
        F->follow[i] = malloc(sizeof(Set));
        F->follow[i]->count = 0;
    }

    // Add $ to FOLLOW(S)
    addToSet(F->follow[0], "$");

    bool changed;
    do {
        changed = false;
        for (int i = 0; i < G->numProductionRules; i++)
        {
            char* LHS = G->P[i]->LHS;
            for (int j = 0; j < G->P[i]->RHSLength; j++)
            {
                char* current = G->P[i]->RHS[j];
                if (isNonTerminal(G, current))
                {
                    int currentIndex = atoi(current);
                    if (j == G->P[i]->RHSLength - 1)
                    {
                        int LHSIndex = atoi(LHS);
                        int oldCount = F->follow[currentIndex]->count;
                        unionSets(F->follow[currentIndex], F->follow[LHSIndex]);
                        if (oldCount != F->follow[currentIndex]->count) changed = true;
                    }
                    else
                    {
                        char* next = G->P[i]->RHS[j+1];
                        Set* nextFirst = computeFirst(G, next, F->first);
                        int oldCount = F->follow[currentIndex]->count;
                        for (int k = 0; k < nextFirst->count; k++)
                        {
                            if (strcmp(nextFirst->symbols[k], "EPS") != 0)
                                addToSet(F->follow[currentIndex], nextFirst->symbols[k]);
                        }
                        if (oldCount != F->follow[currentIndex]->count)
                            changed = true;
                        if (isInSet(nextFirst, "EPS"))
                        {
                            int LHSIndex = atoi(LHS);
                            oldCount = F->follow[currentIndex]->count;
                            unionSets(F->follow[currentIndex], F->follow[LHSIndex]);
                            if (oldCount != F->follow[currentIndex]->count) changed = true;
                        }
                    }
                }
            }
        }
    }
    while (changed); // Terminate when an iteration goes without any changes

    return F;
}

void createParseTable(FirstAndFollow F, Table T, Grammar G)
{
    T->associativeArray = newAssociativeArray(poly_hash, G->numNonTerminals * G->numTerminals);
    
    for (int i = 0; i < G->numProductionRules; i++)
    {
        char *nonTerminal = G->P[i]->LHS;
        for (int j = 0; j < G->numTerminals; j++)
        {
            char *terminal = G->T[j];
            Set* first = F->first[atoi(nonTerminal)];
            Set* follow = F->follow[atoi(nonTerminal)];
            
            if (isInSet(first, terminal) || (isInSet(first, "ε") && isInSet(follow, terminal)))
            {
                insertAA(T->associativeArray, nonTerminal, terminal, (void*)&(G->P[i]));
            }
        }
    }
}

ParseTreeNode createParseTreeNode(char *symbol, bool isLeafNode, tokenInfo *token, ParseTreeNode parent) {
    ParseTreeNode node = (ParseTreeNode) malloc(sizeof(struct parseTreeNode));
    node->symbol = strdup(symbol);
    node->isLeafNode = isLeafNode;
    node->token = token;
    node->parent = parent;
    node->children = NULL;
    node->numChildren = 0;
    return node;
}

ParseTree parseInputSourceCode(char *testcaseFile, Table T, Grammar G) {
    FILE *fp = fopen(testcaseFile, "r");
    if (fp == NULL) {
        printf("Error: Cannot open file %s\n", testcaseFile);
        return NULL;
    }
    
    twinBuffer B;
    initializeTwinBuffer(&B, fp);
    tokenInfo *currentToken = (tokenInfo*) malloc(sizeof(tokenInfo));
    *currentToken = getNextToken(&B, NULL);
    
    ParseTreeNode root = createParseTreeNode(G->startSymbol, false, NULL, NULL);
    ParseTree PT = (ParseTree) malloc(sizeof(struct parseTree));
    PT->root = root;
    
    ParseTreeNode *stack = (ParseTreeNode*) malloc(1000 * sizeof(ParseTreeNode));
    int top = 0;
    stack[top] = root;
    
    while (top >= 0) {
        ParseTreeNode current = stack[top--];
        
        if (current->isLeafNode) {
            if (strcmp(current->symbol, currentToken->type) == 0) {
                current->token = currentToken;
                currentToken = (tokenInfo*) malloc(sizeof(tokenInfo));
                *currentToken = getNextToken(&B, NULL);
            } else {
                printf("Syntax Error at line %d: Expected %s but got %s\n", 
                       currentToken->lineNumber, current->symbol, currentToken->type);
                fclose(fp);
                return NULL;
            }
        } else {
            char key[200];
            sprintf(key, "%s %s", current->symbol, currentToken->type);
            ProductionRule *rule = (ProductionRule*) get(T->associativeArray, key, currentToken->type);
            
            if (rule == NULL) {
                printf("Syntax Error at line %d: No production rule for %s with input %s\n", 
                       currentToken->lineNumber, current->symbol, currentToken->type);
                fclose(fp);
                return NULL;
            }
            
            current->children = (ParseTreeNode*) malloc((*rule)->RHSLength * sizeof(ParseTreeNode));
            current->numChildren = (*rule)->RHSLength;
            
            for (int i = (*rule)->RHSLength - 1; i >= 0; i--) {
                bool isLeaf = (strcmp((*rule)->RHS[i], "ε") != 0) && (strchr((*rule)->RHS[i], '<') == NULL);
                ParseTreeNode child = createParseTreeNode((*rule)->RHS[i], isLeaf, NULL, current);
                current->children[i] = child;
                stack[++top] = child;
            }
        }
    }
    
    fclose(fp);
    return PT;
}

void printParseTreeNode(ParseTreeNode node, FILE *fp) {
    if (node->isLeafNode && node->token != NULL) {
        fprintf(fp, "%-20s %-20s %-5d %-20s %-20s %-20s %-5s %-20s\n",
                node->token->lexeme,
                node->symbol,
                node->token->lineNumber,
                node->token->type,
                (strcmp(node->token->type, "TK_NUM") == 0 || strcmp(node->token->type, "TK_RNUM") == 0) ? node->token->lexeme : "---",
                node->parent ? node->parent->symbol : "ROOT",
                "yes",
                node->symbol);
    } else {
        fprintf(fp, "%-20s %-20s %-5s %-20s %-20s %-20s %-5s %-20s\n",
                "----",
                node->symbol,
                "----",
                "----",
                "----",
                node->parent ? node->parent->symbol : "ROOT",
                "no",
                node->symbol);
    }
}

void printParseTreeHelper(ParseTreeNode node, FILE *fp) {
    if (node == NULL) return;
    
    printParseTreeNode(node, fp);
    
    for (int i = 0; i < node->numChildren; i++) {
        printParseTreeHelper(node->children[i], fp);
    }
}

void printParseTree(ParseTree PT, char *outfile) {
    FILE *fp = fopen(outfile, "w");
    if (fp == NULL) {
        printf("Error: Cannot open output file %s\n", outfile);
        return;
    }
    
    fprintf(fp, "%-20s %-20s %-5s %-20s %-20s %-20s %-5s %-20s\n",
            "lexeme", "currentNode", "lineno", "tokenName", "valueIfNumber",
            "parentNodeSymbol", "isLeaf", "NodeSymbol");
    
    printParseTreeHelper(PT->root, fp);
    
    fclose(fp);
}
