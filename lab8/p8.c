#include <stdio.h>
#include <stdlib.h>

struct HeapStruct;
typedef struct HeapStruct* Heap;
struct HeapStruct {
    int cap; // Max heap capacity.
    int size; // Current heap size.
    int *ele; // Elements.
};

// Functions.
void InitHeap(Heap h, int cap);
void Insert(Heap h, int x);
int Find(Heap h, int x);
void Print(Heap h);
void DeleteHeap(Heap h);

FILE *output;

void main() {
    FILE *input;
    Heap h = (Heap)malloc(sizeof(struct HeapStruct));
    int cap, temp; // Capacity of heap, temp variable for input.
    char func_sel; // Selected function.

    // Open files.
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");

    // Read the size of heap.
    fscanf(input, "%d", &cap);

    // Initialize the heap.
    InitHeap(h, cap);
    
    while(1) {
        fscanf(input, " %c", &func_sel);
        if(feof(input)) { // Break at the end of file.
            break;
        }

        switch(func_sel) {
            case 'i' : // Insert.
                fscanf(input, "%d", &temp);
                if(!Find(h, temp)) {
                    Insert(h, temp);
                    fprintf(output, "insert %d\n", temp);
                }
                else {
                    fprintf(output, "%d is already in the heap.\n", temp);
                }
                break;

            case 'f' : // Find.
                fscanf(input, "%d", &temp);
                if(Find(h, temp)) {
                    fprintf(output, "%d is in the heap.\n", temp);
                }
                else {
                    fprintf(output, "%d is not in the heap.\n", temp);
                }
                break;

            case 'p' : // Print.
                Print(h);
                break;

            default :
                break;
        }
    }

    // Free.
    DeleteHeap(h);

    // Close files.
    fclose(input);
    fclose(output);
}

// Input : A heap pointer, given capacity.
// Initialize the heap with given capacity.
void InitHeap(Heap h, int cap) {
    h->cap = cap;
    h->size = 0;
    h->ele = (int*)malloc(sizeof(int)*(cap+1));
}

// Input : A heap, an element to insert.
// Insert the element into the heap.
void Insert(Heap h, int x) {
    int i;

    if(h->cap == h->size) { // If heap is full,
        fprintf(output, "Heap is full.\n");
        return;
    }

    // Repeat pecolating up the given element from the original last index + 1.
    for(i = ++h->size; i > 1 && h->ele[i/2] < x; i /= 2) {
        h->ele[i] = h->ele[i/2];
    }
    h->ele[i] = x;
}

// Input : A heap, an element to find.
// Find the element from the heap. If succeed to find, return 1, or else return 0.
int Find(Heap h, int x) {
    int i;
    
    for(i = 1; i <= h->size; i++) {
        if(h->ele[i] == x) { // If succeed to find,
            return 1;
        }
    }
    
    // If fail to find,
    return 0;
}

// Input : A heap.
// Print the heap.
void Print(Heap h) {
    int i;

    for(i = 1; i <= h->size; i++) {
        fprintf(output, "%d ", h->ele[i]);
    }
    fprintf(output, "\n");
}

// Input : A heap.
// Delete(deallocate) the heap.
void DeleteHeap(Heap h) {
    free(h->ele);
    free(h);
}
