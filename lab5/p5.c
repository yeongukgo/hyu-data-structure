#include <stdio.h>
#include <stdlib.h>

struct TreeNode;
typedef struct TreeNode* BST; // Binary Search Tree.
typedef struct TreeNode* Node; // Node of binary search tree.

typedef int EleType; // In this program, I will use int as Element Type of (Binary Search) Tree. 

struct TreeNode {
    EleType Ele; // Element.
    BST Left; // Left child.
    BST Right; // Right child.
};

// Functions about binary search tree.
BST InsertNode(EleType X, BST T);
BST DeleteNode(EleType X, BST T);
Node FindNode(EleType X, BST T);
Node FindMax(BST T);
void DeleteTree(BST T);

// Functions about traversal.
void PrintInorder(BST T);
void PrintPreorder(BST T);
void PrintPostorder(BST T);

FILE *output;

void main() {
    FILE *input;
    char func_sel;
    EleType ele_temp;
    Node T = NULL; // Root of tree. At first set it NULL.
    
    // Open files.
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");

    while(1) {
        fscanf(input, " %c", &func_sel);
        if(feof(input)) { // Break at the end of file.
            break;
        }

        switch(func_sel) {
            case 'i' : // Insert.
                fscanf(input, "%d", &ele_temp);
                T = InsertNode(ele_temp, T);
                break;

            case 'd' : // Delete.
                fscanf(input, "%d", &ele_temp);
                T = DeleteNode(ele_temp, T);
                break;

            case 'f' : // Find.
                fscanf(input, "%d", &ele_temp);
                FindNode(ele_temp, T);
                break;

            case 'p' : // Print traversal.
                fscanf(input, "%c", &func_sel);
                
                switch(func_sel) {
                    case 'i' : // Inorder.
                        fprintf(output, "pi -");
                        PrintInorder(T);
                        fprintf(output, "\n");
                        break;

                    case 'r' : // Preorder.
                        fprintf(output, "pr -");
                        PrintPreorder(T);
                        fprintf(output, "\n");
                        break;

                    case 'o' : // Postorder.
                        fprintf(output, "po -");
                        PrintPostorder(T);
                        fprintf(output, "\n");
                        break;

                    default : // Invalid Input. Read to end of line.
                        fprintf(output, "Invalid Input.\n");
                        while(1) {
                            char temp;
                            fscanf(input, "%c", &temp);
                            if(temp == '\n') {
                                break;
                            }
                        }
                        break;
                }
                break;

            default : // Invalid Input. Read to end of line.
                fprintf(output, "Invalid Input.\n");
                while(1) {
                    char temp;
                    fscanf(input, "%c", &temp);
                    if(temp == '\n') {
                        break;
                    }
                }
                break;
        }
    }

    // Free.
    DeleteTree(T);

    // Close files.
    fclose(input);
    fclose(output);
}

// Input : Element X (data), Binary Search Tree T.
// Insert a node with element X to BST T.
BST InsertNode(EleType X, BST T) {
    if(T == NULL) { // Insert the Node at this spot. It will be leaf node.
        T = (Node)malloc(sizeof(struct TreeNode));
        if(T == NULL) {
            fprintf(output, "Failed to allocate. Out of space.\n");
            return NULL;
        }
        else {
            T->Ele = X;
            T->Left = T->Right = NULL;
        }
    }
    else if(X < T->Ele) {
        T->Left = InsertNode(X, T->Left);
    }
    else if(X > T->Ele) {
        T->Right = InsertNode(X, T->Right);
    }
    else { // X is in the tree already.
        fprintf(output, "%d already exists.\n", X);
    }
    
    return T;
}


// Input : Element X (data), Binary Search Tree T.
// Delete a node with element X from BST T.
BST DeleteNode(EleType X, BST T) {
    Node TmpNode;

    if(T == NULL) {
        fprintf(output, "Deletion failed. %d does not exist.\n", X);
    }
    else if(X < T->Ele) { // Go left.
        T->Left = DeleteNode(X, T->Left);
    }
    else if(X > T->Ele) { // Go right.
        T->Right = DeleteNode(X, T->Right);
    }
    else if(T->Left && T->Right) { // The node to be deleted, has 2 childs.
        TmpNode = FindMax(T->Left);
        T->Ele = TmpNode->Ele;
        T->Left = DeleteNode(TmpNode->Ele, T->Left);
    }
    else { // The node to be deleted, has 1 or 0 child.
        TmpNode = T;
        if(T->Left == NULL) {
            T = T->Right;
        }
        else if(T->Right == NULL) {
            T = T->Left;
        }
        free(TmpNode);
    }

    return T;
}

// Input : Element X (data), Binary Search Tree T.
// Find a node with element X from BST T.
Node FindNode(EleType X, BST T) {
    if(T == NULL) { // There're not the node that has Element X.
        fprintf(output, "%d is not in the tree.\n", X);
        return NULL;
    }
    if(X < T->Ele) {
        return FindNode(X, T->Left);
    }
    if(X > T->Ele) {
        return FindNode(X, T->Right);
    }

    // else(X == T->Ele)
    fprintf(output, "%d is in the tree.\n", X);
    return T;
}

// Input : Binary Search Tree T.
// Find the node that has maximum element value in the BST T.
Node FindMax(BST T) {
    if(T == NULL) { // Given tree is invalid.
        return NULL;
    }
    while(T->Right != NULL) {
        T = T->Right;
    }
    return T;
}


// Input : Binary Search Tree T.
// Delete(deallocate) the nodes of the BST T.
void DeleteTree(BST T) { // Delete with postorder traversal.
    if(T) {
        DeleteTree(T->Left);
        DeleteTree(T->Right);
        free(T);
    }
}

// Input : Binary Search Tree T.
// Implement and print inorder traversal.
void PrintInorder(BST T) {
    if(T) {
        PrintInorder(T->Left);
        fprintf(output, " %d", T->Ele);
        PrintInorder(T->Right);
    }
}

// Input : Binary Search Tree T.
// Implement and print preorder traversal.
void PrintPreorder(BST T) {
    if(T) {
        fprintf(output, " %d", T->Ele);
        PrintPreorder(T->Left);
        PrintPreorder(T->Right);
    }
}

// Input : Binary Search Tree T.
// Implement and print postorder traversal.
void PrintPostorder(BST T) {
    if(T) {
        PrintPostorder(T->Left);
        PrintPostorder(T->Right);
        fprintf(output, " %d", T->Ele);
    }
}
