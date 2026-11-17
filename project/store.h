#ifndef STORE_H
#define STORE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "sensor.h"

/* A single logged entry in the store */
typedef struct {
    reading_t reading;
    int state;
    uint32_t status;
} record_t;

/* Opaque store type (backed by a sorted array for now; BST in week 8) */
typedef struct store store_t;

/* Callback signature for in-order traversal */
typedef void (*store_visit_fn)(const record_t *rec, void *ctx);

/**
 * record_compare() - compares two records by timestamp.
 * Returns < 0 if a is earlier than b, 0 if equal, > 0 if a is later than b.
 */
int record_compare(const record_t *a, const record_t *b);

/**
 * store_create() - allocates and initializes an empty store.
 * Returns NULL on allocation failure.
 */
store_t *store_create(void);

/**
 * store_insert() - inserts a record into the store in sorted timestamp order.
 * If a record with the same timestamp already exists, updates it in place.
 * Returns true on success, false on allocation failure or invalid arguments.
 */
bool store_insert(store_t *s, const record_t *rec);

/**
 * store_find() - looks up a record by timestamp.
 * Returns a pointer to the stored record, or NULL if not found.
 */
const record_t *store_find(const store_t *s, time_t timestamp);

/**
 * store_walk() - visits every record in ascending timestamp order.
 */
void store_walk(const store_t *s, store_visit_fn visit, void *ctx);

/**
 * store_count() - returns the total number of records in the store.
 */
size_t store_count(const store_t *s);

/**
 * store_height() - returns the height of the underlying structure
 * (for a sorted array placeholder: 0 when empty, otherwise count).
 */
size_t store_height(const store_t *s);

/**
 * store_destroy() - frees all memory owned by the store.
 */
void store_destroy(store_t *s);

#endif /* STORE_H */