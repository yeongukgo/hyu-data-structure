#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int *Sets;
typedef struct MazeStruct *Maze;

struct MazeStruct {
    Sets S; // Disjoint Set of cells.
    Sets E; // Set of edges.
   
    int sizeM; // size of maze;
    int sizeS; // size of S.
    int sizeE; // size of E.
};

// Functions.
void InitMaze(Maze m, int size);
int Find(int x, Sets S);
void Union(Sets, int r1, int r2);
void GenMaze(Maze m);
void PrintMaze(Maze m);
void DeleteMaze(Maze m);

FILE *output;

void main() {
    FILE *input;
    int size;
    Maze m = (Maze)malloc(sizeof(struct MazeStruct));

    // Open files.
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");
    
    // Read input file and initialize the maze.
    fscanf(input, " %d", &size);
    InitMaze(m, size);
    
    // Generate mase.
    GenMaze(m);

    // Print Maze.
    PrintMaze(m);
    
    // Free.
    DeleteMaze(m);

    // Close files.
    fclose(input);
    fclose(output);
}

// Input : An Maze pointer, given size.
// Initialize the maze with given size.
void InitMaze(Maze m, int in_size) {
    int i;
    
    // Size of maze is given size.
    m->sizeM = in_size;

    // Initialize disjoint set of cells.
    m->sizeS = in_size*in_size;
    m->S = (Sets)malloc(sizeof(int)*m->sizeS);
    for(i = 0; i < m->sizeS; i++) {
        // Use -1 for root.
        // Because of using the index starting at 0 instead at 1.
        m->S[i] = -1;
    }

    // Initialize set of edges.
    m->sizeE = in_size*(in_size-1)*2;
    m->E = (Sets)malloc(sizeof(int)*m->sizeE);
    for(i = 0; i < m->sizeE; i++) {
        // 0 means closed edge. (1 means opened edge.)
        // Half of set are vertical edges, and others are horizontal edges.
        // ex) If given size is 4,
        // E[0] is between S[0]&S[1] ~ E[11] is between S[14]&S[15],
        // E[12] is between S[0]&S[4] ~ E[23] is between S[11]&S[15].
        m->E[i] = 0;
    }
}

// Input : Cell x, a set of cells.
// Find the root of x and return it.
int Find(int x, Sets S) {
    if(S[x] <= -1) {
        return x;
    }
    else {
        return(S[x] = Find(S[x], S));
    }
}

// Input : A set of cells, cell r1 & r2.
// Make union with r1 & r2.
void Union(Sets S, int r1, int r2) {
    if(S[r2] < S[r1]) {
        S[r1] = r2;
    }
    else {
        if(S[r2] == S[r1]) {
            S[r1]--;
        }
        S[r2] = r1;
    }
}

// Input : An maze pointer.
// Generate a maze.
void GenMaze(Maze m) {
    srand((unsigned int)time(NULL)); // Generate seed. (for random)
    int e; // Edge.
    int x, y; // Neighbor cells they have joint edge.
    int u, v; // Roots of each cells.
    int cnt = m->sizeS-1;
    
    // Repeat making unions and opening edges until every cells will be connected.
    while(cnt != 0) {
        do {
            e = rand()%m->sizeE; // Pick a random edge.
        } while(m->E[e] != 0); // The edge must be closed. (To open it.)
        
        // Find Neighbor cells they have the joint edge e.
        if(e < m->sizeE/2) { // If e is vertical edge,
            x = e/(m->sizeM-1)*m->sizeM+e%(m->sizeM-1);
            y = x+1;
        }
        else { // If e is horizontal edge,
            x = e-(m->sizeM*(m->sizeM-1));
            y = x+m->sizeM;
        }
        
        // Find the roots of each cells.
        u = Find(x, m->S);
        v = Find(y, m->S);

        if(u != v) { // If they're roots are not same,
            Union(m->S, u, v); // Make union with them.
            cnt--;
            m->E[e] = 1; // Open the edge.
        }
    }
}

// Input : An maze.
// Print the edges of maze.
void PrintMaze(Maze m) {
    int i, j;
    
    // First horizontal walls.
    for(i = 0; i < m->sizeM; i++) {
        fprintf(output, "+-");
    }
    fprintf(output, "+\n");

    // Print vertical/horizontal walls of each lines.
    for(i = 0; i < m->sizeM; i++) {
        // Entrance or First vertical walls of each lines.
        if(i == 0) { fprintf(output, "  "); }
        else { fprintf(output, "| "); }

        // Vertical walls.
        for(j = 0; j < m->sizeM-1; j++) {
            if(m->E[i*(m->sizeM-1)+j] == 1) { // If the edge is opened,
                fprintf(output, "  ");
            }
            else { // If the edge is closed,
                fprintf(output, "| ");
            }
        }

        if(i == m->sizeM-1) { // Exit and Last horizontal walls.
            fprintf(output, " \n");
            for(j = 0; j < m->sizeM; j++) {
                fprintf(output, "+-");
            }
            fprintf(output, "+\n");
            break;
        }
        else { fprintf(output, "|\n"); } // Last vertical walls of each lines.

        // Horizontal walls.
        for(j = 0; j < m->sizeM; j++) {
            fprintf(output, "+");
            if(m->E[m->sizeM*(m->sizeM-1)+i*m->sizeM+j] == 1) { // If the edge is opened,
                fprintf(output, " ");
            }
            else { // If the edge is closed,
                fprintf(output, "-");
            }
        }
        fprintf(output, "+\n");
    }
}

// Input : An maze.
// Delete(deallocate) the maze.
void DeleteMaze(Maze m) {
    free(m->S);
    free(m->E);
    free(m);
}
