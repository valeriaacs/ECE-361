#include "ptr.h"
#include <stddef.h>

void swap_float(float *a, float *b){
    if( a == NULL || b == NULL){
        return;
    }

    float temp = *a;
    *a = *b;
    *b = temp;
}

int minmax(const float *a, int n, float *min, float *max){
    if( a == NULL || min == NULL || max == NULL || n < 1){
        return -1;
    }

    // Initialize minimum and maximum with the first elements of the array
    float cur_min = *a;
    float cur_max = *a;
    
   for (const float *p = a; p < a + n; p++){
    if(*p < cur_min){
        cur_min = *p;
    }
    if(*p > cur_max){
        cur_max = *p;
    }
   }

*min = cur_min;
*max = cur_max;
return 0;
}

const float *find_above(const float *a, int n, float threshold) {
    if (a == NULL || n <= 0) {
        return NULL;
    }

    for (const float *p = a; p < a + n; p++) {
        if (*p > threshold) {
            return p; 
        }
    }

    return NULL;
}

void reverse(float *a, int n) {
    // Base case: No need to reverse if array is NULL or has less than 2 elements
    if (a == NULL || n < 2) {
        return;
    }

    float *left = a;
    float *right = a + (n - 1);

    while (left < right) {
        swap_float(left, right);
        left++;
        right--;
    }
}