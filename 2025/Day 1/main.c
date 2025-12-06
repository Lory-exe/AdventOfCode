#include <stdio.h>
#include <stdlib.h>

#define STARTING_POINT 50
#define MIN_POINT 0
#define MAX_POINT 99

#define LEFT 'L'
#define RIGHT 'R'

typedef struct action_t {
    char dir;
    int click;
} action_t;

struct action_t *loadData(FILE *f, int *count) {
    action_t *actions, *tmp;
    char buff[100];
    int nConv, dim = 4;

    *count = 0;
    dim = 4;
    if (!(actions = malloc(dim * sizeof(*actions)))) return NULL;

    while (fgets(buff, sizeof(buff), f)) {
        nConv = sscanf(buff, "%c%d", &(actions + *count)->dir, &(actions + *count)->click);

        if (nConv != 2) {
            free(actions);
            return NULL;
        }
        (*count)++;

        if (*count >= dim) {
            dim *= 2;
            if (!(tmp = realloc(actions, dim * sizeof(*actions)))) {
                free(actions);
                return NULL;
            }
            actions = tmp;
        }
    }

    actions = realloc(actions, (*count) * sizeof(*actions));
    return actions;
}

void makeAction(action_t *a, int *pointing) {
    int mv = a->click;

    switch (a->dir) {
        case LEFT:
            *pointing = (*pointing - mv) % (MAX_POINT + 1);
            if (*pointing < 0) *pointing += (MAX_POINT + 1);
            break;
        case RIGHT:
            *pointing = (*pointing + mv) % (MAX_POINT + 1);
    }
}

void makeActionPart2(action_t *a, int *pointing, int *zeros) {
    int mv = a->click;
    int tmp;

    int rounds = mv / (MAX_POINT + 1);
    *zeros += rounds;

    int rmngMv = mv % (MAX_POINT + 1);

    switch (a->dir) {
        case LEFT:
            if ((tmp = *pointing - rmngMv) <= MIN_POINT) {
                if (*pointing != MIN_POINT) (*zeros)++;
                *pointing = MAX_POINT + (tmp + 1);
            } else {
                *pointing -= rmngMv;
            }
            break;
        case RIGHT:
            if ((tmp = *pointing + rmngMv) > MAX_POINT) {
                if (*pointing != MAX_POINT + 1) (*zeros)++;
                *pointing = (tmp - 1) - MAX_POINT;
            } else {
                *pointing += rmngMv;
            }
    }
}

int main(int argc, char *argv[]) {
    int pointing = STARTING_POINT, zeros = 0;
    action_t *actions;
    int actionsCount = 0;
    
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
    actions = loadData(f, &actionsCount);
    fclose(f);

    for (int i = 0; i < actionsCount; i++) {
        makeAction((actions + i), &pointing);
        if (pointing == 0) zeros++;
    }
    printf("Password Part 1: %d\n", zeros);
    
    pointing = 50;
    zeros = 0;
    for (int i = 0; i < actionsCount; i++) {
        makeActionPart2((actions + i), &pointing, &zeros);
    }
    printf("Password Part 2: %d\n", zeros);
    
    return 0;
}