#include <stddef.h>
#include "istack.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Struct definition Opaque type
struct istack {
    int *data;
    size_t count;
    size_t cap;
};

// Allocates and initializes a new integer stack on the heap
istack_t *istack_create(void) {
    // Allocate memory for the stack struct itself
    istack_t *s = malloc(sizeof *s);
    // Return NULL if struct allocation fails
    if (s == NULL) return NULL;

    // Allocate initial room for ISTACK_START integers on the heap
    s->data = malloc(ISTACK_START * sizeof *s->data);
    // If array allocation fails, free the struct first (no leaks)
    if (s->data == NULL) {
        free(s);
        return NULL;
    }

    // Initialize element count to 0 and capacity to ISTACK_START
    s->count = 0;
    s->cap = ISTACK_START;
    // Return pointer to the newly initialized stack
    return s;
}

// Frees all heap memory associated with the stack
void istack_destroy(istack_t *s) {
    // Do nothing if the stack pointer is already NULL
    if (s == NULL) return;
    // Free the underlying integer array first
    free(s->data);
    free(s);
}

// Pushes an integer onto the top of the stack, doubling capacity when full
int istack_push(istack_t *s, int v) {
    // Reject if the stack pointer is NULL
    if (s == NULL) return -1;

    // Check if the current array is completely full
    if (s->count == s->cap) {
        // Guard against size_t integer overflow before doubling capacity
        if (s->cap > SIZE_MAX / 2 / sizeof *s->data) return -1;

        // Reallocate into a second pointer so a failure leaves s->data intact
        int *bigger = realloc(s->data, 2 * s->cap * sizeof *s->data);
        // Return error without modifying existing stack state if realloc fails
        if (bigger == NULL) return -1;

        // Commit the new pointer and doubled capacity
        s->data = bigger;
        s->cap *= 2;
    }

    // Store the new value at the top and increment count
    s->data[s->count++] = v;
    return 0;
}

// Removes the top element from the stack and writes it to *v (if v is non-NULL)
int istack_pop(istack_t *s, int *v) {
    // Reject if stack is NULL or empty
    if (s == NULL || s->count == 0) return -1;
    // Decrement count to get the top index and copy out if v is provided
    int top = s->data[--s->count];
    if (v != NULL) *v = top;
    return 0;
}

// Reads the top element without removing it from the stack
int istack_peek(const istack_t *s, int *v) {
    // Reject if stack is NULL, empty, or output pointer is NULL
    if (s == NULL || s->count == 0 || v == NULL) return -1;
    // Copy the element at index (count - 1) without changing count
    *v = s->data[s->count - 1];
    return 0;
}

// Returns the number of items currently stored in the stack
size_t istack_count(const istack_t *s) {
    // Return 0 if the stack pointer is NULL, otherwise return count
    return s == NULL ? 0 : s->count;
}

// Checks whether the stack is empty
bool istack_is_empty(const istack_t *s) {
    // Treat a NULL stack or a stack with 0 elements as empty
    return s == NULL || s->count == 0;
}

// Returns the total allocated capacity of the stack
size_t istack_capacity(const istack_t *s) {
    // Return 0 if the stack pointer is NULL, otherwise return cap
    return s == NULL ? 0 : s->cap;
}

// Allocates and returns a heap copy of the stack's elements (bottom to top)
int *istack_to_array(const istack_t *s, size_t *n) {
    // Reject if output length pointer is NULL
    if (n == NULL) return NULL;
    // Initialize output length to 0 in case of early return
    *n = 0;
    // Return NULL if stack is NULL or contains no elements
    if (s == NULL || s->count == 0) return NULL;

    // Allocate a new array sized exactly to the current element count
    int *copy = malloc(s->count * sizeof *copy);
    // Return NULL if heap allocation fails
    if (copy == NULL) return NULL;

    // Copy all active stack elements into the new heap array
    memcpy(copy, s->data, s->count * sizeof *copy);
    // Write the number of copied elements into *n
    *n = s->count;
    // Return the new array (caller owns this memory and must free it)
    return copy;
}