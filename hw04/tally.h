#ifndef TALLY_H
#define TALLY_H
#include <stdio.h>

#define TALLY_MAX 4 /* tallies that can exist at the same time */
#define TALLY_NAMES 16 /* different names one tally can hold */
#define TALLY_LEN 15 /* longest name, without the '\0' */

typedef struct tally tally_t; /* opaque */
/*
Takes nothing. Returns a pointer to a new, empty tally_t, or NULL if TALLY_MAX are already in use*/

tally_t *tally_create(void); /*Initializes a new tally object and returns the pointer to the initalized tally or NULL if allocation fails*/
void tally_destroy(tally_t *t); /*Decreases the tally count so a new tally can be created*/
int tally_add(tally_t *t, const char *name); /*Increments the tally count for name, if name already exists in t, increments its occurrence count by 1 */
int tally_count(const tally_t *t, const char *name); /*Queries how many times name has been tallied. Returns: the current count of name */
int tally_size(const tally_t *t); /*Informs the current tracked unique names in the tally. Returns a number betwwen 0 and TALLY_NAMES, or -1 if t == NULL */
void tally_print(const tally_t *t, FILE *out); /*Outputs the tracked name of the tally an its count to the provided strem out */

#endif /*TALLY_H*/