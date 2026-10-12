#include "stats.h"

// Recursive max implementation
float stats_max(const float a[], int n) {
    if (n == 1) {
        return a[0];
    }
    float rest = stats_max(a + 1, n - 1);
    return a[0] > rest ? a[0] : rest;
}

stats_result_t stats_calculate(const float a[], int n) {
    stats_result_t res;
    res.min = a[0];
    res.max = stats_max(a, n);

    float sum = 0.0f;
    for (int i = 0; i < n; i++) {
        if (a[i] < res.min) {
            res.min = a[i];
        }
        sum += a[i];
    }
    res.mean = sum / n;

    return res;
}

stats_run_t stats_longest_run(const reading_t readings[], int n, float threshold) {
    stats_run_t result = {0, -1, -1};
    int run = 0;
    int best = 0;
    int best_start = -1;
    int start = 0;

    for (int i = 0; i < n; i++) {
        if (readings[i].temp > threshold) {
            if (run == 0) {
                start = i;
            }
            run++;
            if (run > best) {
                best = run;
                best_start = start;
            }
        } else {
            run = 0;
        }
    }

    result.length = best;
    if (best > 0) {
        result.start_tick = readings[best_start].tick;
        result.end_tick = readings[best_start + best - 1].tick;
    }

    return result;
}

static float stats_max_tail_helper(const float a[], int n, float current_max) {
    if (n == 0) {
        return current_max;
    }
    float new_max = (a[0] > current_max) ? a[0] : current_max;
    return stats_max_tail_helper(a + 1, n - 1, new_max);
}

float stats_max_tail(const float a[], int n) {
    if (n <= 0) return 0.0f;
    return stats_max_tail_helper(a + 1, n - 1, a[0]);
}