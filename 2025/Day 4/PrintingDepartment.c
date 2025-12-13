#include <stdio.h>
#include <stdlib.h>

#define PAPER_ROLL '@'
#define CHECKED 'x'

#define MAX_ADJACENT_ROLLS 3

char **loadGrid(FILE *f, int *rows, int *columns) {
    *rows = 16;
    char **grid = malloc((*rows) * sizeof(char*));
    if (!grid) return NULL;

    int readLines = 0, maxCols = 0;

    char *line = NULL;
    int lineSize = 0, currReadLen = 0;

    char c;
    do {
        c = fgetc(f);

        if (c != '\n' && c != EOF) {
            if (currReadLen + 1 >= lineSize) {
                lineSize = (lineSize == 0) ? 16 : lineSize * 2;
                char *tmp = realloc(line, lineSize);
                if (!tmp) {
                    free(line);
                    for (int i = 0; i < readLines; i++) free(grid[i]);
                    free(grid);
                    return NULL;
                }
                line = tmp;
            }

            line[currReadLen++] = (char)c;
        } else {
            if (currReadLen == 0) continue;
            line[currReadLen] = '\0';

            if (readLines >= (*rows)) {
                (*rows) *= 2;
                char **tmp = realloc(grid, (*rows) * sizeof(char*));
                if (!tmp) {
                    free(line);
                    for (int i = 0; i < readLines; i++) free(grid[i]);
                    free(grid);
                    return NULL;
                }
                grid = tmp;
            }

            grid[readLines++] = line;

            if (currReadLen > maxCols) maxCols = currReadLen;
            line = NULL;
            lineSize = 0;
            currReadLen = 0;
        }
    } while (c != EOF);

    *rows = readLines;
    *columns = maxCols;

    char **tmp = realloc(grid, (*rows) * sizeof(char*));
    if (tmp) grid = tmp;

    return grid;
}

int isRoll(char c) {
    switch (c) {
        case PAPER_ROLL:
            return 1;
        case CHECKED:
        default:
            return 0;
    }
}

int checkAdjacentRolls(char **grid, const int rows, const int columns, const int x, const int y) {
    int found = 0;

    if (y > 0 && y < columns - 1) {
        for (int i = -1; i < 2; i++) {
            if (x != 0) {
                if (isRoll(grid[x - 1][y + i])) found++;
            }
            if (x != rows - 1) {
                if (isRoll(grid[x + 1][y + i])) found++;
            }
        }
        if (isRoll(grid[x][y - 1])) found++;
        if (isRoll(grid[x][y + 1])) found++;
        
    } else if (y == 0) {
            if (x != 0) {
                if (isRoll(grid[x - 1][y])) found++;
                if (isRoll(grid[x - 1][y + 1])) found++;
            }
            if (x != rows - 1) {
                if (isRoll(grid[x + 1][y])) found++;
                if (isRoll(grid[x + 1][y + 1])) found++;
            }
            if (isRoll(grid[x][y + 1])) found++;

        } else {
            if (x != 0) {
                if (isRoll(grid[x - 1][y])) found++;
                if (isRoll(grid[x - 1][y - 1])) found++;
            }
            if (x != rows - 1) {
                if (isRoll(grid[x + 1][y])) found++;
                if (isRoll(grid[x + 1][y - 1])) found++;
            }
            if (isRoll(grid[x][y - 1])) found++;
        }
        
    return found;
}

int main(int argc, char *argv[]) {
    char **grid;
    int rows = 0, columns = 0;

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
    grid = loadGrid(f, &rows, &columns);
    fclose(f);
    if (grid == NULL) {
        fprintf(stderr, "Impossible to load data\n");
        return 1;
    }

    int removed = 0, checked;
    do {
        checked = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (grid[i][j] == PAPER_ROLL) {
                    if (checkAdjacentRolls(grid, rows, columns, i, j) <= MAX_ADJACENT_ROLLS) {
                        grid[i][j] = CHECKED;
                        checked++;
                    }
                }
            }
        }
        removed += checked;
    } while(checked != 0);

    printf("Total removed: %d\n", removed);

    return 0;
}