#include <stdio.h>
#include <stdlib.h>

typedef struct range_t {
    long int min;
    long int max;
} range_t;

range_t *loadRanges(FILE *f, int *count) {
    range_t *ranges, *tmp;
    char buff[100];
    int nConv, dim = 4;

    *count = 0;
    dim = 4;
    if (!(ranges = malloc(dim * sizeof(*ranges)))) return NULL;

    while (fgets(buff, sizeof(buff), f)) {
        if (buff[0] == '\n') {
            break;
        }

        nConv = sscanf(buff, "%ld-%ld", &(ranges + *count)->min, &(ranges + *count)->max);

        if (nConv != 2) {
            free(ranges);
            return NULL;
        }
        (*count)++;

        if (*count >= dim) {
            dim *= 2;
            if (!(tmp = realloc(ranges, dim * sizeof(*ranges)))) {
                free(ranges);
                return NULL;
            }
            ranges = tmp;
        }
    }

    ranges = realloc(ranges, (*count) * sizeof(*ranges));
    return ranges;
}

long int *loadIds(FILE *f, int *count) {
    long int *ids, *tmp;
    char buff[100];
    int nConv, dim = 4;

    *count = 0;
    dim = 4;
    if (!(ids = malloc(dim * sizeof(*ids)))) return NULL;

    while (fgets(buff, sizeof(buff), f)) {
        nConv = sscanf(buff, "%ld", (ids + *count));

        if (nConv != 1) {
            free(ids);
            return NULL;
        }
        (*count)++;

        if (*count >= dim) {
            dim *= 2;
            if (!(tmp = realloc(ids, dim * sizeof(*ids)))) {
                free(ids);
                return NULL;
            }
            ids = tmp;
        }
    }

    ids = realloc(ids, (*count) * sizeof(*ids));
    return ids;
}

int inRange(const range_t *ranges, const int nRanges, const long int value) {
    for (int i = 0; i < nRanges; i++) {
        if (value >= (ranges + i)->min && value <= (ranges + i)->max) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    range_t *ranges;
    long int *ids;
    int nRanges, nIds;

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
    ranges = loadRanges(f, &nRanges);
    ids = loadIds(f, &nIds);
    fclose(f);

    if (ranges == NULL || ids == NULL) {
        fprintf(stderr, "Impossible to load data from the File\n");
        return 1;
    }

    int freshIngredients = 0;
    for (int i = 0; i < nIds; i++) {
        // printf("Value_%d: \t%ld\n", i, ids[i]);
        freshIngredients += inRange(ranges, nRanges, ids[i]);
    }

    printf("Fresh ingredients: %d\n", freshIngredients);

    return 0;
}