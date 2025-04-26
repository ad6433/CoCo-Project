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
void printParseTable(Table T, Grammar G) {
    FILE *fp = fopen("output.txt", "w");
    if (fp == NULL) {
        printf("Error: Unable to open output.txt for writing.\n");
        return;
    }

    int i, j;
    // Print header: first an empty cell, then each terminal.
    fprintf(fp, "%-15s", "");
    for (j = 0; j < G->numTerminals; j++) {
        fprintf(fp, "%-20s", G->T[j]);
    }
    fprintf(fp, "\n");
    
    // For each nonterminal (each row)
    for (i = 0; i < G->numNonTerminals; i++) {
        // Print the nonterminal as the row header.
        fprintf(fp, "%-15s", G->N[i]);
        for (j = 0; j < G->numTerminals; j++) {
            // Retrieve the production rule using the same key format used during insertion.
            ProductionRule rule = (ProductionRule) get(T->associativeArray, G->N[i], G->T[j]);
            if (rule != NULL) {
                // Create a summary string for the production rule.
                // For example: "S->... [RHSLen]" where "S" is the LHS.
                char buffer[256];
                sprintf(buffer, "%s->...[%d]", rule->LHS, rule->RHSLength);
                fprintf(fp, "%-20s", buffer);
            } else {
                fprintf(fp, "%-20s", "error");
            }
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

bool isTerminal(Grammar G, char* symbol){
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
    for (int i = 0; i < G->numTerminals; i++)
    {
        G->T[i] = (char *) malloc(100 * sizeof(char));
        fscanf(fp, "%s\n", G->T[i]);
    }
    
    fscanf(fp, "\n");
    
    G->N = (char **) malloc(G->numNonTerminals * sizeof(char *));
    for (int i = 0; i < G->numNonTerminals; i++)
    {
        G->N[i] = (char *) malloc(100 * sizeof(char));
        fscanf(fp, "%s\n", G->N[i]);
    }
    
    fscanf(fp, "\n");
    
    G->P = (ProductionRule *) malloc(G->numProductionRules * sizeof(ProductionRule));
    
    char line[1024];
    
    for (int i = 0; i < G->numProductionRules; i++)
    {
        G->P[i] = (ProductionRule) malloc(sizeof(struct productionRule));
        
        G->P[i]->LHS = (char *) malloc(100 * sizeof(char));
        
        G->P[i]->RHS = (char **) malloc(10 * sizeof(char *));
        for (int j = 0; j < 10; j++)
        {
            G->P[i]->RHS[j] = (char *) malloc(100 * sizeof(char));
        }
        
        if (fgets(line, sizeof(line), fp) == NULL)
        {
            break;
        }
        line[strcspn(line, "\r\n")] = '\0';

        char *token = strtok(line, " \t");
        if (token == NULL)
            continue;

        strcpy(G->P[i]->LHS, token);
        printf("LHS: %s -> ", G->P[i]->LHS);
        
        token = strtok(NULL, " \t");
        if (token && strcmp(token, "->") == 0)
        {
            token = strtok(NULL, " \t");
        }
        
        int j = 0;
        while (token != NULL)
        {
            strcpy(G->P[i]->RHS[j], token);
            printf("%s",G->P[i]->RHS[j]);
            j++;
            token = strtok(NULL, " \t");
        }
        G->P[i]->RHSLength = j;
        printf(" Length : %d",j);
        printf("\n");
    }
    
    return G;
}

FirstAndFollow computeFirstAndFollowSets(Grammar G)
{
    printf("Allocating memory for first and follow sets\n");

    FirstAndFollow F = (FirstAndFollow) malloc(sizeof(struct firstAndFollow));
    F->first = (Set **) malloc(G->numNonTerminals * sizeof(Set *));
    F->follow = (Set **) malloc(G->numNonTerminals * sizeof(Set *));

    for (int i = 0; i < G->numNonTerminals; i++)
    {
        F->first[i] = (Set *) malloc(sizeof(Set));
        F->first[i]->count = 0;
        F->follow[i] = (Set *) malloc(sizeof(Set));
        F->follow[i]->count = 0;
    }

    printf("Memory allocated\n");
    
    // HARDCODE
    printf("Computing First and Follow sets (HARDCODED)\n");
    Set **fi = F->first, **fo = F->follow;
    
    // <program>
    addToSet(fi[0], "TK_MAIN"); addToSet(fi[0], "TK_FUNID");
    addToSet(fo[0], "$");

    // <mainFunction>
    addToSet(fi[1], "TK_MAIN");
    addToSet(fo[1], "$");

    // <otherFunctions>
    addToSet(fi[2], "TK_FUNID"); addToSet(fi[2], "EPS");
    addToSet(fo[2], "TK_MAIN");

    // <function>
    addToSet(fi[3], "TK_FUNID");
    addToSet(fo[3], "TK_MAIN"); addToSet(fo[3], "TK_FUNID");

    // <input_par>
    addToSet(fi[4], "TK_INPUT");
    addToSet(fo[4], "TK_OUTPUT"); addToSet(fo[4], "TK_SEM");

    // <output_par>
    addToSet(fi[5], "TK_OUTPUT"); addToSet(fi[5], "EPS");
    addToSet(fo[5], "TK_SEM");

    // <parameter_list>
    addToSet(fi[6], "TK_INT"); addToSet(fi[6], "TK_REAL"); addToSet(fi[6], "TK_RECORD"); addToSet(fi[6], "TK_UNION"); addToSet(fi[6], "TK_RUID");
    addToSet(fo[6], "TK_SQR");

    // <dataType>
    addToSet(fi[7], "TK_INT"); addToSet(fi[7], "TK_REAL"); addToSet(fi[7], "TK_RECORD"); addToSet(fi[7], "TK_UNION");
    addToSet(fi[7], "TK_RUID");
    addToSet(fo[7], "TK_ID"); addToSet(fo[7], "TK_COLON:");

    // <primitiveDatatype>
    addToSet(fi[8], "TK_INT"); addToSet(fi[8], "TK_REAL");
    addToSet(fo[8], "TK_ID"); addToSet(fo[8], "TK_COLON:");
    // FOLLOW OF FIELD TYPE

    // <constructedDatatype>
    addToSet(fi[9], "TK_RECORD"); addToSet(fi[9], "TK_UNION"); addToSet(fi[9], "TK_RUID");
    addToSet(fo[9], "TK_ID"); addToSet(fo[9], "TK_COLON:");
    // FOLLOW OF FIELD TYPE

    // <remaining_list>
    addToSet(fi[10], "TK_COMMA"); addToSet(fi[10], "EPS");
    addToSet(fo[10], "TK_SQR");

    // <stmts>
    addToSet(fi[11], "TK_RECORD"); addToSet(fi[11], "TK_UNION"); addToSet(fi[11], "TK_DEFINETYPE"); addToSet(fi[11], "TK_TYPE"); addToSet(fi[11], "TK_ID"); addToSet(fi[11], "TK_READ"); addToSet(fi[11], "TK_WRITE"); addToSet(fi[11], "TK_IF"); addToSet(fi[11], "TK_WHILE"); addToSet(fi[11], "TK_SQL"); addToSet(fi[11], "TK_CALL"); addToSet(fi[11], "TK_RETURN");
    addToSet(fo[11], "TK_END");

    // <typeDefinitions>
    addToSet(fi[12], "TK_RECORD"); addToSet(fi[12], "TK_UNION"); addToSet(fi[12], "TK_DEFINETYPE"); addToSet(fi[12], "EPS");
    addToSet(fo[12], "TK_TYPE"); addToSet(fo[12], "TK_ID"); addToSet(fo[12], "TK_READ"); addToSet(fo[12], "TK_WRITE"); addToSet(fo[12], "TK_IF"); addToSet(fo[12], "TK_WHILE"); addToSet(fo[12], "TK_SQL"); addToSet(fo[12], "TK_CALL"); addToSet(fo[12], "TK_RETURN");

    // <actualOrRedefined>
    addToSet(fi[13], "TK_RECORD"); addToSet(fi[13], "TK_UNION"); addToSet(fi[13], "TK_DEFINETYPE");
    addToSet(fo[13], "TK_RECORD"); addToSet(fo[13], "TK_UNION"); addToSet(fo[13], "TK_DEFINETYPE"); addToSet(fo[13], "TK_TYPE"); addToSet(fo[13], "TK_ID"); addToSet(fo[13], "TK_READ"); addToSet(fo[13], "TK_WRITE"); addToSet(fo[13], "TK_IF"); addToSet(fo[13], "TK_WHILE"); addToSet(fo[13], "TK_SQL"); addToSet(fo[13], "TK_CALL"); addToSet(fo[13], "TK_RETURN");

    // <typeDefinition>
    addToSet(fi[14], "TK_UNION"); addToSet(fi[14], "TK_RECORD");
    addToSet(fo[14], "TK_RECORD"); addToSet(fo[14], "TK_UNION"); addToSet(fo[14], "TK_DEFINETYPE"); addToSet(fo[14], "TK_TYPE"); addToSet(fo[14], "TK_ID"); addToSet(fo[14], "TK_READ"); addToSet(fo[14], "TK_WRITE"); addToSet(fo[14], "TK_IF"); addToSet(fo[14], "TK_WHILE"); addToSet(fo[14], "TK_SQL"); addToSet(fo[14], "TK_CALL"); addToSet(fo[14], "TK_RETURN");

    // <fieldDefinitions>
    addToSet(fi[15], "TK_TYPE");
    addToSet(fo[15], "TK_ENDRECORD"); addToSet(fo[15], "TK_ENDUNION");

    // <fieldDefinition>
    addToSet(fi[16], "TK_TYPE");
    addToSet(fo[16], "TK_TYPE");
    addToSet(fo[16], "TK_ENDRECORD"); addToSet(fo[16], "TK_ENDUNION");

    // <fieldType>
    addToSet(fi[17], "TK_RECORD"); addToSet(fi[17], "TK_UNION"); addToSet(fi[17], "TK_RUID"); addToSet(fi[17], "TK_INT"); addToSet(fi[17], "TK_REAL");
    addToSet(fo[17], "TK_COLON");

    // <moreFields>
    addToSet(fi[18], "TK_TYPE"); addToSet(fi[18], "EPS"); 
    addToSet(fo[18], "TK_ENDRECORD"); addToSet(fo[18], "TK_ENDUNION");

    // <declarations>
    addToSet(fi[19], "TK_TYPE"); addToSet(fi[19], "EPS");
    addToSet(fo[19], "TK_ID"); addToSet(fo[19], "TK_READ"); addToSet(fo[19], "TK_WRITE"); addToSet(fo[19], "TK_IF"); addToSet(fo[19], "TK_WHILE"); addToSet(fo[19], "TK_SQL"); addToSet(fo[19], "TK_CALL");
    addToSet(fo[19], "TK_RETURN");

    // <declaration>
    addToSet(fi[20], "TK_TYPE");
    addToSet(fo[20], "TK_TYPE");addToSet(fo[20], "TK_ID"); addToSet(fo[20], "TK_READ"); addToSet(fo[20], "TK_WRITE"); addToSet(fo[20], "TK_IF"); addToSet(fo[20], "TK_WHILE"); addToSet(fo[20], "TK_SQL"); addToSet(fo[20], "TK_CALL");
    addToSet(fo[20], "TK_RETURN");

    // <global_or_not>
    addToSet(fi[21], "EPS"); addToSet(fi[21], "TK_COLON:"); 
    addToSet(fo[21], "TK_SEM");

    // <otherStmts>
    addToSet(fi[22], "TK_ID"); addToSet(fi[22], "TK_READ"); addToSet(fi[22], "TK_WRITE"); addToSet(fi[22], "TK_IF"); addToSet(fi[22], "TK_WHILE"); addToSet(fi[22], "TK_SQL"); addToSet(fi[22], "TK_CALL"); addToSet(fi[22], "EPS");
    addToSet(fo[22], "TK_RETURN"); addToSet(fo[22], "TK_ENDWHILE"); addToSet(fo[22], "TK_ENDIF"); addToSet(fo[22], "TK_ELSE");

    // <stmt>
    addToSet(fi[23], "TK_ID"); addToSet(fi[23], "TK_READ"); addToSet(fi[23], "TK_WRITE"); addToSet(fi[23], "TK_IF"); addToSet(fi[23], "TK_WHILE"); addToSet(fi[23], "TK_SQL"); addToSet(fi[23], "TK_CALL");
    addToSet(fo[23], "TK_ID"); addToSet(fo[23], "TK_READ"); addToSet(fo[23], "TK_WRITE"); addToSet(fo[23], "TK_IF"); addToSet(fo[23], "TK_WHILE"); addToSet(fo[23], "TK_SQL"); addToSet(fo[23], "TK_CALL");addToSet(fo[23], "TK_RETURN"); addToSet(fo[23], "TK_ENDWHILE"); addToSet(fo[23], "TK_ENDIF"); addToSet(fo[23], "TK_ELSE");

    // <assignmentStmt>
    addToSet(fi[24], "TK_ID");
    addToSet(fo[24], "TK_ID"); addToSet(fo[24], "TK_READ"); addToSet(fo[24], "TK_WRITE"); addToSet(fo[24], "TK_IF"); addToSet(fo[24], "TK_WHILE"); addToSet(fo[24], "TK_SQL"); addToSet(fo[24], "TK_CALL");addToSet(fo[24], "TK_RETURN"); addToSet(fo[24], "TK_ENDWHILE"); addToSet(fo[24], "TK_ENDIF"); addToSet(fo[24], "TK_ELSE");

    // <singleOrRecId>
    addToSet(fi[25], "TK_ID");
    addToSet(fo[25], "TK_ASSIGNOP");

    // <option_single_constructed>
    addToSet(fi[26], "TK_DOT"); addToSet(fi[26], "EPS");
    addToSet(fo[26], "TK_ASSIGNOP");

    // <oneExpansion>
    addToSet(fi[27], "TK_DOT");
    addToSet(fo[27], "TK_DOT"); addToSet(fo[27], "TK_ASSIGNOP");

    // <moreExpansions>
    addToSet(fi[28], "EPS"); addToSet(fi[28], "TK_DOT");
    addToSet(fo[28], "TK_ASSIGNOP");

    // <funCallStmt>
    addToSet(fi[29], "TK_SQL"); addToSet(fi[29], "TK_CALL");
    addToSet(fo[29], "TK_ID"); addToSet(fo[29], "TK_READ"); addToSet(fo[29], "TK_WRITE"); addToSet(fo[29], "TK_IF"); addToSet(fo[29], "TK_WHILE"); addToSet(fo[29], "TK_SQL"); addToSet(fo[29], "TK_CALL");addToSet(fo[29], "TK_RETURN"); addToSet(fo[29], "TK_ENDWHILE"); addToSet(fo[29], "TK_ENDIF"); addToSet(fo[29], "TK_ELSE");

    // <outputParameters>
    addToSet(fi[30], "TK_SQL"); addToSet(fi[30], "EPS");
    addToSet(fo[30], "TK_CALL");

    // <inputParameters>
    addToSet(fi[31], "TK_SQL");
    addToSet(fo[31], "TK_SEM");

    // <iterativeStmt>
    addToSet(fi[32], "TK_WHILE");
    addToSet(fo[32], "TK_ID"); addToSet(fo[32], "TK_READ"); addToSet(fo[32], "TK_WRITE"); addToSet(fo[32], "TK_IF"); addToSet(fo[32], "TK_WHILE"); addToSet(fo[32], "TK_SQL"); addToSet(fo[32], "TK_CALL");addToSet(fo[32], "TK_RETURN"); addToSet(fo[32], "TK_ENDWHILE"); addToSet(fo[32], "TK_ENDIF"); addToSet(fo[32], "TK_ELSE");

    // <conditionalStmt>
    addToSet(fi[33], "TK_IF");
    addToSet(fo[33], "TK_ID"); addToSet(fo[33], "TK_READ"); addToSet(fo[33], "TK_WRITE"); addToSet(fo[33], "TK_IF"); addToSet(fo[33], "TK_WHILE"); addToSet(fo[33], "TK_SQL"); addToSet(fo[33], "TK_CALL");addToSet(fo[33], "TK_RETURN"); addToSet(fo[33], "TK_ENDWHILE"); addToSet(fo[33], "TK_ENDIF"); addToSet(fo[33], "TK_ELSE");

    // <elsePart>
    addToSet(fi[34], "TK_ELSE"); addToSet(fi[34], "TK_ENDIF");
    addToSet(fo[34], "TK_RETURN"); addToSet(fo[34], "TK_ENDWHILE"); addToSet(fo[34], "TK_ENDIF"); addToSet(fo[34], "TK_ELSE");

    // <ioStmt>
    addToSet(fi[35], "TK_READ"); addToSet(fi[35], "TK_WRITE");
    addToSet(fo[35], "TK_ID"); addToSet(fo[35], "TK_READ"); addToSet(fo[35], "TK_WRITE"); addToSet(fo[35], "TK_IF"); addToSet(fo[35], "TK_WHILE"); addToSet(fo[35], "TK_SQL"); addToSet(fo[35], "TK_CALL");addToSet(fo[35], "TK_RETURN"); addToSet(fo[35], "TK_ENDWHILE"); addToSet(fo[35], "TK_ENDIF"); addToSet(fo[35], "TK_ELSE");

    // <arithmeticExpression>
    addToSet(fi[36], "TK_NUM"); addToSet(fi[36], "TK_RNUM"); addToSet(fi[36], "TK_ID"); addToSet(fi[36], "TK_OP");
    addToSet(fo[36], "TK_CL"); addToSet(fo[36], "TK_SEM");

    // <arithmeticExpressionPrime>
    addToSet(fi[37], "EPS"); addToSet(fi[37], "TK_PLUS"); addToSet(fi[37], "TK_MINUS");
    addToSet(fo[37], "TK_CL"); addToSet(fo[37], "TK_SEM");

    // <lowPrecedenceOperator>
    addToSet(fi[38], "TK_PLUS"); addToSet(fi[38], "TK_MINUS");
    addToSet(fo[38], "TK_NUM"); addToSet(fo[38], "TK_RNUM"); addToSet(fo[38], "TK_ID"); addToSet(fo[38], "TK_OP");

    // <highPrecedenceOperator>
    addToSet(fi[39], "TK_MUL"); addToSet(fi[39], "TK_DIV");
    addToSet(fo[39], "TK_NUM"); addToSet(fo[39], "TK_RNUM"); addToSet(fo[39], "TK_ID"); addToSet(fo[39], "TK_OP");

    // <term>
    addToSet(fi[40], "TK_NUM"); addToSet(fi[40], "TK_RNUM"); addToSet(fi[40], "TK_ID"); addToSet(fi[40], "TK_OP");addToSet(fo[40], "TK_PLUS"); addToSet(fo[40], "TK_MINUS"); addToSet(fo[40], "TK_CL"); addToSet(fo[40], "TK_SEM");

    // <termPrime>
    addToSet(fi[41], "TK_MUL"); addToSet(fi[41], "TK_DIV"); addToSet(fi[41], "EPS");
    addToSet(fo[41], "TK_PLUS"); addToSet(fo[41], "TK_MINUS"); addToSet(fo[41], "TK_CL"); addToSet(fo[41], "TK_SEM");

    // <factor>
    addToSet(fi[42], "TK_NUM"); addToSet(fi[42], "TK_RNUM"); addToSet(fi[42], "TK_ID"); addToSet(fi[42], "TK_OP");
    addToSet(fo[42], "TK_MUL"); addToSet(fo[42], "TK_DIV");addToSet(fo[42], "TK_PLUS"); addToSet(fo[42], "TK_MINUS"); addToSet(fo[42], "TK_CL"); addToSet(fo[42], "TK_SEM");

    // <booleanExpression>
    addToSet(fi[43], "TK_OP"); addToSet(fi[43], "TK_NOT"); addToSet(fi[43], "TK_NUM"); addToSet(fi[43], "TK_RNUM"); addToSet(fi[43], "TK_ID");
    addToSet(fo[43], "TK_CL");

    // <var>
    addToSet(fi[44], "TK_NUM"); addToSet(fi[44], "TK_RNUM"); addToSet(fi[44], "TK_ID");
    addToSet(fo[44], "TK_CL");addToSet(fo[44], "TK_MUL"); addToSet(fo[44], "TK_DIV");addToSet(fo[44], "TK_PLUS"); addToSet(fo[44], "TK_MINUS"); addToSet(fo[44], "TK_SEM");addToSet(fo[44], "TK_LT"); addToSet(fo[44], "TK_LE"); addToSet(fo[44], "TK_GT"); addToSet(fo[44], "TK_GE"); addToSet(fo[44], "TK_EQ"); addToSet(fo[44], "TK_NE");

    // <logicalOp>
    addToSet(fi[45], "TK_AND"); addToSet(fi[45], "TK_OR");
    addToSet(fo[45], "TK_OP"); 

    // <relationalOp>
    addToSet(fi[46], "TK_LT"); addToSet(fi[46], "TK_LE"); addToSet(fi[46], "TK_GT"); addToSet(fi[46], "TK_GE"); addToSet(fi[46], "TK_EQ"); addToSet(fi[46], "TK_NE");
    addToSet(fo[46], "TK_NUM"); addToSet(fo[46], "TK_RNUM"); addToSet(fo[46], "TK_ID");

    // <returnStmt>
    addToSet(fi[47], "TK_RETURN");
    addToSet(fo[47], "TK_END");

    // <optionalReturn>
    addToSet(fi[48], "EPS"); addToSet(fi[48], "TK_SQL");
    addToSet(fo[48], "TK_SEM");

    // <idList>
    addToSet(fi[49], "TK_ID");
    addToSet(fo[49], "TK_SQR");

    // <more_ids>
    addToSet(fi[50], "TK_COMMA"); addToSet(fi[50], "EPS");
    addToSet(fo[50], "TK_SQR");

    // <definetypestmt>
    addToSet(fi[51], "TK_DEFINETYPE");
    addToSet(fo[51], "TK_RECORD"); addToSet(fo[51], "TK_UNION"); addToSet(fo[51], "TK_DEFINETYPE"); addToSet(fo[51], "TK_TYPE"); addToSet(fo[51], "TK_ID"); addToSet(fo[51], "TK_READ"); addToSet(fo[51], "TK_WRITE"); addToSet(fo[51], "TK_IF"); addToSet(fo[51], "TK_WHILE"); addToSet(fo[51], "TK_SQL"); addToSet(fo[51], "TK_CALL"); addToSet(fo[51], "TK_RETURN");

    // <A>
    addToSet(fi[52], "TK_RECORD"); addToSet(fi[52], "TK_UNION");
    addToSet(fo[52], "TK_RUID");

    // HARDCODE FINISH

    printf("First and Follow sets computed\n");
    return F;
}

void createParseTable(FirstAndFollow F, Table T, Grammar G) {
    T->associativeArray = newAssociativeArray(poly_hash, G->numNonTerminals * G->numTerminals);
    printf("Number of NT:%d",G->numNonTerminals);

    for (int i = 0; i < G->numProductionRules; i++) {
        char *nonTerminal = G->P[i]->LHS;
        printf("NT is %s",nonTerminal);
        int LHSindex = 0;
        
        // Find LHS index in the non-terminals list
        for (; LHSindex < G->numNonTerminals; LHSindex++) {
            if (strcmp(G->N[LHSindex], nonTerminal) == 0) 
            break;
        }

        if (LHSindex == G->numNonTerminals) 
            continue; // LHS not found in non-terminals

        int gofurther = 0; // Use int for strict C compatibility

        for (int j = 0; j < G->P[i]->RHSLength; j++) {
            char *RHSsymbol = G->P[i]->RHS[j];
            printf("RHS[%d]: %s  ",j,RHSsymbol);

            if (isTerminal(G, RHSsymbol)) {
                if(LHSindex == 10)
                printf("Symbol %s is terminal\n",RHSsymbol);
                insertAA(T->associativeArray, nonTerminal, RHSsymbol, G->P[i]);
                break;
            } else {
                int index = 0;
                for (; index < G->numNonTerminals; index++) {
                    if (strcmp(G->N[index], RHSsymbol) == 0) 
                        break;
                }

                if(strcmp(RHSsymbol,"EPS") == 0){
                    for (int k = 0; k < F->follow[LHSindex]->count; k++) {
                        insertAA(T->associativeArray, nonTerminal, F->follow[LHSindex]->symbols[k], G->P[i]);
                    }
                    continue;
                }
                else if (index == G->numNonTerminals ) {
                    continue; // RHS symbol not found in non-terminals
                }
                // Add FIRST set of RHSsymbol to parse table
                for (int k = 0; k < F->first[index]->count; k++) {
                    char *terminal = F->first[index]->symbols[k];
                    if (strcmp(terminal, "EPS") == 0) {
                        
                        gofurther = 1;
                        continue;
                    }
                    insertAA(T->associativeArray, nonTerminal, terminal, G->P[i]);
                }

                if (!gofurther) 
                    break;
            }
        }
        printf("\n");
        // If EPS was present in RHS, add FOLLOW set of LHS
        if (gofurther) {
            for (int k = 0; k < F->follow[LHSindex]->count; k++) {
                insertAA(T->associativeArray, nonTerminal, F->follow[LHSindex]->symbols[k], G->P[i]);
            }
        }
    }
    printParseTable(T,G);
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

ParseTree parseInputSourceCode(char *testcaseFile, Table T, Grammar G, FirstAndFollow F) {
    FILE *fp = fopen(testcaseFile, "r");
    if (fp == NULL) {
        // printf("Error: Cannot open file %s\n", testcaseFile);
        return NULL;
    }
    // printf("[DEBUG] Opened file %s successfully.\n", testcaseFile);

    HashMap lookup;
    initializeLookupTable(&lookup);
    // printf("[DEBUG] Lookup table initialized.\n");

    twinBuffer B;
    initializeTwinBuffer(&B, fp);
    // printf("[DEBUG] Twin buffer initialized.\n");

    getStream(&B);
    // printf("[DEBUG] Stream fetched from twin buffer.\n");

    tokenInfo *currentToken = (tokenInfo *) malloc(sizeof(tokenInfo));
    *currentToken = getNextToken(&B, &lookup);
    // printf("[DEBUG] First token fetched: lexeme = %s, type = %s, line = %d\n",
        //    currentToken->lexeme, currentToken->type, currentToken->lineNumber;

    // Skip comment tokens
    while(strcmp(currentToken->type, "TK_COMMENT") == 0) {
        // printf("[DEBUG] Skipping comment token: %s\n", currentToken->lexeme);
        *currentToken = getNextToken(&B, &lookup);
        if (currentToken->end) break;
    }
    
    // printf("First token after comments: lexeme = %s, type = %s, line = %d\n", 
        //    currentToken->lexeme, currentToken->type, currentToken->lineNumber;

    ParseTreeNode root = createParseTreeNode(G->startSymbol, 0, NULL, NULL);
    // printf("[DEBUG] Created root parse tree node with symbol: %s\n", G->startSymbol);
    ParseTree PT = (ParseTree) malloc(sizeof(struct parseTree));
    PT->root = root;
    
    ParseTreeNode stack[1000]; 
    int top = 0;
    stack[top] = root;
    // printf("[DEBUG] Root pushed onto the stack.\n");

    while (top >= 0) {
        ParseTreeNode current = stack[top--];
        // printf("[DEBUG] Popped from stack: current symbol = %s\n", current->symbol);

        if (current->isLeafNode) {
            // printf("[DEBUG] Processing leaf node: %s\n", current->symbol);
            if (strcmp(current->symbol, currentToken->type) == 0) {
                current->token = currentToken;
                // printf("[DEBUG] Matched leaf node. Token %s assigned to node %s\n", 
                       currentToken->lexeme, current->symbol;
                *currentToken = getNextToken(&B, &lookup);
                // printf("[DEBUG] Fetched next token: lexeme = %s, type = %s, line = %d\n",
                       currentToken->lexeme, currentToken->type, currentToken->lineNumber;
                while(strcmp(currentToken->type, "TK_COMMENT") == 0) {
                    // printf("[DEBUG] Skipping comment token: %s\n", currentToken->lexeme);
                    *currentToken = getNextToken(&B, &lookup);
                    // printf("[DEBUG] Fetched next token after comment: lexeme = %s, type = %s, line = %d\n",
                        //    currentToken->lexeme, currentToken->type, currentToken->lineNumber;
                }
            } else {
                printf("Syntax Error at line %d: Expected %s but got %s\n", 
                       currentToken->lineNumber, current->symbol, currentToken->type);
                fclose(fp);
                *currentToken = getNextToken(&B, &lookup);
                while(strcmp(currentToken->type, "TK_COMMENT") == 0) {
                    *currentToken = getNextToken(&B, &lookup);
                    if (currentToken->end) break;
                }
            }
        } else {
            // printf("[DEBUG] Processing non-terminal node: %s with current token %s\n",
                //    current->symbol, currentToken->type;
            ProductionRule rule = (ProductionRule) get(T->associativeArray, current->symbol, currentToken->type);
            
            if (rule == NULL) {
                printf("Syntax Error at line %d: No production rule for %s with input %s\n", 
                       currentToken->lineNumber, current->symbol, currentToken->type);
                int LHSindex = 0;

                // Find LHS index in the non-terminals list
                for (; LHSindex < G->numNonTerminals; LHSindex++) {
                    if (strcmp(G->N[LHSindex], current->symbol) == 0) 
                    break;
                }
                if(isInSet(F->follow[LHSindex],currentToken->type)){
                    // synset
                    printf("Syn Set is used");
                    continue;
                }
                else{
                    while(strcmp(currentToken->type, "TK_COMMENT") == 0) {
                        *currentToken = getNextToken(&B, &lookup);
                        if (currentToken->end) break;
                    }
                    continue;
                }
            }
            // printf("[DEBUG] Production rule found for %s -> ", current->symbol);
            for (int j = 0; j < rule->RHSLength; j++) {
                // printf("%s ", rule->RHS[j]);
            }
            // printf("\n");

            current->children = (ParseTreeNode*) malloc(rule->RHSLength * sizeof(ParseTreeNode));
            current->numChildren = rule->RHSLength;

            for (int i = rule->RHSLength - 1; i >= 0; i--) {
                int isLeaf = false;
                if (strcmp(rule->RHS[i], "EPS") != 0 && (rule->RHS[i][0] != '<'))
                    isLeaf = true;
                ParseTreeNode child = createParseTreeNode(rule->RHS[i], isLeaf, NULL, current);
                current->children[i] = child;
                // printf("[DEBUG] Created child node: %s (isLeaf: %d)\n", rule->RHS[i], isLeaf);
                
                if (strcmp(rule->RHS[i], "EPS") != 0) {  
                    stack[++top] = child;
                    // printf("[DEBUG] Pushed child node %s onto the stack.\n", child->symbol);
                }
            }
        }
    }
    // printf("[DEBUG] Parsing complete.\n");
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

