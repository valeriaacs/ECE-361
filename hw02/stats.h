#ifndef STATS_H
#define STATS_H

#include "reading.h"

// Struct holding statistical results for a dataset
typedef struct {
    float min;
    float max;
    float mean;
} stats_result_t;

// Struct holding information on the longest consecutive run above a threshold 
typedef struct {
    int length;
    int start_tick;
    int end_tick;
} stats_run_t;

/*
 * Recursively calculates the maximum value in an array of floats.
 *
 * Param a: Array of float values.
 * Param n: Number of elements in array a.
 *
 * Returns: The maximum value in the array.
 * Assumes: n >= 1.
 */
float stats_max(const float a[], int n);

/*
 * Calculates min, max (using stats_max), and mean for an array of floats.
 *
 * Param a: Array of float values.
 * Param n: Number of elements in array a.
 *
 * Returns: stats_result_t containing min, max, and mean.
 * Assumes: n >= 1.
 */
stats_result_t stats_calculate(const float a[], int n);

/*
 * Finds the longest run of consecutive readings with temperatures strictly above threshold.
 *
 * Param readings: Array of sensor readings.
 * Param n: Number of readings.
 * Param threshold: Cutoff temperature in degrees C.
 *
 * Returns: stats_run_t containing run length and start/end ticks (length = 0 if never above).
 * Assumes: n >= 0.
 */
stats_run_t stats_longest_run(const reading_t readings[], int n, float threshold);

/*
 * Recursively calculates max value using tail recursion.
 *
 * Param a: Array of float values.
 * Param n: Number of elements in array.
 *
 * Returns: Maximum value in array.
 * Assumes: n >= 1.
 */
float stats_max_tail(const float a[], int n);

#endif /* STATS_H */