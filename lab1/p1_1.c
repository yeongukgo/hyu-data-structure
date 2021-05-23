#include <stdio.h>
#include <stdlib.h>

void sort(int size, int* array);

void main() {
    int size, *array;
    FILE *file;
    
    // read input.txt
    file = fopen("input.txt", "r");
    fscanf(file, "%d", &size);
    array = (int*)malloc(sizeof(int)*size); 
    for(int i = 0; i < size; i++) {
        fscanf(file, "%d", &array[i]);
    }
    fclose(file); 
  
    // sort
    sort(size, array);

    // write output.txt
    file = fopen("output.txt", "w");
    for(int i = 0; i < size; i++) {
        fprintf(file, "%d", array[i]);
        if(i+1 < size) {
            fprintf(file, " ");
        }
    }
    fclose(file);
    
    free(array);
}

void sort(int size, int* array) {
    int temp, min_index;
    for(int i = 0; i < size-1; i++) { // 1st index -> 2nd to last index
        min_index = i;
        for(int j = i+1; j < size; j++) { // From the unsorted integers, find the smallest
            if(array[j] < array[min_index]) {
                min_index = j;
            }
        }
        // place the smallest next to the sorted list
        temp = array[i];
        array[i] = array[min_index];
        array[min_index] = temp;
    }
}
