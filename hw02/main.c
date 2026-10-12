#include <stdio.h>
#include <stdlib.h>
#include "reading.h"
#include "stats.h"
#include "histogram.h"

int main(int argc, char *argv[]) {
    float threshold = 30.0f; // Set defaul temperature threshold(30 C)

    if (argc > 2) {
        fprintf(stderr, "usage: %s [threshold] < readings.txt\n", argv[0]);
        return 1;
    }
    if (argc == 2) {
        char *end;
        threshold = strtof(argv[1], &end);
        if (*end != '\0') {
            fprintf(stderr, "error: threshold '%s' is not a number\n", argv[1]);
            return 1;
        }
    }

    //  Calling the reading module
    reading_t readings[MAX_READINGS]; // Allocates an array of reading_t structs on the stack.
    int skipped = 0;
    int count = reading_read_all(stdin, readings, MAX_READINGS, &skipped); // Returns the total number of valid readings.

    printf("readings: %d\n", count); 
    printf("skipped:  %d\n", skipped);
    if (count == 0) { // Handles empty files.
        printf("no readings, no summary\n");
        return 0;
    }

    // Extracting float arrays for stats processing
float temps[MAX_READINGS];
    float hums[MAX_READINGS];
    for (int i = 0; i < count; i++) {
        temps[i] = readings[i].temp;
        hums[i] = readings[i].hum;
    }

    // Compute & print temperature stats
    stats_result_t tstats = stats_calculate(temps, count);
    printf("temperature: min %6.1f  max %6.1f  mean %6.2f C\n", 
           tstats.min, tstats.max, tstats.mean);

    // Compute & print humidity stats
    stats_result_t hstats = stats_calculate(hums, count);
    printf("humidity:    min %6.1f  max %6.1f  mean %6.2f %%RH\n", 
           hstats.min, hstats.max, hstats.mean);

    // Longest run above threshold
    stats_run_t run_info = stats_longest_run(readings, count, threshold);
    if (run_info.length == 0) {
        printf("above %.1f C: never\n", threshold);
    } else {
        printf("above %.1f C: longest run %d readings, from tick %d to tick %d\n",
               threshold, run_info.length, run_info.start_tick, run_info.end_tick);
    }

    histogram_print(readings, count);

    return 0;
}
