#include <stdio.h>
#include <stdlib.h>

#define INF 99999
#define NIL -99999

typedef struct {
    int key;
    int weight; // Distance from start node.
} Node;

typedef struct {
    int size;
    Node* node;

    // Same to weight of each nodes. For easy using.
    // Not two-dimensional array. Array of int pointers.
    int** d;
    
    int* pred; // Index of previous node. NOT key.
    int** w;
} Graph;

typedef struct {
    int capacity;
    int size;
    Node** element; // Not two-dimensional array. Array of Node pointers.
} Heap;

// Functions for graph.
Graph* CreateGraph(int* key, int size);
void InsertEdge(Graph* G, int a, int b, int w);
void DeleteGraph(Graph* G);
int IndexOfKey(Graph* G, int key);

// Functions about dijkstra algorithm.
void Dijkstra(Graph* G, int start_key);
void PrintShortestPath(Graph* G, int end_index);

// Functions for heap.
Heap* CreateMinHeap(int capacity);
void Insert(Heap* H, Node* x);
int DeleteMin(Heap* H);
void DecreasePriority(Heap* H, int v);
void DeleteHeap(Heap* H);

FILE* output;

int main(void) {
    FILE* input;
    Graph* G;
    int* key;
    int num = 0;
    int a, b, c;
    char ch;

    // Open files.
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");

    // Check number of nodes.
    while(1) {
        fscanf(input, "%d", &a);
        fscanf(input, "%c", &ch);
        if(ch == ' ' || ch == '\n' || ch == '\r') {
            num++;
        }
        else if(ch == '-') {
            break;
        }
    }
    fclose(input);
    input = fopen("input.txt", "r");

    // Read keys of nodes.
    key = (int*)malloc(sizeof(int)*num);
    for(a = 0; a < num; a++) {
        fscanf(input, " %d", &key[a]);
    }

    // Create Graph.
    G = CreateGraph(key, num);
    free(key);
    
    // Read edges.
    while(1) {
        fscanf(input, "%d-%d-%d", &a, &b, &c);
        fscanf(input, "%c", &ch);
        InsertEdge(G, a, b, c);
        if(ch == '\n' || ch == '\r') {
            break;
        }
    }
    
    // Read start/end nodes, and print shortest path.
    fscanf(input, "%d %d", &a, &b);
    Dijkstra(G, a);
    PrintShortestPath(G, IndexOfKey(G, b));
    fprintf(output, "\n");

    // Free.
    DeleteGraph(G);

    // Close files.
    fclose(input);
    fclose(output);

    return 0;
}

// Input : An array of keys of nodes, size of the array.
// Create and initialize a graph with given nodes.
Graph* CreateGraph(int* key, int size) {
    int i, j;

    Graph* G = (Graph*)malloc(sizeof(Graph));
    G->size = size;
    
    G->node = (Node*)malloc(sizeof(Node)*size);
    G->d = (int**)malloc(sizeof(int*)*size);
    G->pred = (int*)malloc(sizeof(int)*size);
    G->w = (int**)malloc(sizeof(int*)*size);
    for(i = 0; i < size; i++) {
        G->node[i].key = key[i];
        G->node[i].weight = INF;
        G->d[i] = &G->node[i].weight; // For easy using.
        G->pred[i] = NIL;
        G->w[i] = (int*)malloc(sizeof(int)*size);
        for(j = 0; j < size; j++) {
            G->w[i][j] = INF;
        }
    }

    return G;
}

// Input : A graph, both keys of nodes, a weight.
// Insert an edge that has given weight between given nodes in given graph.
void InsertEdge(Graph* G, int a, int b, int w) {
    int index_a = IndexOfKey(G, a);
    int index_b = IndexOfKey(G, b);

    if(index_a == -1 || index_b == -1) {
        fprintf(output, "invalid edge : (%d->%d, w : %d)\n", a, b, w);
    }
    else {
        G->w[index_a][index_b] = w;
    }
}

// Input : A graph.
// Delete(deallocate) given graph.
void DeleteGraph(Graph* G) {
    int i;

    free(G->node);
    free(G->d);
    free(G->pred);
    
    for(i = i; i < G->size; i++) {
        free(G->w[i]);
    }
    free(G->w);

    free(G);
}

// Input : A graph, a key.
// Return index of given key in given graph.
int IndexOfKey(Graph* G, int key) {
    int i;

    for(i = 0; i < G->size; i++) {
        if(G->node[i].key == key) {
            return i;
        }
    }

    return -1;
}

// Input : A graph, the start node.
// Execute dijkstra's algorithm with given start node.
void Dijkstra(Graph* G, int start_key) {
    Heap* H = CreateMinHeap(G->size);
    int v, u;
    int s = IndexOfKey(G, start_key);
    
    // For easy using.
    int size = G->size;
    int** d = G->d;
    int* pred = G->pred;
    int** w = G->w;
    
    // Distance of start node from start node is 0.
    *d[s] = 0;
    
    // Check adjacent nodes of start node.
    for(v = 0; v < size; v++) {
        if(w[s][v] != INF) {
            *d[v] = w[s][v];
            pred[v] = s;
        }
    }

    // Add each nodes to min heap.
    for(v = 0; v < size; v++) {
        Insert(H, &G->node[v]);
    }
    
    // Repeat to DeleteMin.
    while(H->size != 0) {
        u = IndexOfKey(G, DeleteMin(H));        

        for(v = 0; v < size; v++) {
            // If v is adjacent to u, and
            // path from u is shorter than original shortest path,
            if(w[u][v] != INF && *d[u] + w[u][v] < *d[v]) {
                // Change the shortest path.
                *d[v] = *d[u] + w[u][v];
                pred[v] = u; // "Index" of previous node. NOT key.

                DecreasePriority(H, G->node[v].key);
            }
        }
    }

    // Free.
    DeleteHeap(H);
}

// Input : A graph, the end node.
// Print shortest path to given end node.
void PrintShortestPath(Graph* G, int end_index) {
    if(*G->d[end_index] == INF) {
        fprintf(output, "no path");
        return;
    }
    if(G->pred[end_index] != NIL) {
        PrintShortestPath(G, G->pred[end_index]);
    }
    fprintf(output, "%d ", G->node[end_index].key);
}

// Input : Capacity of heap.
// Create a heap with given capapcity and return it.
Heap* CreateMinHeap(int capacity) {
    Heap* H = (Heap*)malloc(sizeof(Heap));
    
    H->capacity = capacity;
    H->size = 0;
    H->element = (Node**)malloc(sizeof(Node*)*(capacity+1));
}

// Input : A heap, an element to insert.
// Insert the element into the heap.
void Insert(Heap* H, Node *x) {
    int i;

    if(H->capacity == H->size) {
        fprintf(output, "Heap is full. Fail to insert %d.\n", x->key);
        return;
    }

    // Percolating-up.
    for(i = ++H->size; i > 1 && H->element[i/2]->weight > x->weight; i /= 2) {
        H->element[i] = H->element[i/2];
    }
    H->element[i] = x;
}

// Input : A heap.
// Delete minimum element from root and return the key of the element.
int DeleteMin(Heap* H) {
    int i, child;
    Node* min = H->element[1];
    Node* last = H->element[H->size--];

    // Percolating-down.
    for(i = 1; i*2 <= H->size; i = child) {
        child = i*2;

        // Choose the smaller one between left child & right child.
        if(child != H->size && H->element[child+1]->weight < H->element[child]->weight) {
            child++;
        }

        // Choose the smaller one between last & child.
        if(last->weight > H->element[child]->weight) {
            H->element[i] = H->element[child];
        }
        else {
            break;
        }
    }
    H->element[i] = last;
    
    return min->key;
}

// Input : A tangled heap, a key of node that need percolating-up.
// Perform percolating-up with given node in given heap.
void DecreasePriority(Heap* H, int v) {
    int i;
    Node* tmp;

    // Find node by key.
    for(i = 1; i <= H->size; i++) {
        if(H->element[i]->key == v) {
            v = i;
            break;
        }
    }

    // Percolating-up.
    for(i = v; i > 1 && H->element[i/2]->weight > H->element[i]->weight; i /= 2) {
        tmp = H->element[i];
        H->element[i] = H->element[i/2];
        H->element[i/2] = tmp;
    }
}

// Input : A heap.
// Delete(deallocate) given heap.
void DeleteHeap(Heap* H) {
    free(H->element);
    free(H);
}
