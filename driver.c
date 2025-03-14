#include "parser.h"
#include "lexer.h"
#include <stdio.h>



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
            printf("EXITING\n");
            return 0;

        case 1:
            removeComments(argv[1], "no-comments.txt");
            printfile(fopen("no-comments.txt", "r"));
            break;

        case 2:
            HashMap lookup;
            initializeLookupTable(lookup);
            twinBuffer tb;
            initializeTwinBuffer(&tb, fp);
            
            tokenInfo token;
            while (1)
            {
                token = getNextToken(&tb, &lookup);
                printf("Line no. %d\t Lexeme %s\t \tToken %s\n", token.lineNumber, token.lexeme, token.type);
                if (token.end) break;
            }
            break;
        case 3:
            
        }
    }
    while (choice != 0);
}