#include <stdio.h>
#include <stdlib.h>

#define ID_SEPARATOR ','
#define ID_RANGE_SEPARATOR '-'

typedef struct idRange_t {
    long int min, max;
} idRange_t;

idRange_t *loadData(FILE *f, int *count) {
    char c, *inp;
    idRange_t *ranges, *tmp;
    *count = 0;
    int dim = 4, strLength = 25;

    if (!(ranges = malloc(dim * sizeof(*ranges)))) return NULL;
    if (!(inp = malloc(dim * sizeof(char)))) return NULL;

    int i = 0;
    do {
        c = fgetc(f);
        if (c != ID_SEPARATOR && c != EOF) {
            if (i >= strLength - 1) {
                strLength *= 2;
                if (!(inp = realloc(inp, strLength * sizeof(char)))) {
                    free(ranges);
                    free(inp);
                    return NULL;        
                }
            }
            inp[i++] = c;
        } else {
            inp[i] = '\0';
            i = 0;
            
            int nConv = sscanf(inp, "%ld-%ld", &(ranges + *count)->min, &(ranges + *count)->max);
            
            if (nConv != 2) {
                free(ranges);
                free(inp);
                return NULL;
            }
            (*count)++;
            
            if (*count >= dim) {
                dim *= 2;
                if (!(tmp = realloc(ranges, dim * sizeof(*ranges)))) {
                    free(ranges);
                    free(inp);
                    return NULL;
                }
                ranges = tmp;
            }
        }
    } while (c != EOF);

    ranges = realloc(ranges, (*count) * sizeof(*ranges));
    free(inp);
    return ranges;
}

int checkValidity(long int value) {
    char *str;
    int dim = 10;
    if (!(str = malloc(dim * sizeof(char)))) return 1;

    int size = sprintf(str, "%ld", value);
    if (size <= 0 || !(str = realloc(str, (size + 1) * sizeof(char)))) {
        free(str);
        return 1;
    }

    if (size % 2 != 0) {
        return 0;
    }
    int mid = size / 2;
    
    for (int i = 0; i < mid; i++) {
        if (str[i] != str[mid + i]) {
            return 0;
        }
    }

    return 1;   // invalid ID
}

int checkValidityPart2(long int value) {
    char *str;
    int dim = 10;
    if (!(str = malloc(dim * sizeof(char)))) return 1;

    int size = sprintf(str, "%ld", value);
    if (size <= 0 || !(str = realloc(str, (size + 1) * sizeof(char)))) {
        free(str);
        return 1;
    }

    for (int i = 1; i <= size / 2; i++) {
        if (size % i == 0) {
            int is_valid = 1;
            for (int j = i; j < size; j++) {
                if (str[j] != str[j % i]) {
                    is_valid = 0;
                    break;
                }
            }
            if (is_valid) {
                return 1;       // invalid ID
            }
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    idRange_t *ranges;
    int rangesNum = 0;

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
    ranges = loadData(f, &rangesNum);
    fclose(f);

    long int sum = 0, sum2 = 0;
    for (int i = 0; i < rangesNum; i++) {
        for (long int j = (ranges + i)->min; j <= (ranges + i)->max; j++) {
            if (checkValidity(j)) {
                sum += j;
            }
            if (checkValidityPart2(j)) {
                sum2 += j;
            }
        }
    }

    printf("Invalid IDs sum: %ld\n", sum);
    printf("Invalid IDs sum pt.2: %ld\n", sum2);

    return 0;
}