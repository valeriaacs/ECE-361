#include "tally.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *in = stdin;

    /* File opening if argument provided */
    if (argc > 1) {
        in = fopen(argv[1], "r");
        if (!in) {
            perror("Error opening input file");
            return 1;
        }
    }

    tally_t *t = tally_create();
    if (!t) {
        fprintf(stderr, "Error: Could not allocate tally instance.\n");
        if (in != stdin) fclose(in);
        return 1;
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), in)) {
        /* Strip trailing newline */
        buffer[strcspn(buffer, "\r\n")] = '\0';

        /* Ignore empty lines */
        if (strlen(buffer) == 0) {
            continue;
        }

        /* Process name */
        if (tally_add(t, buffer) == -1) {
            /* String too long or tally full */
            fprintf(stderr, "Warning: Failed to tally event '%s'\n", buffer);
        }
    }

    /* Print results to stdout */
    tally_print(t, stdout);

    /* Cleanup resources */
    tally_destroy(t);
    if (in != stdin) {
        fclose(in);
    }

    return 0;
}
