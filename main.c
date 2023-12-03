#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINES 100
#define MAX_WORDS 10

struct Log {
    int A;
    int X;
    int L;
    int PC;
    int SW;
};

// Define registers
int A = 0, X = 0, L = 0, PC = 0, SW = 0;
int wordCount = 1;
int line_count = 0;
char num[MAX_WORDS][500];

void processFullLine(char *line) {
    // Check if the line contains the "WORD" keyword
    char *wordKeyword = strstr(line, "WORD");

    if (wordKeyword != NULL) {
        // Move the pointer to the next character after the space following "WORD"
        wordKeyword += strlen("WORD") + 1;

        // Use strtok to extract the value after "WORD" and store it in num array
        char *ptr = strtok(wordKeyword, " ");
        while (ptr != NULL && wordCount < MAX_WORDS) {
            strcpy(num[wordCount], ptr);
            printf("NUM[%d]: %s\n", wordCount, num[wordCount]);
            wordCount++;

            ptr = strtok(NULL, " ");
        }
    }
}

// Function to process a line and update PC if "START" is found
void processLine(char *line, struct Log *registers) {
    // Check if the line contains the "START" keyword
    char *startKeyword = strstr(line, "START");

    if (startKeyword != NULL) {
        // Move the pointer to the next character after the space following "START"
        startKeyword += strlen("START") + 1;

        // Parse the number and update PC
        PC = atoi(startKeyword);
        registers->PC = PC;

        printf("PC: %d\n", registers->PC);
    } else if (startKeyword == NULL){
        PC += 3;
        registers->PC = PC;
    }
    
    char *ldaKeyword = strstr(line, "LDA");

    if (ldaKeyword != NULL) {
        printf("hello?");
        ldaKeyword += strlen("LDA") + 1;

        char *ptr = strtok(ldaKeyword, " ");
        while (ptr != NULL) {
            if (strcmp(ptr, "NUM1") == 0) {
                printf("%s, %s", num[1], num[2]);
                A = atoi(num[1]);
                printf("%d", A);
            } else if (strcmp(ptr, "NUM2") == 0) {
                A = atoi(num[2]);
                printf("%d", A);
            }
            ptr = strtok(NULL, " ");
        }
    }

    // Update other registers if necessary
    registers->A = A;
    registers->X = X;
    registers->L = L;
    registers->SW = SW;
}

// Function to print all lines stored in mem
void printRegisters(struct Log *registers) {
    printf("A: %d\n", registers->A);
    printf("X: %d\n", registers->X);
    printf("L: %d\n", registers->L);
    printf("PC: %d\n", registers->PC);
    printf("SW: %d\n", registers->SW);
}

int main() {
    FILE *file = NULL;
    char mem[MAX_LINES][500];

    struct Log registers[MAX_LINES];

    file = fopen("code.txt", "r");

    if (file != NULL) {
        while (fgets(mem[line_count], sizeof(mem[0]), file) != NULL) {
            size_t len = strlen(mem[line_count]);
            if (len > 0 && mem[line_count][len - 1] == '\n') {
                mem[line_count][len - 1] = '\0';
            }

            // Process full line
            processFullLine(mem[line_count]);

            line_count++;

        }
        
        line_count = 0;

        while (fgets(mem[line_count], sizeof(mem[0]), file) != NULL) {

            // Remove newline character if present
            size_t len = strlen(mem[line_count]);
            if (len > 0 && mem[line_count][len - 1] == '\n') {
                mem[line_count][len - 1] = '\0';
            }

            // Process the line
            processLine(mem[line_count], &registers[line_count]);

            // Check if the maximum number of lines has been reached
            if (line_count >= MAX_LINES) {
                printf("Warning: Maximum number of lines reached.\n");
                break;
            }
        }

        fclose(file);

        // Print all lines
        for (int i = 0; i < line_count; i++) {
            printf("\nRegisters for Line %d:\n", i + 1);
            printRegisters(&registers[i]);
        }
    } else {
        printf("입력 파일이 존재하지 않습니다.\n");
    }

    return 0;
}
