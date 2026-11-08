#ifndef SCENARIO_H
#define SCENARIO_H

#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define MAX_SCENARIO_LINES 1000

typedef struct {
    uint32_t tick;
    float temp_c;
    float rh_pct;
    uint32_t switches;
} scenario_entry_t;

typedef struct {
    time_t start_time;
    int count;
    scenario_entry_t entries[MAX_SCENARIO_LINES];
} scenario_t;

/**
 * Parses a scenario file into a scenario_t struct.
 * @param filename File path to scenario.
 * @param sc Pointer to destination scenario_t struct.
 * @return 0 on success, or the 1-based line number of the first malformed line.
 */
int scenario_load(const char *filename, scenario_t *sc);

/**
 * Returns the scenario entry in force for a given tick.
 * Values hold forward between defined ticks.
 */
const scenario_entry_t *scenario_get(const scenario_t *sc, uint32_t tick);

#endif /* SCENARIO_H */