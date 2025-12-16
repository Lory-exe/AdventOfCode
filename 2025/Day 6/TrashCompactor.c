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

long int operation(const exp_t exp) {
    long int ret = exp.numbers[0];
    for (int i = 1; i < MAX_NUMBERS; i++) {
        switch (exp.operationType) {
            case SUM:
                ret += exp.numbers[i];
                break;
            case MUL:
                ret *= exp.numbers[i];
        }
    }
    return ret;
}

int main(int argc, char *argv[]) {
    exp_t *exps;
    int count;
    
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
    fclose(f);
    if (exps == NULL) {
        fprintf(stderr, "Impossible to load data from file\n");
        return 1;
    }

    long int total = 0;
    for (int i = 0; i < count; i++) {
        total += operation(exps[i]);
    }

    printf("The grand total is '%ld'\n", total);

    return 0;
}