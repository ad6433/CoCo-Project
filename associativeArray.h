/*
 @Group No.:    27
 @Authors:
    Aditya Agrawal    2022A7PS0160P
    Samarth           2022A7PS0096P
    Shashank Jha      2022A7PS1186P
    Shikhar Singh     2022A7PS1170P
*/

typedef struct linkedList* LinkedList;
struct linkedList
{
    char *key;
    void *data;
    LinkedList next;
};

typedef struct associativeArray* AssociativeArray;
struct associativeArray
{
    char **keys;
    int (*hashFunction)(char *);
    LinkedList *values;
    int size;
};

AssociativeArray newAssociativeArray(int (*hashFunction)(char*), int initialSize);

void insertAA(AssociativeArray A, char *row, char *column, void *value);

void *get(AssociativeArray A, char *row, char *column);

// Implement if needed
// AssociativeArray rehash(AssociativeArray A);

int poly_hash(char *s);