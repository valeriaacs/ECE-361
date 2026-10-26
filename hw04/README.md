# HW04: Abstract data types: Tally 
Valeria Yanez Garcia

*Overview*
`tally_create(void)` grabs the first available slot from the static pool, resets its size, marks it in-use, and returns its pointer. Returns `NULL` if the pool is completely full or if we run out of static instances.

`tally_destroy(tally_t *t)` releases a tally instance back to the static pool by flipping its in-use flag off and clearing its memory. Does nothing if `t` is `NULL` or points to an invalid slot.

`tally_add(tally_t *t, const char *name)` inserts a name into the tally while keeping the array alphabetically sorted using `strcmp`. If the name already exists, it just bumps the count; if it's new, it shifts later elements right to make room. Returns 0 on success, or -1 if `t` is full, `name` is invalid, or the string exceeds `TALLY_LEN`.

`tally_count(const tally_t *t, const char *name)` searches the tally for a given name and returns its current occurrence count. Returns 0 if the name isn't found, or -1 if `t` or `name` is `NULL`.

`tally_size(const tally_t *t)` returns the total number of unique names currently stored in the tally. Returns -1 if `t` is `NULL`.

*How to Build and Test*
To build and test the project, open the terminal in the root `hw04` directory and run `make`. That command compiles `tally.c`, `events.c`, and `tests/test_tally.c` together using GCC with strict flags (`-Wall -Wextra -Werror -std=c99 -g`), which produces two executables: `events` and `test_tally_runner`. To run the unit test suite, type `make test` (or run `./test_tally_runner` directly) to check that memory pool limits, array shifting, string guards, and counter logic are all working correctly. Executing `./events` runs the main event-tracking program, and running `make clean` wipes the build artifacts for a fresh start.

*Opaque Test: What the Opaque Type Prevents*
### Compiler Error Message
events.c: In function ‘main’:
events.c:19:17: error: invalid use of incomplete typedef ‘tally_t’ {aka ‘struct tally’}
   19 |     int size = t->size;
      |                 ^~

### Explanation
The error message `invalid use of incomplete typedef 'tally_t'` means that the full declaration of `struct tally` is hidden inside `tally.c`. The opaque type makes the compiler refuse to let external code access the internal members of the struct. This is useful because protects the internal struct from external files, ensuring memory safety and modular design.