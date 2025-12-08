#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bank_t {
    char *batteries;
    struct bank_t *next;
} bank_t;

bank_t *newBank(const char *str) {
    bank_t *out;
    if (!(out = malloc(sizeof(bank_t)))) return NULL;
    
    if (!(out->batteries = malloc(strlen(str) * sizeof(char)))) {
        free(out);
        return NULL;
    }

    strcpy(out->batteries, str);
    out->next = NULL;
    return out;
}

bank_t *getLast(bank_t *list) {
    bank_t *out;
    if (list == NULL) {
        out = NULL;
    } else {
        out = list;
        while (out->next != NULL) {
            out = out->next;
        }
    }
    return out;
}

bank_t *push(bank_t *list, bank_t *new) {
    bank_t *out, *last = getLast(list);
	if (last == NULL) {
		out = new;
	} else {
		last->next = new;
		out = list;
	}
	return out;
}

int listLen(bank_t *currBank) {
    int count = 0;
    while (currBank != NULL) {
        count++;
        currBank = currBank->next;
    }
    return count;
}

bank_t *getAt(bank_t *list, int idx) {
    bank_t *out;
    if (listLen(list) > idx && idx >= 0) {
        out = list;
        int i;
        for (i = 0; i < idx; i++) {
            out = out->next;
        }
    } else {
        out = NULL;
    }
    return out;
}