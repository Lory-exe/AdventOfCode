#include <bankArrayList.h>
#include <math.h>

#define DIGITS 12

bank_t *loadData(FILE *f, bank_t *list) {
    char *inp, c;
    int strLength = 25;

    if (list != NULL) return NULL;
    if (!(inp = malloc(strLength * sizeof(char)))) return NULL;

    int i = 0;
    do {
        c = fgetc(f);
        if (c != EOF && c != '\n') {
            if (i >= strLength - 1) {
                strLength *= 2;
                if (!(inp = realloc(inp, strLength * sizeof(char)))) {
                    free(inp);
                    return NULL;        
                }
            }
            inp[i++] = c;
        } else {
            inp[i] = '\0';
            i = 0;

            list = push(list, newBank(inp));
        }
    } while (c != EOF);

    free(inp);
    return list;
}

int joltageRating(char *v) {
    int ret[2] = { 0 };
    int idx = 0;

    if (v == NULL) return -1;

    for (int i = 0; i < strlen(v) - 1; i++) {
        int val = v[i] - '0';
        if (val > ret[0]) {
            ret[0] = val;
            idx = i;
        }
    }

    for (int i = (idx + 1); i < strlen(v); i++) {
        int val = v[i] - '0';
        if (val > ret[1]) {
            ret[1] = val;
        }
    }

    return (ret[0] * 10 + ret[1]);
}

long int joltageRatingPart2(char *v) {
    int ret[DIGITS] = { 0 };
    int idx = -1;

    if (v == NULL) return -1;

    for (int count = 0; count < DIGITS; count++) {
        int i = (idx + 1);

        for (; i < strlen(v) - (DIGITS - 1 - count); i++) {
            int val = v[i] - '0';
            if (val > ret[count]) {
                ret[count] = val;
                idx = i;
            }
        }
        if (i >= strlen(v) - (DIGITS - 1 - count)) {
            ret[count] = v[idx] - '0';
        }
    }

    long int value = 0;
    for (int i = 0; i < DIGITS; i++) {
        value += ret[i] * pow(10, DIGITS - i - 1);
    }

    return value;
}

int main(int argc, char *argv[]) {
    bank_t *banks = NULL;

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
    banks = loadData(f, banks);
    fclose(f);
    
    int joltageSum = 0;
    long int joltageSumPt2 = 0;
    for (int i = 0; i < listLen(banks); i++) {
        joltageSum += joltageRating(getAt(banks, i)->batteries);
        joltageSumPt2 += joltageRatingPart2(getAt(banks, i)->batteries);
    }

    printf("Joltage Sum: %d\n", joltageSum);
    printf("Joltage Sum Pt.2: %ld\n", joltageSumPt2);

    return 0;
}