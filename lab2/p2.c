#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STU_NAME_MAX 31

typedef struct {
    int stuID;
    char* stuName;
} EleType;

typedef struct Node *PtrToNode; // Pointer to Node
typedef PtrToNode List; // Header of Linked List

struct Node {
    EleType ele; // Element of the node
    PtrToNode next; // Next node of the node
};

// Function description은 각 함수를 정의하는 부분에 작성됨.

// 함수 선언

// Main Functions
void Insert(EleType X, List L);
void Delete(int id, List L);
void Find(int id, List L);
void PrintList(List L);

// Other Functions
List MakeEmpty();
List FindPreNode(int id, List L);
void FreeNode(PtrToNode P);
void DeleteList(List L);
void PrintCurrentList(List L);

FILE *output;

void main() {
    FILE *input;
    char func_selected;
    int stuID_temp;
    char stuName_temp[STU_NAME_MAX];
    EleType ele_temp;
    List L = MakeEmpty();

    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");

    while(1) {
        fscanf(input, " %c", &func_selected);
        if(feof(input)) { // Break at the end of file.
            break;
        }
        switch(func_selected) {
            case 'i' : // Insert
                fscanf(input, "%d ", &ele_temp.stuID); // Read id.
                int i;
                for(i = 0; i < STU_NAME_MAX; i++) { // Read name to end of the line.
                    int isEOF = fscanf(input, "%c", &stuName_temp[i]); // Read a character.
                    if(stuName_temp[i] == '\r' || stuName_temp[i] == '\n' || isEOF == EOF) { // If it's CR or NL or EOF, (end of the line or end of file)
                        stuName_temp[i] = '\0'; // Convert it to NULL,
                        break; // And break.
                    }
                }
                ele_temp.stuName = (char*)malloc(sizeof(char)*(strlen(stuName_temp)+1)); // +1 for NULL
                strcpy(ele_temp.stuName, stuName_temp);

                Insert(ele_temp, L);
                break;
            case 'd' : // Delete
                fscanf(input, "%d", &stuID_temp);
                Delete(stuID_temp, L);
                break;
            case 'f' : // Find
                fscanf(input, "%d", &stuID_temp);
                Find(stuID_temp, L);
                break;
            case 'p' : // Print
                PrintList(L);
                break;
            default :
                break;
        }
    }
    
    // Free
    DeleteList(L);

    // Close files
    fclose(input);
    fclose(output);
}

// 함수 정의

// input으로 Element X와 Linked List의 Header인 L을 받아서,
// (Linked List가 student ID에 따라 정렬되도록 하는) Linked List 안의 적절한 위치에 Element X를 가지는 노드를 삽입하는 함수.
void Insert(EleType X, List L) {
    PtrToNode P = L, TmpNode;
    for (;; P = P->next) {
        if(P->next == NULL || X.stuID < P->next->ele.stuID) {
            TmpNode = (PtrToNode)malloc(sizeof(struct Node));
            TmpNode->ele = X;
            TmpNode->next = P->next;
            P->next = TmpNode;
            fprintf(output, "Insertion Success : %d\n", TmpNode->ele.stuID);
            PrintCurrentList(L);
            break;
        }
        if(X.stuID == P->next->ele.stuID) {
            fprintf(output, "Insertion Failed. Id %d already exists.\n", X.stuID);
            break;
        }
    }
}

// input으로 Student ID와 Linked List의 Header인 L을 받아서,
// 주어진 Linked List에 해당 id를 가지는 노드가 존재할 경우 삭제하는 함수.
void Delete(int id, List L) {
    PtrToNode Pre = FindPreNode(id, L); // The previous node of the node that has given id.
    PtrToNode P = Pre->next; // The node that has given id.
    if(P != NULL) {
        Pre->next = P->next;
        FreeNode(P);
        fprintf(output, "Deletion Success : %d\n", id);
        PrintCurrentList(L);
    }
    else {
        fprintf(output, "Deletion Failed : Student ID %d is not in the list.\n", id);
    }
}

// input으로 Student ID와 Linked List의 Header인 L을 받아서,
// 주어진 Linked List에 해당 id를 가지는 노드가 존재하는지 찾고, 존재할 경우 해당 노드가 가지는 student ID와 Name을 출력하는 함수.
void Find(int id, List L) {
    PtrToNode P = FindPreNode(id, L)->next; // The node that has given id.
    if(P != NULL) {
        fprintf(output, "Find Success : %d %s\n", P->ele.stuID, P->ele.stuName);
    }
    else {
        fprintf(output, "Find %d Failed. There is no student ID\n", id);
    }
}

// input으로 Linked List의 Header인 L을 받아서,
// 해당 Linked List의 각 노드들(이 가진 정보)를 순서대로 출력하는 함수.
void PrintList(List L) {
    PtrToNode P = L->next;
    fprintf(output, "-----LIST-----\n");
    for(; P != NULL; P = P->next) {
        fprintf(output, "%d %s\n", P->ele.stuID, P->ele.stuName);
    }
    fprintf(output, "--------------\n");
}

// Linked List를 만들기 위해 이의 Header 역할을 하는 Node(포인터)를 만들고 반환하는 함수.
List MakeEmpty() {
    List L = (List)malloc(sizeof(struct Node));
    L->ele.stuID = -1;
    L->ele.stuName = "";
    L->next = NULL;
    return L;
}

// Find 함수나 Delete 함수 등에서 활용하기 위해,
// input으로 Student ID와 Linked List의 Header인 L을 받아서,
// 주어진 Linked List에 해당 id를 가지는 노드가 존재하는지 찾고, 있을 경우 해당 노드의 전 노드, 없을 경우 마지막 노드를 반환하는 함수.
// (Delete 함수에서는 삭제할 노드의 전 노드에 접근할 필요가 있어서 전 노드를 반환하며,
// 해당 노드가 필요할 경우 여기서 반환한 것의 다음 노드에 바로 접근하면 되기에 따로 해당 노드를 찾는 함수는 만들지 않음,
// 해당 id를 가지는 노드가 없으면, next로 NULL을 가지는 마지막 노드가 반환되므로 해당 id를 가지는 노드가 없음을 판별할 수 있음.) 
PtrToNode FindPreNode(int id, List L) {
    PtrToNode P = L;
    for(;; P = P->next) {
        if(P->next == NULL || P->next->ele.stuID == id) {
            return P; // Return previous node of the node that has given id, for Delete function.
            // If the node that has given id isn't in the list, return the last node of the list.
        }
    }
}

// input으로 한 개 노드(포인터)를 받아서,
// 각 Node를 free할 때 안의 문자열도 free해줘야 하기에 이를 동시에 하기 위한 함수.
void FreeNode(PtrToNode P) {
    free(P->ele.stuName);
    free(P);
}

// input으로 Linked List의 Header인 L을 받아서,
// 해당 Linked List를 모두 삭제하며 동적할당을 해제하는 함수.
void DeleteList(List L) {
    PtrToNode P = L->next, TmpNode;
    while(P != NULL) {
        TmpNode = P->next;
        FreeNode(P);
        P = TmpNode;
    }
    free(L);
}

// input으로 Linked List의 Header인 L을 받아서,
// Insert 함수와 Delete 함수에서 해당 Linked List의 현재 상태(각 노드들이 가진 정보)를 출력할 때 쓰는 함수.
void PrintCurrentList(List L) {
    PtrToNode P = L->next;
    int i = (P != NULL);
    fprintf(output, "Current List > ");
	while(i) {
        fprintf(output, "%d %s", P->ele.stuID, P->ele.stuName);
        P = P->next;
        if(i = (P != NULL)) {
            fprintf(output, "-");
        }
    }
    fprintf(output, "\n");
}
