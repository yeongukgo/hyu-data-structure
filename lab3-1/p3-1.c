#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE_MAX 100
#define EMPTY_TOS -1

typedef struct StackRecord *Stack;
typedef int EleType; // In this program, I will use int as Element Type of Stack.

struct StackRecord {
    int Cap; // Capacity
    int TOS; // Top Of Stack
    EleType *Arr; // Array
};

// Function Declaration
Stack CreateStack(int MaxEle);
void Push(EleType X, Stack S);
void Pop(Stack S);
int IsFull(Stack S);
int IsEmpty(Stack S);
void FreeStack(Stack S);

// global variable for using in every functions
FILE *output;

void main() {

    FILE *input;
    int repeat, temp, i;
    char func_selected[5]; // This will have "push" or "pop".

    // Open files.
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");
    
    Stack S = CreateStack(STACK_SIZE_MAX);
    if(S == NULL) {
        printf("Exit program, because allocating Stack failed.\n");
        fclose(input);
        fclose(output);
        return;
    }

    fscanf(input, "%d", &repeat);

    for(i = 0; i < repeat; i++) {
        fscanf(input, "%s", func_selected);
        if(strcmp(func_selected, "push") == 0) { // push
            fscanf(input, "%d", &temp);
            Push(temp, S);
        }
        else if(strcmp(func_selected, "pop") == 0) { // pop
            Pop(S);
        }
        else { // If the input text isn't "push" or "pop",
            fprintf(output, "Error : The input text isn't ""push"" or ""pop"".\n");
        }
    }
    
    // Free.
    FreeStack(S);

    // Close files.
    fclose(input);
    fclose(output);
}

// Input : Max Element Size of the Stack
// Function that create a Stack and initialize members of the struct
Stack CreateStack(int MaxEle) {
    Stack S = (Stack)malloc(sizeof(struct StackRecord));
    if(S == NULL) {
        fprintf(output, "Failed to allocate. Out of space.\n");
        return NULL;
    }
    
    S->Arr = (EleType*)malloc(sizeof(EleType)*MaxEle);
    if(S->Arr == NULL) {
        fprintf(output, "Failed to allocate. out of space.\n");
        free(S);
        return NULL;
    }

    S->Cap = MaxEle;
    S->TOS = EMPTY_TOS;

    return S;
}

// Input : Element X, Stack Pointer S
// Function that puts Element X into Stack S
void Push(EleType X, Stack S) {
    if(IsFull(S)) {
        fprintf(output, "Full\n");
    }
    else {
        S->Arr[++S->TOS] = X; // Increase S->TOS, and then save X into S->Arr[S->TOS].
    }
}

// Input : Stack Pointer S
// Function that pops (prints and deletes) the top element in the Stack S
void Pop(Stack S) {
    if(IsEmpty(S)) {
        fprintf(output, "Empty\n");
    }
    else {
        fprintf(output, "%d\n", S->Arr[S->TOS--]); // Print S->Arr[S->TOS], and then decrease S->TOS.
    }
}

// Input : Stack Pointer S
// Function that returns whether the Stack S is full or not
int IsFull(Stack S) {
    if(S->TOS == S->Cap-1) {
        return 1;
    }
    return 0;
}

// Input : Stack Pointer S
// Function that returns whether the Stack S is Empty or not
int IsEmpty(Stack S) {
    if(S->TOS == EMPTY_TOS) {
        return 1;
    }
    return 0;
}

// Input : Stack Pointer S
// Function that deallocate the Array of Stack S and Stack S.
void FreeStack(Stack S) {
    free(S->Arr);
    free(S);
}
