# HW03: A Pointer Library
Valeria Yanez Garcia

*Overview*
- `swap_float(float *a, float *b)`: Swaps the values of two floating-point numbers in memory using temporary storage.

- `minmax(const float *a, int n, float *min, float *max)`: Finds the minimum and maximum values in array a[0..n-1] via single-pass pointer traversal. Returns 0 on success, or -1 without modifying output pointers if n < 1 or if any pointer is NULL.

- `find_above(const float *a, int n, float threshold)`: Traverses array a using pointer arithmetic and returns a pointer to the first element strictly greater than threshold (or NULL if none match or inputs are invalid).

- `reverse(float *a, int n)`: Reverses array a[0..n-1] in-place using two converging pointers (left and right) and calls swap_float. Does nothing if a is NULL or n < 2.

*How to Build and Test*
To build and test my code, I navigate to the `hw03` directory and run `make test`. This compiles my source file `(ptr.c)` alongside the unit test suite `(test/test_ptr.c)` using GCC with strict standard C11 flags `(-std=c11 -Wall -Wextra)` and the math library linked `(-lm)`, then automatically executes the test suite to verify that all edge cases pass. If I ever need to remove generated object files and binaries to perform a fresh build, I simply run `make clean` before running `make` again.