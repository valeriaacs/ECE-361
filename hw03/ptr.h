#ifndef PTR_H
#define PTR_H
#include <stddef.h>

/*Takes pointers to two float variables a and b*/
void swap_float(float *a, float *b);

/*Takes a read only float array 'a' and pointes min and max to store the results.*/
/* Returns 0 if sucessful or -1 if n < 1 or if any pointer is NULL */
int minmax(const float *a, int n, float *min, float *max);

/*Takes a read only float array 'a' and  a float threshold.*/
/* Returns the pointer to the first element in 'a' when is greater than the threshold, or NULL if none found */
const float *find_above(const float *a, int n, float threshold);

/*Takes a float array 'a' and its size n. It returns nothing */
void reverse(float *a, int n);
   
#endif
