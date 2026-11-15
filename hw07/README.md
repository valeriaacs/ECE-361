# HW07: Dynamic Memory
Valeria Yanez Garcia

*Overview*
The `istack` module implements a heap-allocated, dynamically resizing integer stack (`istack_t`) with an opaque struct definition in `istack.c`. Lifecycle management is handled by `istack_create`, which allocates the stack header and an initial backing buffer of `ISTACK_START` (8) integers, and `istack_destroy`, which safely ignores `NULL` pointers and frees both the internal `data` array and the struct itself. Core stack operations follow standard LIFO semantics: `istack_push` appends an integer to the top of the stack and doubles the buffer capacity via `realloc` whenever `count == capacity`, `istack_pop` removes the top element and writes it to an optional output pointer (failing safely if the stack is `NULL` or empty), and `istack_peek` copies the top element without modifying the stack. Finally, `istack_count`, `istack_is_empty`, and `istack_capacity` provide read-only state inspection (treating `NULL` stacks safely), while `istack_to_array` allocates and returns an independent, bottom-to-top heap copy of the stack's elements via `memcpy` so callers can inspect the contents without breaking encapsulation.

*How to Build and Test*
## Build and Test
From the `hw07/` directory, run `make` (or `make balance`) to compile `istack.c` and `balance.c` into the `./balance` executable with strict C11 warning flags (`-Wall -Wextra -Werror -std=c11 -pedantic`), which you can then run against any input file via `./balance < inputs/balanced.txt`. To verify everything works as expected, run `make test` to compile and execute both the `istack` unit test suite (`./tests/test_istack`) and the end-to-end regression script (`./regress.sh`), which checks `./balance` output and exit codes against the baseline files in `expected/`. Finally, run `make memcheck` to rebuild with AddressSanitizer and UndefinedBehaviorSanitizer (`-fsanitize=address,undefined`) and re-run both test suites to confirm there are no memory leaks or invalid memory accesses, and use `make clean` to remove all compiled binaries and object files.

*Memory behavior:Error Messages*
### Bug 1
ERROR: AddressSanitizer: heap-use-after-free
### Explanation
After the first `istack_destroy` frees the struct and array back to the allocator, the dangling pointer s still holds that now-poisoned memory address, so the second call immediately crashes with a use-after-free read when trying to access `s->data` before `free()` can ever run again.

# Bug 2 
ERROR: AddressSanitizer: heap-use-after-free
### Explanation
Because `s` remains a non-NULL dangling pointer to heap memory already freed by `istack_destroy`, calling `istack_push` crashes on a heap-use-after-free read when trying to check `s->count` (and `s->capacity`) before writing to the stack.

# Bug 3
ERROR: LeakSanitizer: detected memory leaks
### Explanation
Because the caller that owns the stack is responsible for freeing it, omitting `istack_destroy` leaves the heap-allocated `istack_t` struct and its internal data array unfreed when balance exits, which LeakSanitizer flags as a memory leak.