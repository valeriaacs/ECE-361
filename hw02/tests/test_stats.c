#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../stats.h"
#include "../reading.h"

static int g_failed = 0;

static void check(int condition, const char *test_name) {
    if (condition) {
        printf("PASS: %s\n", test_name);
    } else {
        printf("FAIL: %s\n", test_name);
        g_failed++;
    }
}

// Test 1: Single value 
static void test_single_value(void) {
    float arr[] = { 42.0f };
    stats_result_t res = stats_calculate(arr, 1);
    check(res.min == 42.0f && res.max == 42.0f && res.mean == 42.0f, "single value");
}

// Test 2: Maximum at start and at end 
static void test_max_positions(void) {
    float arr_start[] = { 100.0f, 10.0f, 20.0f, 30.0f };
    float arr_end[]   = { 10.0f, 20.0f, 30.0f, 100.0f };

    check(stats_max(arr_start, 4) == 100.0f, "maximum at start");
    check(stats_max(arr_end, 4) == 100.0f, "maximum at end");
}

// Test 3: Value exactly equal to threshold ends a run 
static void test_threshold_boundary(void) {
    reading_t readings[4];
    
    readings[0].tick = 1; readings[0].temp = 30.0f;
    readings[1].tick = 2; readings[1].temp = 25.0f; // equal to threshold 
    readings[2].tick = 3; readings[2].temp = 30.0f;
    readings[3].tick = 4; readings[3].temp = 30.0f;

    // Threshold is 25.0f. Temp must be strictly > 25.0f. Index 0 is run of 1. Index 1 terminates run. Index 2.3 is run of 2. 
    stats_run_t run = stats_longest_run(readings, 4, 25.0f);
    check(run.length == 2 && run.start_tick == 3 && run.end_tick == 4, 
          "threshold boundary ends run");
}

// Test 4: Recursive max on array of 10,000 values 
static void test_large_array(void) {
    int n = 10000;
    float *arr = malloc(sizeof(float) * n);
    assert(arr != NULL);

    for (int i = 0; i < n; i++) {
        arr[i] = (float)i;
    }
    arr[5000] = 99999.0f; // max in the middle 

    check(stats_max(arr, n) == 99999.0f, "recursive max on 10,000 values");
    free(arr);
}

int main(void) {
    printf("=== Running Unit Tests: stats ===\n");
    test_single_value();
    test_max_positions();
    test_threshold_boundary();
    test_large_array();

    printf("\nSummary: %d failures\n", g_failed);
    return g_failed == 0 ? 0 : 1;
}
// BONUS - Test 5: Verify stats_max_tail agrees with stats_max
static void test_max_tail(void) {
    float arr[] = { 10.0f, 45.5f, -3.0f, 99.1f, 12.0f };
    check(stats_max_tail(arr, 5) == stats_max(arr, 5), "stats_max_tail matches stats_max");
}