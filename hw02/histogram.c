#include "histogram.h"
#include <stdio.h>

#define MAX_BINS 20

void histogram_print(const reading_t readings[], int n) {
    if (n == 0) {
        return;
    }

    int bins[MAX_BINS] = {0};
    int max_bin_idx = 0;
    int min_bin_idx = MAX_BINS - 1;

    for (int i = 0; i < n; i++) {
        float t = readings[i].temp;
        int bin_idx = (int)(t / 5.0f);
        
        if (bin_idx < 0) {
            bin_idx = 0;
        }
        if (bin_idx >= MAX_BINS) {
            bin_idx = MAX_BINS - 1;
        }

        bins[bin_idx]++;
        if (bin_idx > max_bin_idx) {
            max_bin_idx = bin_idx;
        }
        if (bin_idx < min_bin_idx) {
            min_bin_idx = bin_idx;
        }
    }

    printf("histogram:\n");
    for (int i = min_bin_idx; i <= max_bin_idx; i++) {
        if (bins[i] == 0) {
            continue;
        }

        float range_start = i * 5.0f;
        float range_end = range_start + 5.0f;
        
        printf("%7.1f to %5.1f C | ", range_start, range_end);
        for (int j = 0; j < bins[i]; j++) {
            putchar('*');
        }
        printf(" %d\n", bins[i]);
    }
}