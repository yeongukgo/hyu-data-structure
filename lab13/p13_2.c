#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEFT 1
#define RIGHT 2
#define MIDDLE 3

// Functions.
void QuickSort(int* arr, int left, int right, int pivot);
int Partition(int* arr, int left, int right, int pivot);
void Swap(int* a, int* b);

FILE* output;

int main(void) {
    FILE* input;
    int pivot, num;
    int i;
    int* arr;
    char str[10];

    // Open files.
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");
    
    while(1) {
        fscanf(input, " %s", str); // Read pivot.
        if(feof(input)) { break; } // Break at the end of file.

        // Set pivot.
        if(strcmp(str, "leftmost") == 0) { pivot = LEFT; }
        else if(strcmp(str, "rightmost") == 0) { pivot = RIGHT; }
        else if(strcmp(str, "middle") == 0) { pivot = MIDDLE; }
        else { printf("Invalid pivot.\n");  break; }
        printf("%s:\n", str);
        fprintf(output, "%s\n", str);

        // Read number of values.
        fscanf(input, "%d", &num);
        arr = (int*)malloc(sizeof(int)*num);

        // Read values.
        for(i = 0; i < num; i++) {
            fscanf(input, "%d", &arr[i]);
        }

        QuickSort(arr, 0, num-1, pivot);        
        
        printf("\nresult\n");
        for(i = 0; i < num; i++) {
            printf("%d ", arr[i]);
            fprintf(output, "%d ", arr[i]);
        }
        printf("\n\n");
        fprintf(output, "\n\n");
        
        // Free.
        free(arr);
    }

    // Close files.
    fclose(input);
    fclose(output);

    return 0;
}

// Input : An array, left and right of the range to sort, pivot option.
// Excute quick sort with given values.
void QuickSort(int* arr, int left, int right, int pivot) {
    if(left >= right) { return; }
    int p = Partition(arr, left, right, pivot);

    int i;
    printf("<");
    for(i = left; i <= p-1; i++) {
        printf("%d ", arr[i]);
    }
    printf("><%d ><", arr[p]);
    for(i = p+1; i <= right; i++) {
        printf("%d ", arr[i]);
    }
    printf(">\n");

    QuickSort(arr, left, p-1, pivot);
    QuickSort(arr, p+1, right, pivot);
}

// Input : An array, left and right of the range to sort, pivot option.
// Set pivot and partitions.
int Partition(int* arr, int left, int right, int pivot) {
    int p, i, j, temp;
    if(pivot == LEFT) { p = left; }
    else if(pivot == RIGHT) { p = right; }
    else if(pivot == MIDDLE) { p = (left + right) / 2; }
    Swap(&arr[p], &arr[right]);
    p = arr[right];
    i = left - 1;
    j = right;

    while(1) {
        while(arr[--j] > p);
        while(arr[++i] < p);
        if(i < j) {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
        else {
            temp = arr[i];
            arr[i] = arr[right];
            arr[right] = temp;
            return i;
        }
    }
}

// Input : 2 integers.
// Swap given integers.
void Swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
