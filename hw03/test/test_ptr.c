#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../ptr.h"

// Helper function to print test status and track global failures
static bool run_test(const char *test_name, bool passed, int *fail_count) {
    if (passed) {
        printf("[PASS] %s\n", test_name);
    } else {
        printf("[FAIL] %s\n", test_name);
        (*fail_count)++;
    }
    return passed;
}

// Helper to compare floats within tolerance
static bool float_eq(float x, float y) {
    return fabsf(x - y) < 1e-5f;
}

int main(void) {
    int fail_count = 0;

    printf("=== RUNNING PTR UNIT TESTS ===\n\n");

    // -------------------------------------------------------------
    // TEST 1: swap_float
    // -------------------------------------------------------------
    {
        float x = 3.14f;
        float y = 9.81f;
        swap_float(&x, &y);
        bool ok = float_eq(x, 9.81f) && float_eq(y, 3.14f);
        run_test("swap_float: valid pointers", ok, &fail_count);
    }

    // -------------------------------------------------------------
    // TEST 2: minmax
    // -------------------------------------------------------------
    {
        // Case A: Several values (Expect return 0 on success)
        float arr[] = { 4.5f, -1.2f, 10.0f, 0.0f, -3.4f, 8.2f };
        float min_val = 0.0f, max_val = 0.0f;
        int res = minmax(arr, 6, &min_val, &max_val);
        bool ok = (res == 0) && float_eq(min_val, -3.4f) && float_eq(max_val, 10.0f);
        run_test("minmax: several values", ok, &fail_count);

        // Case B: One value (Expect return 0 on success)
        float single_arr[] = { 42.0f };
        min_val = 0.0f; max_val = 0.0f;
        res = minmax(single_arr, 1, &min_val, &max_val);
        ok = (res == 0) && float_eq(min_val, 42.0f) && float_eq(max_val, 42.0f);
        run_test("minmax: single value", ok, &fail_count);

        // Case C: n = 0 (Expect return -1, outputs must stay unchanged)
        min_val = 99.0f; max_val = 99.0f;
        res = minmax(arr, 0, &min_val, &max_val);
        ok = (res == -1) && float_eq(min_val, 99.0f) && float_eq(max_val, 99.0f);
        run_test("minmax: n = 0 unchanged outputs", ok, &fail_count);

        // Case D: NULL pointer (Expect return -1, outputs must stay unchanged)
        min_val = 99.0f; max_val = 99.0f;
        res = minmax(NULL, 5, &min_val, &max_val);
        ok = (res == -1) && float_eq(min_val, 99.0f) && float_eq(max_val, 99.0f);
        run_test("minmax: NULL pointer unchanged outputs", ok, &fail_count);
    }

    // -------------------------------------------------------------
    // TEST 3: find_above
    // -------------------------------------------------------------
    {
        float arr[] = { 1.0f, 5.5f, 3.0f, 8.2f, 2.0f };

        // Case A: Two matches (Must return pointer to first match: 5.5)
        const float *p1 = find_above(arr, 5, 4.0f);
        bool ok = (p1 == &arr[1]) && float_eq(*p1, 5.5f);
        run_test("find_above: two matches returns first", ok, &fail_count);

        // Case B: No matches (Must return NULL)
        const float *p2 = find_above(arr, 5, 10.0f);
        ok = (p2 == NULL);
        run_test("find_above: no matches returns NULL", ok, &fail_count);

        // Case C: Value equal to threshold (Strictly greater requirement)
        const float *p3 = find_above(arr, 5, 8.2f);
        ok = (p3 == NULL);
        run_test("find_above: value equal to threshold returns NULL", ok, &fail_count);
    }

    // -------------------------------------------------------------
    // TEST 4: reverse
    // -------------------------------------------------------------
    {
        // Case A: Even length
        float even_arr[] = { 1.0f, 2.0f, 3.0f, 4.0f };
        reverse(even_arr, 4);
        bool ok = float_eq(even_arr[0], 4.0f) && float_eq(even_arr[1], 3.0f) &&
                  float_eq(even_arr[2], 2.0f) && float_eq(even_arr[3], 1.0f);
        run_test("reverse: even length", ok, &fail_count);

        // Case B: Odd length
        float odd_arr[] = { 10.0f, 20.0f, 30.0f, 40.0f, 50.0f };
        reverse(odd_arr, 5);
        ok = float_eq(odd_arr[0], 50.0f) && float_eq(odd_arr[1], 40.0f) &&
             float_eq(odd_arr[2], 30.0f) && float_eq(odd_arr[3], 20.0f) &&
             float_eq(odd_arr[4], 10.0f);
        run_test("reverse: odd length", ok, &fail_count);
    }

    // -------------------------------------------------------------
    // SUMMARY & EXIT STATUS
    // -------------------------------------------------------------
    printf("\n=== TEST SUMMARY ===\n");
    if (fail_count == 0) {
        printf("ALL TESTS PASSED!\n");
        return EXIT_SUCCESS;
    } else {
        printf("TOTAL FAILURES: %d\n", fail_count);
        return EXIT_FAILURE;
    }
}