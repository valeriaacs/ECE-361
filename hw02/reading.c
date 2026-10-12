#include "reading.h"
#include <stdio.h>
#include <string.h>

#define LINE_LEN 128

int reading_read_all(FILE *stream, reading_t readings[], int max_capacity, int *out_skipped) {
    char line[LINE_LEN];
    int count = 0;
    int skipped = 0;

    while (fgets(line, sizeof(line), stream) != NULL) {
        int i = 0;
        /* Skip leading spaces/tabs */
        while (line[i] == ' ' || line[i] == '\t') {
            i++;
        }
        /* Ignore blank lines and comments starting with '#' */
        if (line[i] == '\n' || line[i] == '\0' || line[i] == '#') {
            continue;
        }
        /* Capacity guard */
        if (count == max_capacity) {
            fprintf(stderr, "warning: more than %d readings, the rest are ignored\n", max_capacity);
            break;
        }
        /* Parse tick, temp, and hum into struct array elements */
        if (sscanf(line, "%d %f %f", &readings[count].tick, &readings[count].temp, &readings[count].hum) == 3) { //Pass the address of each individual struct member.
            count++; // so sscanf can store parsed numbers directly into the array struct.
        } else {
            skipped++;
        }
    }

    if (out_skipped != NULL) { // Pass it as a pointer
        *out_skipped = skipped; // Updates the caller's variable 
    }

    return count;
}