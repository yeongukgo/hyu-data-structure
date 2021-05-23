#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUEUE_SIZE_MAX 100
#define FIRST_REAR -1
#define FIRST_FRONT 0

typedef struct QueueRecord *Queue;
typedef int EleType; // In this program, I will use int as Element Type of Queue.

struct QueueRecord { // Linear Queue
    int Rear;
    int Front;
    EleType *Arr; // Array
};

// Function Declaration
Queue CreateQueue(int MaxEle);
void Enqueue(EleType X, Queue Q);
void Dequeue(Queue Q);
int IsFull(Queue Q);
int IsEmpty(Queue Q);
void FreeQueue(Queue Q);

// global variable for using in every functions
FILE *output;

void main() {

    FILE *input;
    int repeat, temp, i;
    char func_selected[4]; // This will have "enQ" or "deQ".

    // Open files.
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");
    
    Queue Q = CreateQueue(QUEUE_SIZE_MAX);
    if(Q == NULL) {
        printf("Exit program, because allocating Queue failed.\n");
        fclose(input);
        fclose(output);
        return;
    }

    fscanf(input, "%d", &repeat);

    for(i = 0; i < repeat; i++) {
        fscanf(input, "%s", func_selected);
        if(strcmp(func_selected, "enQ") == 0) { // enQ
            fscanf(input, "%d", &temp);
            Enqueue(temp, Q);
        }
        else if(strcmp(func_selected, "deQ") == 0) { // deQ
            Dequeue(Q);
        }
        else { // If the input text isn't "enQ" or "deQ",
            fprintf(output, "Error : The input text isn't ""enQ"" or ""deQ"".\n");
        }
    }
    
    // Free.
    FreeQueue(Q);

    // Close files.
    fclose(input);
    fclose(output);
}

// Input : Max Element Size of the Queue
// Function that create a Queue and initialize members of the struct
Queue CreateQueue(int MaxEle) {
    Queue Q = (Queue)malloc(sizeof(struct QueueRecord));
    if(Q == NULL) {
        fprintf(output, "Failed to allocate. Out of space.\n");
        return NULL;
    }
    
    Q->Arr = (EleType*)malloc(sizeof(EleType)*MaxEle);
    if(Q->Arr == NULL) {
        fprintf(output, "Failed to allocate. Out of space.\n");
        free(Q);
        return NULL;
    }

    Q->Rear = FIRST_REAR;
    Q->Front = FIRST_FRONT;

    return Q;
}

// Input : Element X, Queue Pointer Q
// Function that puts Element X into Queue Q
void Enqueue(EleType X, Queue Q) {
    if(IsFull(Q)) {
        fprintf(output, "Full\n");
    }
    else {
        Q->Arr[++Q->Rear] = X; // Increase Q->Rear, and then save X into Q->Arr[Q->Rear].
    }
}

// Input : Queue Pointer Q
// Function that prints and deletes the front element in the Queue Q
void Dequeue(Queue Q) {
    if(IsEmpty(Q)) {
        fprintf(output, "Empty\n");
    }
    else {
        fprintf(output, "%d\n", Q->Arr[Q->Front++]); // Print Q->Arr[Q->Front], and then increase Q->Front.
    }
}

// Input : Queue Pointer Q
// Function that returns whether the Queue Q is full or not
int IsFull(Queue Q) {
    if(Q->Rear == QUEUE_SIZE_MAX-1) {
        return 1;
    }
    return 0;
}

// Input : Queue Pointer Q
// Function that returns whether the Queue Q is Empty or not
int IsEmpty(Queue Q) {
    if(Q->Front > Q->Rear) {
        return 1;
    }
    return 0;
}

// Input : Queue Pointer Q
// Function that deallocate the Array of Queue Q and Queue Q.
void FreeQueue(Queue Q) {
    free(Q->Arr);
    free(Q);
}
