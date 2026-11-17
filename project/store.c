#include <stdlib.h>
#include <string.h>
#include "store.h"

#define INITIAL_CAPACITY 16

struct store {
    record_t *items;
    size_t count;
    size_t capacity;
};

int record_compare(const record_t *a, const record_t *b) {
    if (a->reading.timestamp < b->reading.timestamp) {
        return -1;
    }
    if (a->reading.timestamp > b->reading.timestamp) {
        return 1;
    }
    return 0;
}

store_t *store_create(void) {
    store_t *s = malloc(sizeof(*s));
    if (s == NULL) {
        return NULL;
    }

    s->items = malloc(INITIAL_CAPACITY * sizeof(*s->items));
    if (s->items == NULL) {
        free(s);
        return NULL;
    }

    s->count = 0;
    s->capacity = INITIAL_CAPACITY;
    return s;
}

/* Binary search helper: returns index of target or insertion index if absent */
static size_t lower_bound(const store_t *s, const record_t *target, bool *found) {
    size_t lo = 0;
    size_t hi = s->count;
    *found = false;

    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        int cmp = record_compare(&s->items[mid], target);
        if (cmp == 0) {
            *found = true;
            return mid;
        } else if (cmp < 0) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

bool store_insert(store_t *s, const record_t *rec) {
    if (s == NULL || rec == NULL) {
        return false;
    }

    bool found = false;
    size_t idx = lower_bound(s, rec, &found);

    if (found) {
        s->items[idx] = *rec;
        return true;
    }

    if (s->count == s->capacity) {
        size_t new_cap = s->capacity * 2;
        record_t *new_items = realloc(s->items, new_cap * sizeof(*new_items));
        if (new_items == NULL) {
            return false;
        }
        s->items = new_items;
        s->capacity = new_cap;
    }

    if (idx < s->count) {
        memmove(&s->items[idx + 1], &s->items[idx],
                (s->count - idx) * sizeof(*s->items));
    }

    s->items[idx] = *rec;
    s->count++;
    return true;
}

const record_t *store_find(const store_t *s, time_t timestamp) {
    if (s == NULL || s->count == 0) {
        return NULL;
    }

    record_t key;
    memset(&key, 0, sizeof(key));
    key.reading.timestamp = timestamp;

    bool found = false;
    size_t idx = lower_bound(s, &key, &found);
    if (found) {
        return &s->items[idx];
    }
    return NULL;
}

void store_walk(const store_t *s, store_visit_fn visit, void *ctx) {
    if (s == NULL || visit == NULL) {
        return;
    }

    for (size_t i = 0; i < s->count; i++) {
        visit(&s->items[i], ctx);
    }
}

size_t store_count(const store_t *s) {
    return (s != NULL) ? s->count : 0;
}

size_t store_height(const store_t *s) {
    return (s != NULL) ? s->count : 0;
}

void store_destroy(store_t *s) {
    if (s == NULL) {
        return;
    }
    free(s->items);
    free(s);
}