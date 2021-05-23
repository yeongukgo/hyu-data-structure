#include <stdio.h>
#include <stdlib.h>

#define order 3

struct B_node_struct;
typedef struct B_node_struct *B_node;
typedef B_node B_tree;

struct B_node_struct {
    int n_keys; // Number of keys.
    B_node child[order];
    int key[order-1];
};

// Functions.
B_node InitBNode();
B_tree Insert(B_tree bt, int key);
void Inorder(B_tree bt);
void DeleteBTree(B_tree bt);

FILE *output;

void main() {
    FILE *input;
    char func_sel;
    int temp_in;
    B_tree bt = InitBNode(); // Root node of B_tree.

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
                fscanf(input, " %d", &temp_in);
                bt = Insert(bt, temp_in);
                break;

            case 'p' : // Print inorder traversal.
                Inorder(bt);
                fprintf(output, "\n");
                break;

            default : // Invalid Input. Read to end of line.
                fprintf(output, "Invalid Input.\n");
                while(1) {
                    char temp_ch;
                    fscanf(input, "%c", &temp_ch);
                    if(temp_ch == '\n') {
                        break;
                    }
                }
        }
    }

    // Free.
    DeleteBTree(bt);

    // Close files.
    fclose(input);
    fclose(output);
}

// Make and initialize a B_node. return it.
B_node InitBNode() {
    B_node bn = (B_node)malloc(sizeof(struct B_node_struct));
    bn->n_keys = 0;
    
    int i;
    for(i = 0; i < order; i++) {
        bn->child[i] = NULL;
    }

    return bn;
}

// Input : A B_tree, a key.
// Insert given key into given B_tree.
B_tree Insert(B_tree bt, int key_in) {
    int i;
    int temp1, temp2;
    B_node temp_node1, temp_node2;
    
    // Variables for temp tree. Copy given tree.
    // B_tree that has order+1 as order, because it must be able to memorize overflowed node.
    int n_keys = bt->n_keys;
    B_node child[order+1];
    int key[order];
    for(i = 0; i < order; i++) { child[i] = bt->child[i]; }
    child[order] = NULL;
    for(i = 0; i < order-1; i++) { key[i] = bt->key[i]; }
    
    // Insert.
    if(child[0] == NULL) { // This is leaf node. Insert here.
        if(n_keys == 0) { // Empty root. First Insert.
            key[0] = key_in;
        }
        else { // This node that has 1~order-1 keys. With below insertion, overflow can occur.
            // Find the position to insert key, and insert.
            for(i = 0; i < n_keys; i++) {
                if(key_in <= key[i]) { // If both are same, insert key to left side of key[i].
                    temp1 = key[i];
                    break;
                }
            }
            key[i] = key_in;
            
            // Modify posterior keys to sort.
            for(i++; i <= n_keys; i++) {
                temp2 = key[i];
                key[i] = temp1;
                temp1 = temp2;
            }
        }

        n_keys++;
    }
    else { // This has childs. Insert key to appropriate child.
        // Find the child to insert key, and insert.
        for(i = 0; i < n_keys; i++) {
            if(key_in <= key[i]) { // If both are same, insert key to left side of key[i].
                break;
            }
        }
        temp1 = child[i]->n_keys;
        child[i] = Insert(child[i], key_in);

        if(child[i]->n_keys < temp1) { // If n_keys of child[i] is decreased, it was splited. Insert it between keys of this node. Overflow can occur with this insertion.
            temp1 = child[i]->key[0]; // Memorize key of child[i].
            temp_node1 = child[i]->child[1]; // Memorize right child of chlid[i].
            
            // Replace chlid[i] with Left child of child[i].
            temp_node2 = child[i];
            child[i] = child[i]->child[0];
            free(temp_node2);
            
            // Modify posterior keys and childs to sort.
            for(; i <= n_keys; i++) {
                temp2 = key[i];
                key[i] = temp1;
                temp1 = temp2;

                temp_node2 = child[i+1];
                child[i+1] = temp_node1;
                temp_node1 = temp_node2;
            }
            
            n_keys++;
        }
    }

    // Make a new tree from temp tree. If it is overflowed, split it.
    B_node BB = InitBNode();
    if(n_keys == order) { // Overflow.
        int center = (order+1)/2-1; // If order=3, center is 1 of {0,1,2}, if order=4, center is 1 of {0,1,2,3}.
        
        BB->n_keys = 1;
        BB->key[0] = key[center];

        // Left child.
        BB->child[0] = InitBNode();
        for(i = 0; i < center; i++) { BB->child[0]->key[i] = key[i]; }
        for(i = 0; i <= center; i++) { BB->child[0]->child[i] = child[i]; }
        BB->child[0]->n_keys = center;

        // Right child.
        BB->child[1] = InitBNode();
        for(i = 0; i < order-(center+1); i++) { BB->child[1]->key[i] = key[center+1+i]; }
        for(i = 0; i <= order-(center+1); i++) { BB->child[1]->child[i] = child[center+1+i]; }
        BB->child[1]->n_keys = order-(center+1);
    }
    else { // Just copy temp tree to bt.
        BB->n_keys = n_keys;
        for(i = 0; i < n_keys; i++) { BB->key[i] = key[i]; }
        for(i = 0; i <= n_keys; i++) { BB->child[i] = child[i]; }
    }
    free(bt);

    return BB;
}

// Input : A B_tree.
// Print inorder traversal of given B_tree.
void Inorder(B_tree bt) {
    int i;
    if(bt != NULL) {
        for(i = 0; i < bt->n_keys; i++) {
            Inorder(bt->child[i]);
            fprintf(output, "%d ", bt->key[i]);
        }
        Inorder(bt->child[bt->n_keys]);
    }
}

// Input : A B_tree.
// Delete(deallocate) the nodes of given B_tree.
void DeleteBTree(B_tree bt) {
    int i;
    if(bt != NULL) {
        for(i = 0; i <= bt->n_keys; i++) {
            DeleteBTree(bt->child[i]);
        }
        free(bt);
    }
}
