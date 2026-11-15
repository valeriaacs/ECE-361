#ifndef ISTACK_H
#define ISTACK_H
#include <stdbool.h>
#include <stddef.h>

#define ISTACK_START 8 /* room at first; doubles when full */
typedef struct istack istack_t; /* opaque */

/*
Allocates a new istack_t struct on the heap. Sets count to 0 and capacity to ISTACK_START (8).
Returns: pointer to new stack (or NULL on allocation failure)
Assumes: caller will later call istack_destroy() to free memory
*/
istack_t *istack_create(void);

/*
Frees the internal array and the istack_t struct itself.
Takes: stack pointer s
Returns: nothing (void)
Assumes: s is a valid stack pointer created by istack_create() or NULL
*/
void istack_destroy(istack_t *s);

/*
Pushes an integer v onto the top of the stack, doubling capacity if full.
Takes: stack pointer s, integer value v
Returns: 0 on success, non-zero on error (NULL stack or realloc failure)
Assumes: s is a valid non-NULL stack pointer
*/
int istack_push(istack_t *s, int v);

/*
Removes the top element from the stack and writes its value to *v.
Takes: stack pointer s, output int pointer v
Returns: 0 on success, non-zero if stack is NULL, empty, or v is invalid
Assumes: s is a valid non-NULL stack pointer; v points to writable int memory (or NULL if discarding)
*/
int istack_pop(istack_t *s, int *v);

/*
Copies the top element's value into *v without removing it from the stack.
Takes: const stack pointer s, output int pointer v
Returns: 0 on success, non-zero if stack is NULL, empty, or v is invalid
Assumes: s is a valid non-NULL stack pointer; v points to writable int memory
*/
int istack_peek(const istack_t *s, int *v);

/*
Reads the number of elements currently stored in the stack.
Takes: const stack pointer s
Returns: number of elements (0 if s is NULL)
Assumes: s is a valid stack pointer or NULL
*/
size_t istack_count(const istack_t *s);

/*
Checks whether the stack currently contains zero elements.
Takes: const stack pointer s
Returns: true if count is 0 or s is NULL, false otherwise
Assumes: s is a valid stack pointer or NULL
*/
bool istack_is_empty(const istack_t *s);

/*
Reads the total number of elements the stack can hold before resizing.
Takes: const stack pointer s
Returns: current allocated capacity (0 if s is NULL)
Assumes: s is a valid stack pointer or NULL
*/
size_t istack_capacity(const istack_t *s);

/*
Allocates a new array on the heap, copies stack elements into it, and writes the length to *n.
Takes: const stack pointer s, output size pointer n
Returns: pointer to the new heap-allocated int array (or NULL if empty, invalid, or on allocation failure)
Assumes: s is valid and n is non-NULL; caller owns the returned array and must call free()
*/
int *istack_to_array(const istack_t *s, size_t *n);

#endif