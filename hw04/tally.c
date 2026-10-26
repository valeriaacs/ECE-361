#include "tally.h"
#include <stdio.h>
#include <string.h>

/* Entry structure holding a name string and its associated count */
typedef struct {
    char name[TALLY_LEN + 1]; /* +1 for the null terminator '\0' */
    int count;
} entry_t;

/* Complete definition of struct tally */
struct tally {
    int in_use;                      /* Flag indicating if slot is active in static pool */
    entry_t entries[TALLY_NAMES];    /* Array of entries kept sorted by strcmp */
    int size;                        /* Current number of unique names stored */
};

/* Static array pool of tallies */
static tally_t pool[TALLY_MAX];

tally_t *tally_create(void) {
    for (int i = 0; i < TALLY_MAX; i++) {
        if (!pool[i].in_use) {
            pool[i].in_use = 1;
            pool[i].size = 0;
            return &pool[i];
        }
    }
    return NULL; /* All TALLY_MAX tallies are currently in use */
}

void tally_destroy(tally_t *t) {
    if (t == NULL) {
        return; /* NULL does nothing */
    }
    t->in_use = 0;
    t->size = 0;
}

int tally_add(tally_t *t, const char *name) {
    if (t == NULL || name == NULL) {
        return -1; 
    }

    /* Safety: Check lenght before copying */
    size_t len = strlen(name);
    if (len == 0 || len > TALLY_LEN) {
        return -1; /* Name is empty or exceeds maximum permitted length*/
    }

    /* Binary search / position lookup since array is kept sorted */
    int i = 0;
    while (i < t->size) {
        int cmp = strcmp(name, t->entries[i].name);
        if (cmp == 0) {
            /* Name already exists: increment count and return new value */
            t->entries[i].count++;
            return t->entries[i].count; 
        }
        if (cmp < 0) {
            /* Insert position found */
            break;
        }
        i++;
    }

    /* New name case: check if tally is full */
    if (t->size >= TALLY_NAMES) {
        return -1; /* Tally is full[cite: 1] */
    }

    /* Shift later entries right by one to maintain sorted order */
    for (int j = t->size; j > i; j--) {
        t->entries[j] = t->entries[j - 1];
    }

    /* Copy name safely (length was verified above) */
    strcpy(t->entries[i].name, name);
    t->entries[i].count = 1;
    t->size++;

    return t->entries[i].count;
}

int tally_count(const tally_t *t, const char *name) {
    if (t == NULL || name == NULL) {
        return 0;
    }

    for (int i = 0; i < t->size; i++) {
        int cmp = strcmp(name, t->entries[i].name);
        if (cmp == 0) {
            return t->entries[i].count;
        }
        if (cmp < 0) {
            /* Array is sorted; past the target position */
            break;
        }
    }

    return 0; /* Name was never added */
}

int tally_size(const tally_t *t) {
    if (t == NULL) {
        return 0; 
    }
    return t->size;
}

void tally_print(const tally_t *t, FILE *out) {
    if (t == NULL || out == NULL) {
        return;
    }

    /* Print each entry in alphabetical order */
    for (int i = 0; i < t->size; i++) {
        fprintf(out, "%s %d\n", t->entries[i].name, t->entries[i].count);
    }
}