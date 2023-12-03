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

int A = 0, X = 0, L = 0, PC = 0, SW = 0;
int wordCount = 1;
int line_count = 0;
char num[MAX_WORDS][500];
int cnt = 0;
int TMP = 0;

// "WORD" 키워드를 처리하는 함수
void processFullLine(char *line) {
    char *wordKeyword = strstr(line, "WORD");

    if (wordKeyword != NULL) {
        wordKeyword += strlen("WORD") + 1;

        char *ptr = strtok(wordKeyword, " ");
        while (ptr != NULL && wordCount < MAX_WORDS) {
            strcpy(num[wordCount], ptr);
            printf("NUM%d Type : WORD\n", wordCount);
            printf("      Value : %s\n", num[wordCount]);
            wordCount++;

            ptr = strtok(NULL, " ");
        }
    }

    char *reswKeyword = strstr(line, "RESW");

    if(reswKeyword != NULL) {
        reswKeyword += strlen("RESW") + 1;
        TMP = atoi(reswKeyword);
        printf("%s Type : RESW\n");
    }
}

// 한 줄을 처리하고 "START"를 찾아 PC를 업데이트하는 함수
void processLine(char *line, struct Log *registers) {
    char *startKeyword = strstr(line, "START");

    if (startKeyword != NULL) {
        startKeyword += strlen("START") + 1;
        PC = atoi(startKeyword);
        registers->PC = PC;
    } else if (cnt==1){
        PC += 0;
        registers->PC = PC;
    } 
    else if(cnt>1) {
        PC += 3;
        registers->PC = PC;
    }

    char *ldaKeyword = strstr(line, "LDA");

    if (ldaKeyword != NULL) {
        ldaKeyword += strlen("LDA") + 1;

        char *ptr = strtok(ldaKeyword, " ");
        while (ptr != NULL) {
            if (strcmp(ptr, "NUM1") == 0) {
                A = atoi(num[1]);
            } else if (strcmp(ptr, "NUM2") == 0) {
                A = atoi(num[2]);
            }
            registers->A = A;
            ptr = strtok(NULL, " ");
        }
    } else if (ldaKeyword == NULL){
        A += 0;
        registers->A = A;
    }

    // 다른 레지스터 업데이트가 필요한 경우 추가
    registers->X = X;
    registers->L = L;
    registers->SW = SW;

    cnt++;
}

// 레지스터 값을 출력하는 함수
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

    printf("[Symbols]\n");

    if (file != NULL) {
        while (fgets(mem[line_count], sizeof(mem[0]), file) != NULL) {
            size_t len = strlen(mem[line_count]);
            if (len > 0 && mem[line_count][len - 1] == '\n') {
                mem[line_count][len - 1] = '\0';
            }

            processFullLine(mem[line_count]);
            line_count++;
        }

        fclose(file);
        line_count = 0;

        file = fopen("code.txt", "r");

        if (file != NULL) {
            while (fgets(mem[line_count], sizeof(mem[0]), file) != NULL) {
                size_t len = strlen(mem[line_count]);
                if (len > 0 && mem[line_count][len - 1] == '\n') {
                    mem[line_count][len - 1] = '\0';
                }

                processLine(mem[line_count], &registers[line_count]);

                if (line_count >= MAX_LINES) {
                    printf("Warning: Maximum number of lines reached.\n");
                    break;
                }

                line_count++;
            }

            fclose(file);

            for (int i = 0; i < line_count; i++) {
                printf("\nRegisters for Line %d:\n", i + 1);
                printRegisters(&registers[i]);
            }
        } else {
            printf("입력 파일이 존재하지 않습니다.\n");
        }
    } else {
        printf("입력 파일이 존재하지 않습니다.\n");
    }

    return 0;
}
