#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListNode *Position;
typedef Position List;
typedef struct HashTBl *HashTable;

struct ListNode {
    int Element;
};

struct HashTBl {
    int TableSize;
    List* TheLists;
};

// Functions.
void Insert(HashTable H, int value, int solution);
void Delete(HashTable H, int value, int solution);
int Find(HashTable H, int value, int solution);
void Print(HashTable H);
int Hash(int value, int size, int i, int solution);
HashTable CreateHashTable(int size);
void DeleteHashTable(HashTable H);

FILE* output;

int main(void) {
    FILE* input;
    HashTable H;
    int num, sol, tmp, loop, i;
    char ch;
    char str[10];

    // Open files.
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");

    fscanf(input, "%d", &num); // Read number of test cases.

    for(i = 0; i < num; i++) {
        fscanf(input, " %s", str); // Read collision solution.
        if(strcmp(str, "Linear") == 0) { sol = 1; }
        else if(strcmp(str, "Quadratic") == 0) { sol = 2; }
        else if(strcmp(str, "Double") == 0) { sol = 3; }
        fprintf(output, "%s\n", str);

        fscanf(input, "%d", &tmp); // Read size of hash table.
        H = CreateHashTable(tmp);
        
        loop = 1;
        while(loop) {
            fscanf(input, " %c", &ch); // Read instruction.
            switch(ch) {
                case 'i' :
                case 'd' :
                case 'f' :
                    fscanf(input, "%d", &tmp); // Read value to insert/delete/find.
            }
            switch(ch) {
                case 'i' : // Insert.
                    Insert(H, tmp, sol);
                    break;
                
                case 'd' : // Delete.
                    Delete(H, tmp, sol);
                    break;

                case 'f' : // Find.
                    Find(H, tmp, sol);
                    break;

                case 'p' : // Print.
                    Print(H);
                    break;

                case 'q' : // Quit.
                    loop = 0;
                    break;

                default : // Invalid Input. Skip the line.
                    fprintf(output, "Invalid Input : %c\n", ch);
                    while(1) {
                        fscanf(input, "%c", &ch);
                        if(ch == '\n' || feof(input)) { break; }
                    }
            }
        }
        fprintf(output, "\n");

        DeleteHashTable(H);
    }

    // Close files.
    fclose(input);
    fclose(output);

    return 0;
}

// Input : A hashtable, value to insert, collision solution.
// Insert given value into given hashtable.
void Insert(HashTable H, int value, int solution) {
    int i, h;

    // Repeating (TableSize) times can check all hashes in all cases
    // from given value with given three collision solutions.
    for(i = 0; i < H->TableSize; i++) {
        h = Hash(value, H->TableSize, i, solution);
        
        if(H->TheLists[h]->Element == 0 || H->TheLists[h]->Element == -1) { // Empty/deleted node. Insert here.
            H->TheLists[h]->Element = value;
            fprintf(output, "Inserted %d\n", value);
            return;
        }
        else if(H->TheLists[h]->Element == value) { // Already exists.
            fprintf(output, "Already exists\n");
            return;
        }
    }
    // All hashes in all cases from given value are full.
    fprintf(output, "Failed to Insert (Hash nodes for given value are full)\n");
}

// Input : A hashtable, value to delete, collision solution.
// Delete given value from given hashtable.
void Delete(HashTable H, int value, int solution) {
    int i, h;
    
    for(i = 0; i < H->TableSize; i++) {
        h = Hash(value, H->TableSize, i, solution);
        
        if(H->TheLists[h]->Element == 0) { // Empty node. Not exists.
            break;
        }
        else if(H->TheLists[h]->Element == value) { // Exists. Delete it.
            H->TheLists[h]->Element = -1;
            fprintf(output, "Deleted %d\n", value);
            return;
        }
    }
    // Not exists.
    fprintf(output, "%d not exists\n", value);
}

// Input : A hashtable, value to find, collision solution.
// Find given value from given hashtable.
int Find(HashTable H, int value, int solution) {
    int i, h;

    for(i = 0; i < H->TableSize; i++) {
        h = Hash(value, H->TableSize, i, solution);

        if(H->TheLists[h]->Element == 0) { // Empty node. Not found.
            break;
        }
        else if(H->TheLists[h]->Element == value) { // Found.
            fprintf(output, "%d\n", h);
            return h;
        }
    }
    // Not found.
    fprintf(output, "Not found\n");
    return -1;
}

// Input : A hashtable.
// Print all values in given hashtable.
void Print(HashTable H) {
    int i;

    for(i = 0; i < H->TableSize; i++) {
        if(H->TheLists[i]->Element == -1) { fprintf(output, "0"); }
        else { fprintf(output, "%d", H->TheLists[i]->Element); }

        if(i + 1 != H->TableSize) { fprintf(output, " "); }
    }
    fprintf(output, "\n");
}

// Input : Value to hash, size of hashtable, an integer for hashing, collision solution.
// Hash given value.
int Hash(int value, int size, int i, int solution) {
    int h = value % size;
    if(i == 0) { 
        return h;
    }

    switch(solution) {
        case 1 : // Linear.
            h = (h + i) % size;
            break;

        case 2 : // Quadratic
            h = (h + i * i) % size;
            break;

        case 3 : // Double
            h = (h + i * (7 - (value % 7))) % size;
    }
    return h;
}

// Input : Size of hashtable to create.
// Create a hashtable with given size, and return it.
HashTable CreateHashTable(int size) {
    int i;

    HashTable H = (HashTable)malloc(sizeof(struct HashTBl));
    H->TableSize = size;
    H->TheLists = (List*)malloc(sizeof(List)*size);

    for(i = 0; i < size; i++) {
        H->TheLists[i] = (Position)malloc(sizeof(struct ListNode));
        H->TheLists[i]->Element = 0;
    }

    return H;
}

// Input : A hashtable.
// Delete(deallocate) the given hashtable.
void DeleteHashTable(HashTable H) {
    int i;

    for(i = 0; i < H->TableSize; i++) {
        free(H->TheLists[i]);
    }

    free(H->TheLists);
    free(H);
}
