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

    printf("Password: %d\n", zeros);
    
    return 0;
}