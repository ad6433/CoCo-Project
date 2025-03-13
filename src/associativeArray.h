typedef struct linkedList* LinkedList;
struct linkedList
{
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

void insert(AssociativeArray A, char *row, char *column, void *value);

// Implement if needed
// AssociativeArray rehash(AssociativeArray A);

int poly_hash(char *s);