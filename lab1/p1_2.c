#include <stdio.h>
#include <stdlib.h>

int strSize(char* str);
char* strCopy(char* input);

typedef struct {
    char *name;
    int studentID;
    char *major;
} studentT;

void main() {
    int num, strsize;
    char char_temp1[31], char_temp2[31]; // condition : the maximum lengths of strings are 30 characters; 1 more for '\0'(NULL)
    studentT *stuT;
    FILE *file;
    
    // read student_information.txt
    file = fopen("student_information.txt", "r");
    fscanf(file, "%d", &num);
    stuT = (studentT*)malloc(sizeof(studentT)*num);
    for(int i = 0; i < num; i++) { 
        fscanf(file, "%s %d %s", char_temp1, &stuT[i].studentID, char_temp2);
        stuT[i].name = strCopy(char_temp1);
        stuT[i].major = strCopy(char_temp2);
    }
    fclose(file);

    // write output.txt
    file = fopen("output.txt", "w");
    for(int i = 0; i < num; i++) {
        fprintf(file, "%s %d %s", stuT[i].name, stuT[i].studentID, stuT[i].major);
        if(i+1 < num) {
            fprintf(file, "\n");
        }
    }
    fclose(file);
    
    // free
    for(int i = 0; i < num; i++) {
        free(stuT[i].name);
        free(stuT[i].major);
    }
    free(stuT);
}

int strSize(char* str) {
    for(int i = 0; i < 31; i++) {
        if(str[i] == '\0') {
            return i;
        }
    }
}

char* strCopy(char* input) {
    int strsize = strSize(input);
    char *output;
    output = (char*)malloc(sizeof(char)*(strsize+1)); // 1 more for '\0'(NULL)
    for(int i = 0; i < strsize+1; i++) {
        output[i] = input[i];   
    }
    return output;
}
