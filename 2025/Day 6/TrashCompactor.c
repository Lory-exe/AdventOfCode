#include <stdio.h>
#include <stdlib.h>

#define MAX_NUMBERS 4

#define SUM '+'
#define MUL '*'

typedef struct expression_t {
    char operationType;
    int numbers[MAX_NUMBERS];
} exp_t;

exp_t *loadData(FILE *f, int *count) {
    exp_t *ret, *tmp;
    int dim = 16;
    *count = 0;

    if (!(ret = malloc(dim * sizeof(*ret)))) return NULL;

    char c, buff[20];
    int line = 0, column = 0, strChar = 0;
    int numEnd = 0;
    do {
        c = fgetc(f);
        if (c == ' ' || c == EOF || c == '\n') {
            if (numEnd) {
                if (c == '\n') {
                    column = 0;
                    line++;
                }
                continue;
            }
            
            buff[strChar] = '\0';
            numEnd = 1;
            if (sscanf(buff, "%d", &((ret + column)->numbers[line])) != 1) {
                free(ret);
                return NULL;
            }
            if (line == 0) (*count)++;

            if (*count >= dim) {
                dim *= 2;
                if (!(tmp = realloc(ret, dim * sizeof(*ret)))) {
                    free(ret);
                    return NULL;
                }
                ret = tmp;
            }
            
            if (c == '\n') {
                column = 0;
                line++;
            } else {
                column++;
            }
            strChar = 0;
        } else if (c != SUM && c != MUL) {
                buff[strChar++] = c;
                numEnd = 0;
            } else {
                (ret + column)->operationType = c;
                numEnd = 1;
                column++;
            }
    } while(c != EOF);

    ret = realloc(ret, (*count) * sizeof(*ret));
    return ret;
}

void freeMatrixLines(char **matrix, const int LINES) {
    for (int i = 0; i < LINES; i++){
        free(matrix[i]);
    }
    free(matrix);
}

exp_t *loadDataPt2(FILE *f, int *count) {
    exp_t *ret, *tmpRet;
    int dim = 16;
    *count = 0;

    char **lineStr = NULL;
    int initStrLen = 10;
    if (!(lineStr = malloc(MAX_NUMBERS * sizeof(*lineStr)))) {
        return NULL;
    }
    for (int i = 0; i < MAX_NUMBERS; i++) {
        if (!(lineStr[i] = malloc(initStrLen * sizeof(**lineStr)))) {
            freeMatrixLines(lineStr, MAX_NUMBERS);
            return NULL;
        }
        lineStr[i][initStrLen - 1] = '\0';
    }

    char c;
    int line = 0, col = 0;
    int currStrLen = initStrLen;
    do {
        c = fgetc(f);
        if (c != '\n') {
            lineStr[line][col++] = c;
            lineStr[line][col] = '\0';

            if (col >= currStrLen) {
                currStrLen *= 2;

                char *tmp;
                if (!(tmp = realloc(lineStr[line], currStrLen * sizeof(**lineStr)))) {
                    freeMatrixLines(lineStr, MAX_NUMBERS);
                    return NULL;
                }
                lineStr[line] = tmp;
            }
        } else {
            line++;
            currStrLen = initStrLen;
            if (line < MAX_NUMBERS) col = 0;
        }
    } while (line < MAX_NUMBERS);

    if (!(ret = malloc(dim * sizeof(*ret)))) {
        freeMatrixLines(lineStr, MAX_NUMBERS);
        return NULL;
    }

    int currNum = 0;
    for (int i = col - 1; i >= 0; i--) {
        char buff[MAX_NUMBERS + 1];

        for (int j = 0; j < MAX_NUMBERS; j++) {
            buff[j] = lineStr[j][i];
        }
        buff[MAX_NUMBERS] = '\0';


        int num = atoi(buff);
        if (!num) {
            currNum = 0;
            (*count)++;

            if (*count >= dim) {
                dim *= 2;
                if (!(tmpRet = realloc(ret, dim * sizeof(*ret)))) {
                    freeMatrixLines(lineStr, MAX_NUMBERS);
                    free(ret);
                    return NULL;
                }
                ret = tmpRet;
            }
            continue;
        }
        (ret + *count)->numbers[currNum++] = num;
        if (i == 0) (*count)++;
    }

    int currExp = *count - 1;
    do {
        c = fgetc(f);

        switch (c) {
            case ' ':
                break;
            case EOF:
                break;
            default:
                (ret + currExp)->operationType = c;
                currExp--;
        }
    } while(c != EOF);
    
    ret = realloc(ret, (*count) * sizeof(*ret));
    return ret;
}

long int operation(const exp_t exp) {
    long int ret = exp.numbers[0];
    for (int i = 1; i < MAX_NUMBERS; i++) {
        switch (exp.operationType) {
            case SUM:
                ret += exp.numbers[i];
                break;
            case MUL:
                if (exp.numbers[i] != 0) ret *= exp.numbers[i];
        }
    }
    return ret;
}

int main(int argc, char *argv[]) {
    exp_t *exps, *expsPt2;
    int count, countPt2;
    
    if (argc != 2) {
        fprintf(stderr, "MUST be only 1 argument rappresenting the data file name!\n");
        return 1;
    }
    
    FILE *f;
    f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "Impossible to open the file: %s\n", argv[1]);
        return 1;
    }
    exps = loadData(f, &count);
    rewind(f);
    expsPt2 = loadDataPt2(f, &countPt2);
    fclose(f);
    if (exps == NULL || expsPt2 == NULL) {
        fprintf(stderr, "Impossible to load data from file\n");
        return 1;
    }

    long int total = 0;
    for (int i = 0; i < count; i++) {
        total += operation(exps[i]);
    }
    printf("The grand total is '%ld'\n", total);

    total = 0;
    for (int i = 0; i < countPt2; i++) {
        total += operation(expsPt2[i]);
    }
    printf("The grand total for Pt.2 is '%ld'\n", total);

    return 0;
}