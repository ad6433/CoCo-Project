/*
 @Group No.:    27
 @Authors:
    Aditya Agrawal    2022A7PS0160P
    Samarth           2022A7PS0096P
    Shashank Jha      2022A7PS1186P
    Shikhar Singh     2022A7PS1170P
*/

#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./a.out <filename>\n");
        return -1;
    }

    FILE *fp = fopen(argv[1], "r");

    int choice;
    do
    {
        printf("ENTER CHOICE: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 0:
            {
                printf("EXITING\n");
                return 0;
            }
            case 1:
            {
                removeComments(argv[1], "no-comments.txt");
                printfile(fopen("no-comments.txt", "r"));
                break;
            }
            case 2:
            {
                HashMap lookup;
                initializeLookupTable(&lookup);
                twinBuffer tb;
                initializeTwinBuffer(&tb, fp);
                getStream(&tb);

                tokenInfo token;
                while (1)
                {
                    token = getNextToken(&tb, &lookup);
                    if (token.end) break;
                    printf("Line no. %d\t Lexeme %s\t \tToken %s\n", token.lineNumber, token.lexeme, token.type);
                }
                FILE *fp = fopen(argv[1], "r"); // RESET READER
                free(lookup);
                break;
            }
            case 3:
            {
                Grammar G = readGrammar(fopen("grammar.txt", "r"));
                FirstAndFollow F = computeFirstAndFollowSets(G);

                Table T = (Table) malloc(sizeof(struct table));
                createParseTable(F, T, G);
                // printParseTable(T, G);
                ParseTree parseTree = parseInputSourceCode(argv[1], T, G,F);
                printParseTree(parseTree, "parsetree.txt");
                printfile(fopen("parsetree.txt", "r"));
                break;
            }
            case 4:
            {
                clock_t start_time, end_time;

                double total_CPU_time, total_CPU_time_in_seconds;

                start_time = clock();

                Grammar G = readGrammar(fopen("grammar.txt", "r"));
                FirstAndFollow F = computeFirstAndFollowSets(G);

                Table T;
                createParseTable(F, T, G);
                ParseTree parseTree = parseInputSourceCode(argv[1], T, G,F);

                end_time = clock();

                total_CPU_time = (double) (end_time - start_time);

                total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

                printf("Total CPU time: %f\n", total_CPU_time);
                printf("Total CPU time in seconds: %f\n", total_CPU_time_in_seconds);

                break;
            }
            default:
            {
                printf("Enter value between 0 and 4\n");
            }
        }
    }
    while (choice != 0);
}