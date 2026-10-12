# HW02: Recursive Analysis
Valeria Yanez Garcia

*Overview*
* **`main`**: Serves as the central orchestrator that glues all other modules together and handles printing the final summary results. It reads input into a local array via standard input using `reading_read`. To prevent redefinition errors, all header files throughout the project include `#ifndef` include guards.

* **`reading`**: Responsible for defining core data structures, reading input lines from standard input, skipping comments, parsing valid readings, and tracking invalid lines. Variable counts (such as skipped lines) are updated using pointers passed into these functions.

* **`stats`**: Handles statistical calculations on the data arrays, including calculating the minimum, maximum, and mean. It tracks the longest consecutive run of temperatures strictly above a given threshold, implements the recursive maximum logic, and handles threshold run searching.

* **`histogram`**: Takes all collected temperature readings, groups them into 5°C temperature bins, and formats and prints the histogram chart to match legacy output behavior byte-for-byte.

All public functions exposed across `reading.c`, `stats.c`, and `histogram.c` are declared in their respective `.h` header files, meaning they form the module's public API and must not be marked as `static`. Only internal, unexposed helper functions are declared as `static`.

*How to Build and Test*
First make sure you have:
- `gcc` compiler with standard C11 support
- `make` build system

To compile the `readings` executable with strict flags (`-std=c11 -Wall -Wextra -pedantic`):
bash
make

To run all tests
make test

Clean all generated object files
make clean

*Bonus*
# Compile depth test at -O0 and -O2
gcc -std=c11 -Wall -Wextra -O0 -o depth_O0 tests/depth.c stats.c reading.c
gcc -std=c11 -Wall -Wextra -O2 -o depth_O2 tests/depth.c stats.c reading.c

# Run tests
./depth_O0 standard 150000
./depth_O2 tail 20000000

*Module Diagram*
+-------------------------------------------------------+
|                        main                           |
+-------------------------------------------------------+
    |                    |                    |
    v                    v                    v
+-----------+       +-----------+       +---------------+
|  reading  |       |   stats   |       |   histogram   |
+-----------+       +-----------+       +---------------+