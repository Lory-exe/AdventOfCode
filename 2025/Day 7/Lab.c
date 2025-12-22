#include <stdio.h>
#include <stdlib.h>

#define BEAM '|'
#define MARK 'S'
#define EMPTY '.'
#define SPLIT '^'

char **loadData(FILE *f, int *rows, int *columns) {
    char line[1024];
    int r = 0;
    char **ret = NULL;

    while (fgets(line, sizeof(line), f)) {
        int len = 0;
        while(line[len] != '\n' && line[len] != '\0') len++;
        
        if (r == 0) *columns = len;

        ret = realloc(ret, (r + 1) * sizeof(char *));
        ret[r] = malloc((*columns) * sizeof(char));
        
        for(int j = 0; j < *columns; j++) {
            ret[r][j] = line[j];
        }
        r++;
    }

    *rows = r;
    return ret;
}

int spreadBeam(char **diagram, const int rows, const int startX, const int startY) {
    int splits = 0;

    for (int i = startX; i < rows; i++) {
        switch (diagram[i][startY]) {
            case BEAM:
                return 0;
            case EMPTY:
                diagram[i][startY] = BEAM;
                break;
            case SPLIT:
                splits++;
                splits += spreadBeam(diagram, rows, i, startY - 1);
                splits += spreadBeam(diagram, rows, i, startY + 1);
                i = rows;
        }
    }

    return splits;
}

long timeLines(char **diagram, const int rows, const int columns, const int startX, const int startY) {
    long ret = 0;

    long **paths = calloc(rows, sizeof(long *));
    for (int i = 0; i < rows; i++) {
        paths[i] = calloc(columns, sizeof(long));
    }

    paths[startX][startY] = 1;

    for (int i = startX; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (paths[i][j] == 0) continue;

            if (i == rows - 1) {
                ret += paths[i][j];
                continue;
            }

            if (diagram[i][j] == SPLIT) {
                if (j - 1 >= 0) {
                    paths[i + 1][j - 1] += paths[i][j];
                }
                if (j + 1 < columns) {
                    paths[i + 1][j + 1] += paths[i][j];
                }
            } else {
                paths[i + 1][j] += paths[i][j];
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        free(paths[i]);
    }
    free(paths);
    return ret;
}

int main(int argc, char *argv[]) {
    char **diagram;
    int rows, columns;
    
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
    diagram = loadData(f, &rows, &columns);
    fclose(f);
    if (diagram == NULL) {
        fprintf(stderr, "Impossible to load data from file\n");
        return 1;
    }


    int startX = -1, startY = -1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (diagram[i][j] == MARK) {
                startX = i;
                startY = j;
                break;
            }
        }
    }
    if (startX == -1 || startY == -1) {
        fprintf(stderr, "Impossible to find a start for the Beam\n");
        return 1;
    }

    int splits = spreadBeam(diagram, rows, startX + 1, startY);

    printf("The beam splitted '%ld' times\n", splits);

    long int timelines = timeLines(diagram, rows, columns, startX + 1, startY);
    printf("Total active timelines: %ld\n", timelines);
    
    return 0;
}