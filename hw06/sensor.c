#include <stddef.h>
#include <math.h>
#include "sensor.h"
#include "iom361_r4.h"

int sensor_read(uint32_t *base, time_t t, reading_t *r) {
    if (base == NULL || r == NULL) {
        return -1; // Return error on invalid pointer arguments
    }

    // Read raw Temperature Register
    int status_temp = 0;
    uint32_t raw_temp = iom361_readReg(base, TEMP_REG, &status_temp);
    if (status_temp != 0) {
        return status_temp; // Return failed iom361 code; change nothing in *r
    }

    // Read raw Humidity Register
    int status_humid = 0;
    uint32_t raw_humid = iom361_readReg(base, HUMID_REG, &status_humid);
    if (status_humid != 0) {
        return status_humid; // Return failed iom361 code; change nothing in *r
    }

    // Convert raw 20-bit sensor values to degrees C and % RH using iom361 formulas
    float temp_c = ((float)(raw_temp & 0xFFFFF) / 1048576.0f) * 200.0f - 50.0f;
    float calc_temp = roundf(temp_c * 10.0f) / 10.0f;

    float humid_pct = ((float)(raw_humid & 0xFFFFF) / 1048576.0f) * 100.0f;
    float calc_humid = roundf(humid_pct * 10.0f) / 10.0f;

    // Assign values to reading destination struct
    r->t = t;
    r->temp_c = calc_temp;
    r->rh_pct = calc_humid;

    return 0; // Success
}