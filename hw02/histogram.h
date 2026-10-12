#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "reading.h"

/*
 * Computes 5-degree temperature bins and prints an asterisk histogram to standard output.
 *
 * Parameter readings: Array of sensor readings.
 * Parameter n: Number of readings in the array.
 *
 * Assumes: n >= 0.
 */
void histogram_print(const reading_t readings[], int n);

#endif /* HISTOGRAM_H */