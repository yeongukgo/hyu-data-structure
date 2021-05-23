#include <stdio.h>
#include <stdlib.h>

struct _Graph {
    int size;
    int* node;
    int** matrix;
};

struct _Queue {
    int* key;
    int front;
    int rear;
    int size;
    int capacity;
};

typedef struct _Graph* Graph;
typedef struct _Queue* Queue;

// Main functions for graph.
Graph CreateGraph(int* nodes, int size);
void InsertEdge(Graph G, int a, int b);
void PrintAdjacencyMatrix(Graph G);
void TopSort(Graph G);
void DeleteGraph(Graph G);

// Supporting functions for graph.
int InDegree(Graph G, int index);
int isAdjacent(Graph G, int b, int a);
int IndexOfKey(Graph G, int key);
int* SortIndexByKey(Graph G);

// Functions for queue.
Queue CreateQueue(int capacity);
void Enqueue(Queue Q, int x);
int Dequeue(Queue Q);
void DeleteQueue(Queue Q);

FILE* output;

int main() {
    FILE* input;
    Graph G;
    int* nodes;
    int size = 0;
    int temp_i;
    char temp_c;
    int i, a, b;
    
    // Open files.
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");
    
    // Check number of nodes.
    while(1) {
        fscanf(input, "%d", &temp_i);
        fscanf(input, "%c", &temp_c);
        if(temp_c == ' ' || temp_c == '\n' || temp_c == '\r') {
            size++;
        }
        else if(temp_c == '-') {
            break;
        }
    }
    fclose(input);
    input = fopen("input.txt", "r");

    // Read nodes.
    nodes = (int*)malloc(sizeof(int)*size);
    for(i = 0; i < size; i++) {
        fscanf(input, " %d", &nodes[i]);
    }

    // Create Graph.
    G = CreateGraph(nodes, size);
    free(nodes);

    // Read edges.
    while(1) {
        fscanf(input, "%d-%d", &a, &b);
        if(feof(input)) {
            break;
        }
        InsertEdge(G, a, b);
    }

    // Print adjacency matrix.
    PrintAdjacencyMatrix(G);
    fprintf(output, "\n\n");
    
    // Print graph by topological sort.
    TopSort(G);

    // Free.
    DeleteGraph(G);

    // Clse files.
    fclose(input);
    fclose(output);

    return 0;
}

// Input : An array of keys of nodes, size of the array.
// Creat and initialize a graph with given nodes.
Graph CreateGraph(int* nodes, int size) {
    int i;
    
    Graph G = (Graph)malloc(sizeof(struct _Graph));
    G->size = size;
    
    G->node = (int*)malloc(sizeof(int)*size);
    for(i = 0; i < size; i++) {
        G->node[i] = nodes[i];
    }
    
    G->matrix = (int**)malloc(sizeof(int*)*size);
    for(i = 0; i < size; i++) {
        G->matrix[i] = (int*)calloc(size, sizeof(int));
    }

    return G;
}

// Input : A graph, both keys of nodes.
// Insert an edge between given nodes in given graph.
void InsertEdge(Graph G, int a, int b) {
    int index_a = IndexOfKey(G, a);
    int index_b = IndexOfKey(G, b);

    if(index_a == -1 || index_b == -1) {
        fprintf(output, "Invalid edge : (%d, %d)\n", a, b);
        return;
    }

    G->matrix[index_a][index_b] = 1;
}

// Input : A graph.
// Print adjacency matrix of given graph.
void PrintAdjacencyMatrix(Graph G) {
    int i, j;

    fprintf(output, "Adjacency matrix\n  ");

    for(i = 0; i < G->size; i++) {
        fprintf(output, "%d ", G->node[i]);
    }
    fprintf(output, "\n");

    for(i = 0; i < G->size; i++) {
        fprintf(output, "%d ", G->node[i]);
        for(j = 0; j < G->size; j++) {
            fprintf(output, "%d ", G->matrix[i][j]);
        }
        fprintf(output, "\n");
    }
}

// Input : A graph.
// Print the graph with topological sort.
void TopSort(Graph G) {
    Queue Q = CreateQueue(G->size);
    int i, j, min, temp, index = -1;
    int* sortedIndex = SortIndexByKey(G);
    
    // Check in-degrees.
    int* inDegree = (int*)malloc(sizeof(int)*G->size);
    for(i = 0; i < G->size; i++) {
        inDegree[i] = InDegree(G, i);
    }

    fprintf(output, "TopSort Result : ");
    
    // Enqueue the nodes has 0 in-degree at first.
    for(i = 0; i < G->size; i++) {
        if(inDegree[sortedIndex[i]] == 0) {
            Enqueue(Q, G->node[sortedIndex[i]]);
        }
    }

    // Repeat to dequeue and enqueue.
    while(Q->size != 0) {
        temp = Dequeue(Q);
        fprintf(output, "%d ", temp);

        for(i = 0; i < G->size; i++) {
            // If the node of sortedIndex[i] has edge from IndexOfKey(temp)
            // and its in-degree is 0 after deleting the edge,
            if(isAdjacent(G, sortedIndex[i], IndexOfKey(G, temp)) == 1 &&
                    --inDegree[sortedIndex[i]] == 0) {
                Enqueue(Q, G->node[sortedIndex[i]]);
            }
        }
    }

    fprintf(output, "\n");

    // Free.
    DeleteQueue(Q);
    free(sortedIndex);
    free(inDegree);
}

// Input : A graph.
// Delete(deallocate) given graph.
void DeleteGraph(Graph G) {
    int i;

    free(G->node);
    
    for(i = 0; i < G->size; i++) {
        free(G->matrix[i]);
    }
    free(G->matrix);

    free(G);
}

// Input : A graph, an index.
// Return in-degree of the node of given index in given graph.
int InDegree(Graph G, int index) {
    int i, inDegree = 0;
    
    for(i = 0; i < G->size; i++) {
        inDegree += G->matrix[i][index];
    }

    return inDegree;
}

// Input : A graph, both indexes of nodes.
// Return 1 if the node b has edge from node a. Else return 0.
int isAdjacent(Graph G, int b, int a) {
    if(G->matrix[a][b] == 1) {
        return 1;
    }
    return 0;
}

// Input : A graph, a key.
// Return index of given key in given graph.
int IndexOfKey(Graph G, int key) {
    int i;
    
    for(i = 0; i < G->size; i++) {
        if(G->node[i] == key) {
            return i;
        }
    }

    return -1;
}

// Input A graph.
// Return sorted indexes of nodes of given graph in ascending order by key. 
int* SortIndexByKey(Graph G) {
    int i, j, min, temp;
    int* sortedIndex = (int*)malloc(sizeof(int)*G->size);

    // Initialize array.
    for(i = 0; i < G->size; i++) {
        sortedIndex[i] = i;
    }

    // Sort.
    for(i = 0; i < G->size - 1; i++) {
        min = i;
        for(j = i+1; j < G->size; j++) {
            if(G->node[sortedIndex[j]] < G->node[sortedIndex[min]]) {
                min = j;
            }
        }
        temp = sortedIndex[i];
        sortedIndex[i] = sortedIndex[min];
        sortedIndex[min] = temp;
    }

    return sortedIndex;
}

// Input : Capacity of queue to create.
// Create a queue with given capacity.
Queue CreateQueue(int capacity) {
    Queue Q = (Queue)malloc(sizeof(struct _Queue));
    Q->front = 1;
    Q->rear = 0;
    Q->size = 0;
    Q->capacity = capacity;

    Q->key = (int*)malloc(sizeof(int)*capacity);
}


// Input : A queue, an element.
// Enqueue(insert) given element into given queue.
void Enqueue(Queue Q, int x) {
    if(Q->size == Q->capacity) { // If queue is full,
        fprintf(output, "\nFail to enqueue %d. Queue is full.\n", x);
    }
    else {
        Q->size++;
        Q->rear = (Q->rear + 1) % Q->capacity;
        Q->key[Q->rear] = x;
    }
}

// Input : A queue.
// Dequeue(return and delete) the front element in the queue.
int Dequeue(Queue Q) {
    int temp;

    if(Q->size == 0) { // If queue is empty,
        fprintf(output, "\nFail to dequeue. Queue is empty.\n");
        return 0;
    }

    Q->size--;
    temp = Q->front;
    Q->front = (Q->front + 1) % Q->capacity;
    return Q->key[temp];
}

// Input : A queue.
// Delete(deallocate) given queue.
void DeleteQueue(Queue Q) {
    free(Q->key);
    free(Q);
}
