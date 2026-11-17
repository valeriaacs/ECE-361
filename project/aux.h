#ifndef AUX_H
#define AUX_H

/* Module: aux
 * Responsible for managing a hash table with separate chaining to track
 * entries and time spent per state.
 * @file    aux.h
 * @brief   Auxiliary data structure interface (S4: Hash Table for Device Type D).
 *
 * Declares an opaque hash table keyed by state_t (or event_t) to accumulate
 * per-state/per-event counts, tick durations, and min/max readings for
 * reporting (Q6). Implemented in Week 9 (aux.c).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "fsm.h"
#include "store.h"

/* Opaque auxiliary hash table type (S4) */
typedef struct aux aux_t;

/* Per-key summary statistics stored in each hash table entry */
typedef struct {
    state_t key;
    size_t count;          /* Number of ticks or occurrences in this state */
    size_t transitions;    /* Number of times entered */
    float min_temp_c;
    float max_temp_c;
} aux_entry_t;

/* Callback signature for iterating over occupied buckets/entries */
typedef void (*aux_visit_fn)(const aux_entry_t *entry, void *ctx);

/**
 * aux_create() - allocates and initializes an empty hash table with the
 * requested bucket count. Returns NULL on allocation failure.
 */
aux_t *aux_create(size_t bucket_count);

/**
 * aux_record() - inserts or updates the statistics entry for a state key
 * using the given record. Returns true on success, false on allocation failure.
 */
bool aux_record(aux_t *table, state_t key, const record_t *rec);

/**
 * aux_lookup() - looks up the entry for a given state key.
 * Returns a pointer to the entry, or NULL if not present.
 */
const aux_entry_t *aux_lookup(const aux_t *table, state_t key);

/**
 * aux_walk() - visits every entry stored in the hash table.
 */
void aux_walk(const aux_t *table, aux_visit_fn visit, void *ctx);

/**
 * aux_used_buckets() - returns the number of non-empty hash buckets
 * (used by Scenario 2 to verify every bucket is exercised).
 */
size_t aux_used_buckets(const aux_t *table);

/**
 * aux_total_count() - returns the sum of counts across all entries
 * (used to verify tick totals match the length of the run).
 */
size_t aux_total_count(const aux_t *table);

/**
 * aux_destroy() - frees all chains, buckets, and the table itself.
 */
void aux_destroy(aux_t *table);

/**
 * build_rgb_reg() - packs enable bit and 8-bit R, G, B channels into
 * the 32-bit value expected by iom361's RGB_LED_REG.
 */
uint32_t build_rgb_reg(int enable, uint8_t r, uint8_t g, uint8_t b);

#endif /* AUX_H */