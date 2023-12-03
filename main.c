#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINES 100
#define MAX_WORDS 10

struct Log { // input code의 각 라인을 구조체화 시켜 저장
    int A;
    int X;
    int L;
    int PC;
    int SW;
};

int A = 0, X = 0, L = 0, PC = 0, SW = 0;
int wordCount = 1; // WORD 갯수
int line_count = 0; // input code의 line 개수
char num[MAX_WORDS][500];
int cnt = 0;
int TMP = 0;

void processFullLine(char *line) {

    // "WORD" 키워드를 처리
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

    // "RESW" 키워드를 처리
    char *reswKeyword = strstr(line, "RESW");

    if (reswKeyword != NULL) {
        reswKeyword += strlen("RESW") + 1;
        TMP = atoi(reswKeyword);
        printf("%s Type : RESW\n", line);
    }
}

void processLine(char *line, struct Log *registers) {

    // 한 줄을 처리하고 "START"를 찾아 PC를 업데이트
    char *startKeyword = strstr(line, "START");

    if (startKeyword != NULL) {
        startKeyword += strlen("START") + 1;
        PC = atoi(startKeyword);
        registers->PC = PC;
    } else if (cnt == 1) {
        PC += 0;
        registers->PC = PC;
    } else if (cnt > 1) {
        PC += 3;
        registers->PC = PC;
    }

    // "LDA" 키워드 처리 후 변수에 맞는 값 불러옴
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
    } else {
        A += 0;
        registers->A = A;
    }

    // 레지스터 구조체 업데이트
    registers->X = X;
    registers->L = L;
    registers->SW = SW;

    cnt++;
}

// 레지스터 값을 출력
void printRegisters(struct Log *registers) {
    printf("A: %d\n", registers->A);
    printf("X: %d\n", registers->X);
    printf("L: %d\n", registers->L);
    printf("PC: %d\n", registers->PC);
    printf("SW: %d\n", registers->SW);
}

int main() {
    FILE *fileSymbols = fopen("code.txt", "r");
    char mem[MAX_LINES][500];
    struct Log registers[MAX_LINES];

    printf("[Symbols]\n");

    if (fileSymbols != NULL) {
        
        // input code를 전체적으로 훑어 미리 필요한 값을 세팅
        while (fgets(mem[line_count], sizeof(mem[0]), fileSymbols) != NULL) {
            size_t len = strlen(mem[line_count]);
            if (len > 0 && mem[line_count][len - 1] == '\n') {
                mem[line_count][len - 1] = '\0';
            }

            processFullLine(mem[line_count]);
            line_count++;
        }

        fclose(fileSymbols);
        line_count = 0;

        FILE *fileCode = fopen("code.txt", "r");

        if (fileCode != NULL) {
            while (fgets(mem[line_count], sizeof(mem[0]), fileCode) != NULL) {
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

            fclose(fileCode);

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
