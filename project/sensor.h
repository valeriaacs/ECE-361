#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <time.h>

/* A single timestamped sensor measurement */
typedef struct {
    time_t timestamp;
    float temp_c;
    float rh_pct;
} reading_t;

/**
 * sensor_read() - reads TEMP_REG and HUMID_REG from iom361, converts them
 * to degrees C and %RH rounded to 0.1, and stores them in *temp_c and *rh_pct.
 *
 * Returns 0 on success, or the iom361 return code on failure (changing nothing).
 */
int sensor_read(uint32_t *base, float *temp_c, float *rh_pct);

#endif /* SENSOR_H */