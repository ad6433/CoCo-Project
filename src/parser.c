#include <stdio.h>
#include "parser.h"

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
        while (fscanf(fp, "%s", G->P[i]->RHS[j]) != EOF) j++;
    }
    return G;
}

FirstAndFollow computeFirstAndFollowSets (Grammar G)
{
    

}

void createParseTable (FirstAndFollow F, Table T)
{

}

ParseTree parseInputSourceCode (char *testcaseFile, Table T)
{

}

void printParseTree (ParseTree PT, char *outfile)
{

}