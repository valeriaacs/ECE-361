#ifndef READING_H
#define READING_H

#include <stdio.h>

#define MAX_READINGS 1000 //Act as a global constant for max array capacity.

// Struct representing a single sensor reading
typedef struct {
    int tick; //Holds the time for a reading
    float temp; // Holds the recorded temperature in celsius
    float hum; // Holds the recorded relative humidity percentage
} reading_t;

/*
 * Reads sensor readings from input stream until EOF or max_capacity is reached.
 * Skips blank lines, comments starting with '#', and counts malformed lines.
 *
 * Param stream: Input file stream (e.g., stdin).
 * Param readings: Destination array for parsed reading structures.
 * Param max_capacity: Maximum number of elements readings array can hold.
 * Param out_skipped: Pointer to int receiving count of skipped malformed lines.
 *
 * Returns: Number of valid readings successfully parsed and stored.
 * Assumes: readings and out_skipped are non-NULL pointers; max_capacity > 0.
 */
int reading_read_all(FILE *stream, reading_t readings[], int max_capacity, int *out_skipped);

#endif /* READING_H */