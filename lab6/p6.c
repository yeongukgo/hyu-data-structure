#include <stdio.h>
#include <stdlib.h>

struct TreeNode;
typedef struct TreeNode *AVLNode; // Node of AVL Tree.
typedef struct TreeNode *AVLTree; // Root Node of AVL Tree.

typedef int EleType; // In this program, I will use int as Element Type of AVL Tree.

struct TreeNode {
    EleType Ele; // Element.
    AVLTree Left; // Left child.
    AVLTree Right; // Right child.
    int Height; // Height of the Node.
};

// Functions.
int Height(AVLNode P);
AVLNode SingleRotateL(AVLNode K2);
AVLNode SingleRotateR(AVLNode K1);
AVLNode DoubleRotateL(AVLNode K3);
AVLNode DoubleRotateR(AVLNode K1);
AVLTree Insert(EleType X, AVLTree T);
AVLNode Find(EleType X, AVLTree T);
int Max(int a, int b);
void PrintInorder(AVLTree T);
void DeleteTree(AVLTree T);

FILE *output;

void main() {
    FILE *input;
    EleType ele_temp;
    AVLNode T = NULL; // Root of tree. At first set it NULL.

    // Open files.
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");

    while(1) {
        if(feof(input)) { // Break at the end of file.
            break;
        }

        fscanf(input, " %d", &ele_temp);
        
        if(Find(ele_temp, T) == NULL) { // If input element isn't in T,
            // Insert.
            T = Insert(ele_temp, T);
        
            // Print inorder traversal.
            PrintInorder(T);
            fprintf(output, "\n");
        }
        else { // If input element is in T already,
            fprintf(output, "%d is already in the tree!\n", ele_temp);
        }
    }

    // Free.
    DeleteTree(T);

    // Close files.
    fclose(input);
    fclose(output);
}

// Input : An AVL Node.
// Return the height of the node. If there isn't node, return -1.
int Height(AVLNode P) {
    if(P == NULL) {
        return -1;
    }
    else {
        return P->Height;
    }
}

// Input : An AVL Node K2.
// K1 is the left child of K2.
// K1 < K2.
// Execute Single Rotation, because of an insertion into the left subtree of the left child.
AVLNode SingleRotateL(AVLNode K2) { // LL
    // Rotate.
    AVLNode K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    // Set Heights.
    K2->Height = Max(Height(K2->Left), Height(K2->Right)) + 1;
    K1->Height = Max(Height(K1->Left), K2->Height) + 1;

    return K1; // New root.
}

// Input : An AVL Node K1.
// K2 is the right child of K1.
// K1 < K2.
// Execute Single Rotation, because of an insertion into the right subtree of the right child.
AVLNode SingleRotateR(AVLNode K1) { // RR
    // Rotate.
    AVLNode K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;

    // Set Heights.
    K1->Height = Max(Height(K1->Left), Height(K1->Right)) + 1;
    K2->Height = Max(K1->Height, Height(K2->Right)) + 1;

    return K2; // New root.
}

// Input : An AVL Node K3.
// K1 is the left child of K3.
// K2 is the right child of K1.
// K1 < K2 < K3.
// Excute Double Rotation, because of an insertion into the right subtree of the left child.
AVLNode DoubleRotateL(AVLNode K3) { // LR
    // Rotate between K1 & K2.
    K3->Left = SingleRotateR(K3->Left); // The root will be K2.

    // Rotate between K3 & K2.
    return SingleRotateL(K3); // The root will be K2.
}

// Input : An AVL Node K1.
// K3 is the right child of K1.
// K2 is the left child of K3.
// K1 < K2 < K3.
// Excute Double Rotation, because of an insertion into the left subtree of the right child.
AVLNode DoubleRotateR(AVLNode K1) { // RL
    // Rotate between K3 & K2.
    K1->Right = SingleRotateL(K1->Right); // The root will be K2.

    // Rotate between K1 & K2.
    return SingleRotateR(K1); // The root will be K2.
}

// Input : Element X (data), AVL Tree T.
// Insert a node with element X to AVL Tree T.
AVLTree Insert(EleType X, AVLTree T) {
    if(T == NULL) { // Insert the Node at this spot.
        T = (AVLTree)malloc(sizeof(struct TreeNode));
        if(T == NULL) {
            fprintf(output, "Failed to allocate. Out of space.\n");
            return NULL; // If don't return NULL at here, Error will occur at accessing Height and childs of T.
        }
        else {
            T->Ele = X;
            T->Height = 0; // It will be leaf node.
            T->Left = T->Right = NULL;
        }
    }
    else if(X < T->Ele) { // Go left.
        T->Left = Insert(X, T->Left); // BST.
        if(Height(T->Left) - Height(T->Right) == 2) { // If height-balance break,
            if(X < T->Left->Ele) { // If the Node with X is inserted into the left subtree of the left child,
                T = SingleRotateL(T);
            }
            else { // If the Node with X is inserted into the right subtree of the left child,
                T = DoubleRotateL(T);
            }
        }
    }
    else if(X > T->Ele) { // Go right.
        T->Right = Insert(X, T->Right); // BST.
        if(Height(T->Right) - Height(T->Left) == 2) { // If height-balance break;
            if(X > T->Right->Ele) { // If the Node with X is inserted into the right subtree of the right child,
                T = SingleRotateR(T);
            }
            else { // If the Node with X is inserted into the left subtree of the right child,
                T = DoubleRotateR(T);
            }
        }
    }
    else { // X is in the tree already.
        // In main function, program check about this. But for unpredictable situation, I made this.
        fprintf(output, "%d is already in the tree!\n", X);
    }
    
    // Set Height.
    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
    
    return T;
}

// Input : Element X (data), AVL Tree T.
// Find a node with element X form AVL Tree T.
AVLNode Find(EleType X, AVLTree T) {
    if(T == NULL) { // There're not the node that has Element X.
        return NULL;
    }
    if(X < T->Ele) {
        return Find(X, T->Left);
    }
    if(X > T->Ele) {
        return Find(X, T->Right);
    }
    
    // else(X == T->Ele)
    // Find Success.
    return T;
}

// Input : int variables.
// Return the bigger one of them.
int Max(int a, int b) {
    if(a >= b) {
        return a;
    }
    return b;
}

// Input : AVL Tree T.
// Implement and print Inorder traversal.
void PrintInorder(AVLTree T) {
    if(T) {
        PrintInorder(T->Left);
        fprintf(output, "%d(%d) ", T->Ele, T->Height);
        PrintInorder(T->Right);
    }
}

// Input : AVL Tree T.
// Delete(deallocate) the nodes of the AVL Tree T.
void DeleteTree(AVLTree T) { // Delete with postorder traversal.
    if(T) {
        DeleteTree(T->Left);
        DeleteTree(T->Right);
        free(T);
    }
}
