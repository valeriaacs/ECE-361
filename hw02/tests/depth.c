#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../stats.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <standard|tail> <n>\n", argv[0]);
        return 1;
    }

    const char *type = argv[1];
    int n = atoi(argv[2]);

    float *arr = malloc(sizeof(float) * n);
    if (!arr) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = (float)i;
    }

    float result = 0.0f;
    if (strcmp(type, "tail") == 0) {
        result = stats_max_tail(arr, n);
    } else {
        result = stats_max(arr, n);
    }

    printf("Successfully calculated max: %.1f for n = %d\n", result, n);
    free(arr);
    return 0;
}