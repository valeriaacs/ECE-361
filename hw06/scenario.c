#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scenario.h"

int scenario_load(const char *filename, scenario_t *sc) {
    if (filename == NULL || sc == NULL) {
        return 1;
    }

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return 1; // Line 1 error if file cannot be opened
    }

    sc->start_time = 0;
    sc->count = 0;

    char line[256];
    int line_num = 0;
    int header_found = 0;
    uint32_t prev_tick = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        line_num++;

        // Skip comments (#) and empty lines
        char *ptr = line;
        while (*ptr == ' ' || *ptr == '\t') ptr++;
        if (*ptr == '#' || *ptr == '\n' || *ptr == '\r' || *ptr == '\0') {
            continue;
        }

        // Parse 'start <timestamp>' header
        if (!header_found) {
            long long ts;
            if (sscanf(ptr, "start %lld", &ts) == 1) {
                sc->start_time = (time_t)ts;
                header_found = 1;
                continue;
            } else {
                fclose(fp);
                return line_num; // Malformed header
            }
        }

        // Prevent exceeding maximum line capacity
        if (sc->count >= MAX_SCENARIO_LINES) {
            fclose(fp);
            return line_num;
        }

        // Parse entry: tick temp rh switches (hex)
        scenario_entry_t entry;
        int parsed = sscanf(ptr, "%u %f %f %x", &entry.tick, &entry.temp_c, &entry.rh_pct, &entry.switches);
        if (parsed != 4) {
            fclose(fp);
            return line_num; // Format error
        }

        // Verify strictly increasing ticks
        if (sc->count > 0 && entry.tick <= prev_tick) {
            fclose(fp);
            return line_num; // Non-increasing tick error
        }

        prev_tick = entry.tick;
        sc->entries[sc->count++] = entry;
    }

    fclose(fp);

    // File must contain at least a header and 1 entry
    if (!header_found || sc->count == 0) {
        return line_num == 0 ? 1 : line_num;
    }

    return 0; // Success
}

const scenario_entry_t* scenario_get(const scenario_t *sc, uint32_t tick) {
    if (sc == NULL || sc->count <= 0) {
        return NULL;
    }

    const scenario_entry_t *best_match = NULL;

    for (int i = 0; i < sc->count; i++) {
        if (sc->entries[i].tick <= tick) {
            best_match = &sc->entries[i];
        } else {
            break;
        }
    }

    return best_match;
}