#include <stdio.h>
#include <stdlib.h>

typedef struct threaded_tree *threaded_ptr;
struct threaded_tree {
    short int left_thread;
    threaded_ptr left_child;
    char data;
    threaded_ptr right_child;
    short int right_thread;
};

// Functions about creating threaded tree.
threaded_ptr CreateNode(char data);
void InsertNode(threaded_ptr thisnode, threaded_ptr parent, int path);
int Level(int index);
threaded_ptr Create_C_T_B_Tree(int num, char *data);
void DeleteTree(threaded_ptr tree);

// Functions about inorder traversal.
threaded_ptr Insucc(threaded_ptr tree);
void Tinorder(threaded_ptr tree);

FILE *output;

void main() {

    FILE *input;
    int num_of_nodes;
    char *data_of_nodes;
    threaded_ptr tree;
    int i; // For for loop.

    // Open files.
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");
    
    // Read and save the number of nodes and the data of nodes.
    fscanf(input, "%d ", &num_of_nodes);
    data_of_nodes = (char*)malloc(sizeof(char)*num_of_nodes);
    for(i = 0; i < num_of_nodes; i++) {
       fscanf(input, "%c ", &data_of_nodes[i]);
    }
        
    // Create (complete) threaded binary tree with given data.
    tree = Create_C_T_B_Tree(num_of_nodes, data_of_nodes);

    // Implement inorder traversal.
    Tinorder(tree);
    
    // Free.
    free(data_of_nodes);
    DeleteTree(tree);

    // Close files.
    fclose(input);
    fclose(output);
}

// Input : Data of node.
// Create a node and initialize it.
threaded_ptr CreateNode(char data) {
    threaded_ptr node = (threaded_ptr)malloc(sizeof(struct threaded_tree));
    node->left_thread = 1; // True. (Because it has no child when it is created.)
    node->left_child = node; // Thread itself.
    node->data = data;
    node->right_child = node; // Thread itself.
    node->right_thread = 1; // True.

    return node;
}

// Input : A node that will be inserted, parent node, the path(left or right).
// Insert the node at given path of the parent node. Set the threads.
void InsertNode(threaded_ptr thisnode, threaded_ptr parent, int path) {
    if(path == 0) { // If path is to left,
        thisnode->left_child = parent->left_child; // The node receives parent's left thread into it's left thread.
        thisnode->right_child = parent; // The node receives parent into it's right thread.
        parent->left_child = thisnode; // The parent receives the node into it's left child.
        parent->left_thread = 0; // False. (Because it has child now.)
    }
    else if(path == 1) { // If path is to right,
        thisnode->left_child = parent; // The node receives parent into it's left thread.
        thisnode->right_child = parent->right_child; // The node receives parent's right thread into it's right thread.
        parent->right_child = thisnode; // the parent receives the node into it's right child.
        parent->right_thread = 0; // False.
    }
}

// Input : The index of node or the last index of tree.
// Return the level of node or the depth of tree.
int Level(int index) {
    int i, level;
    // Repeat to divide index by 2 until result is 1. The number of divisions is the level.
    for(i = index, level = 0; i != 1; i /= 2, level += 1);
    return level;
}

// Input : The number of nodes and the data of them.
// Create a (complete) threaded binary tree with given data.
threaded_ptr Create_C_T_B_Tree(int num, char *data) {
    threaded_ptr header = CreateNode('\0');
    threaded_ptr temp_parent;
    int depth; // Depth of tree.
    int *paths; // This will have the paths from root to a node.

    // They must false to implement inorder traversal. Because of insucc function.
    header->left_thread = 0; // false
    header->right_thread = 0; // false

    // Find depth of the tree and allocate paths with depth.
    depth = Level(num);
    paths = (int*)malloc(sizeof(int)*(depth+1));

    // Repeat inserting nodes.
    int n;
    for(n = 0; n < num; n++) {
        int index = n+1; // Index of the node in tree.
        int level = Level(index);
        int i, l; // For for loop.
        threaded_ptr thisnode = CreateNode(data[n]);

        // Find the path from root to given index, and the node that will be parent node of this node.
        if(index == 1) { // If index = 1, it will be the root of the tree.
            temp_parent = header; // Header will be the parent of this node(root).
            paths[0] = 0; // This node(root) will be left child of the header.
        }
        else {
            temp_parent = header->left_child; // To find paths from root.
            
            // Find the paths from root to given index. 0 is left, 1 is right.
            // Path[x] is the path from level x to level x+1.
            // If level of the node is y, find the path from level y-1 to level y first. So l = level-1.
            for(i = index, l = level-1; l >= 0; paths[l] = i % 2, i /= 2, l--);

            // Find the node that will be parent node of this node.
            for(l = 0; l < level-1; l++) { // The level of parent node is level(of this node)-1.
                if(paths[l] == 0) { // To left child.
                    temp_parent = temp_parent->left_child;
                }
                if(paths[l] == 1) { // To right child.
                    temp_parent = temp_parent->right_child;
                }
            }
        }

        // Insert thisnode at temp_parent.
        InsertNode(thisnode, temp_parent, paths[level-1]);
    }
    
    // Free.
    free(paths);

    return header;
}

// Input : A threaded binary tree.
// Delete A tree with inorder traversal.
void DeleteTree(threaded_ptr tree) {
    threaded_ptr temp;
    threaded_ptr temp2 = Insucc(tree); // Start at the first node in inorder traversal.
    while(1) {
        temp = temp2;
        if(temp == tree) {
            free(temp); // Free the header.
            break;
        }
        temp2 = Insucc(temp);
        free(temp);
    }
}

// Input : A node.
// Find the in-order successor of given node.
threaded_ptr Insucc(threaded_ptr node) {
    threaded_ptr temp;
    temp = node->right_child;
    if(!node->right_thread) {
        while(!temp->left_thread) {
            temp = temp->left_child;
        }
    }
    return temp;
}

// Input : A threaded binary tree.
// Implement inorder traversal with given threaded binary tree. Print the result.
void Tinorder(threaded_ptr tree) {
    threaded_ptr temp = tree;
    while(1) {
        temp = Insucc(temp);
        if(temp == tree) break;
        fprintf(output, "%c ", temp->data);
    }
    fprintf(output, "\n");
}
