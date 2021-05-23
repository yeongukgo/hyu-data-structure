#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXPRESSION_SIZE_MAX 101 // 1 more for NULL.
#define EMPTY_TOS -1

typedef struct StackRecord *Stack;
typedef char EleType; // In this program, I will use char as Element Type of Stack.

struct StackRecord {
    int Cap; // Capacity
    int TOS; // Top Of Stack
    EleType *Arr; // Array
};

// Functions about Stack.
Stack CreateStack(int MaxEle);
void Push(EleType X, Stack S);
char Pop(Stack S);
char Top(Stack S);
int IsFull(Stack S);
int IsEmpty(Stack S);
void FreeStack(Stack S);

// Function about conversion of infix to postfix.
int Prec(char c); // Precedence

// Functions about evaluation of postfix expression.
int Eval(char *postfix); // Evaluate
int Calc(int operand1, int operand2, char operator); // Calculate

// global variable for using in every functions
FILE *output;

void main() {

    FILE *input;
    char temp;
    char postfix[EXPRESSION_SIZE_MAX];
    int result;
   
    // Open files.
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");
    
    // The Stack that will have operators for conversion of infix to postfix.
    Stack S = CreateStack(EXPRESSION_SIZE_MAX);
    if(S == NULL) {
        printf("Exit program, because allocating Stack failed.\n");
        fclose(input);
        fclose(output);
        return;
    }

    // Read and print Infix Form, and save it as Postfix Form (conversion).
    fprintf(output, "Infix Form : ");
    int index = 0; // Index of postfix string.
    while(1) {
        fscanf(input, "%c", &temp);
        if(temp >=49 && temp <= 57) { // If temp is number,
            // Save it into postfix string.
            postfix[index] = temp;
            index++;
        }
        else if(temp == '#') { // If temp is end of expression,
            // Pop all the operators in the stack, and stop reading&conversion.
            while(Top(S) != '\0') {
                postfix[index] = Pop(S);
                index++;
            }
            postfix[index] = '\0';
            break;
        }
        else if(temp == '(') { // If temp is left parenthesis,
            Push(temp, S); // Push it in the stack.
        }
        else if(temp == ')') { // If temp is right parenthesis,
            // Pop all the operators until meeting left parenthesis.
            while(Top(S) != '(') {
                postfix[index] = Pop(S);
                index++;
            }
            Pop(S); // Delete left parenthesis.
        }
        else if(Prec(temp) > Prec(Top(S))) {
            // If the precedence of the top of the stack is less than the precedence of the temp,
            // or the top is '(',  Push temp.
            Push(temp, S);
        }
        else {
            // If the precedence of the top of the stack isn't less than the precedence of the temp,
            // Pop the top of stack and put it to postfix string.
            // Repeat it until meeting the operator have lower precedence.
            while(Prec(temp) <= Prec(Top(S))) {
                postfix[index] = Pop(S);
                index++;
            }
            // After repeat pop, push temp in the stack.
            Push(temp, S);
        }
        
        fprintf(output, "%c", temp);
    }
    fprintf(output, "\n");

    // Print Postfix Form and evaluate it.
    fprintf(output, "Postfix Form : %s\n", postfix);
    result = Eval(postfix);
    fprintf(output, "Evaluation Result : %d\n", result);

    // Free.
    FreeStack(S);

    // Close files.
    fclose(input);
    fclose(output);
}

// Input : Max Element Size of the Stack
// Function that create a Stack and initialize members of the struct.
Stack CreateStack(int MaxEle) {
    Stack S = (Stack)malloc(sizeof(struct StackRecord));
    if(S == NULL) {
        fprintf(output, "Failed to allocate. Out of space.\n");
        return NULL;
    }
    
    S->Arr = (EleType*)malloc(sizeof(EleType)*MaxEle);
    if(S->Arr == NULL) {
        fprintf(output, "Failed to allocate. Out of space.\n");
        free(S);
        return NULL;
    }

    S->Cap = MaxEle;
    S->TOS = EMPTY_TOS;

    return S;
}

// Input : Element X, Stack Pointer S
// Function that puts Element X into Stack S.
void Push(EleType X, Stack S) {
    if(IsFull(S)) {
        fprintf(output, "Failed to push. Stack is full.\n");
    }
    else {
        S->Arr[++S->TOS] = X; // Increase S->TOS, and then save X into S->Arr[S->TOS].
    }
}

// Input : Stack Pointer S
// Function that pops (returns and deletes) the top element in the Stack S.
char Pop(Stack S) {
    if(IsEmpty(S)) {
        return '\0'; // NULL
    }
    else {
        return S->Arr[S->TOS--]; // Return S->Arr[S->TOS], and then decrease S->TOS.
    }
}

// Input : Stack Pointer S
// Function that returns the top element in the Stack S
char Top(Stack S) {
    if(IsEmpty(S)) {
        return '\0'; // NULL
    }
    else {
        return S->Arr[S->TOS];
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
// Function that deallocate the array of Stack S and Stack S.
void FreeStack(Stack S) {
    free(S->Arr);
    free(S);
}

// Input : character c (operator)
// Function that returns the precedence of the operator.
int Prec(char c) {
    switch(c) {
        case '(' :
            return 0; // It's not precedence of '(', just for convenience.
        case '+' :
        case '-' :
            return 1;
        case '*' :
        case '/' :
        case '%' :
            return 2;
        default : // NULL or invalid input
            return -1;
    }
}

// Input : character array postfix
// Function that evaluate the postfix expression and return the result.
int Eval(char *postfix) {
    int result;
    int nums[EXPRESSION_SIZE_MAX]; // Temp array for numbers that are waiting to calculate.
    int operand[2];
    char temp; // Temp character that will have each operand or operator.

    int i;
    for(i = 0; i < EXPRESSION_SIZE_MAX; i++) {
        temp = postfix[i];
        if(temp == '\0') { // If temp is end of the expression,
            result = nums[i-1]; // Save last calculation to result.
            break; // And escape the loop.
        }
        else if(temp >= '1' && temp <= '9') { // If temp is number,
            nums[i] = temp-'0'; // Save into nums.
        }
        else if(Prec(temp) == 1 || Prec(temp) == 2) { // If temp is operator,
            int j, k;
            for(j = i-1, k = 1; j >= 0, k >= 0; j--) { // Find 2 operands before i, and put them into operand[].
                if(nums[j] != 0) { // Ignore 0.
                    operand[k] = nums[j];
                    k--;
                    nums[j] = 0; // Save 0 to ignore it, because this program use only 1-9 as operand.
                }
            }
            nums[i] = Calc(operand[0], operand[1], temp); // Caculate with operands and temp(operator).
        }
        else { // invalid input
            fprintf(output, "Failed to evaluate. Expression is invalid.\n");
            return 0;
        }
    }

    return result;
}

// Input : 2 operands and operator
// Function that calculate the operands with the operator
int Calc(int operand1, int operand2, char operator) {
    switch(operator) {
        case '+' :
            return operand1 + operand2;
        case '-' :
            return operand1 - operand2;
        case '*' :
            return operand1 * operand2;
        case '/' :
            return operand1 / operand2;
        case '%' :
            return operand1 % operand2;
        default : // invalid input
            fprintf(output, "Failed to calculate. operator is invalid.\n");
            return 0;
    }
}
