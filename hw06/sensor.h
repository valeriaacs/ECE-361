#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <time.h>

/**
 * @brief Structure holding a single environmental sensor reading.
 */
typedef struct {
    time_t t;       /**< Timestamp when the reading was taken (seconds since epoch) */
    float temp_c;   /**< Temperature in degrees Celsius (rounded to 0.1 °C) */
    float rh_pct;   /**< Relative humidity in percent (rounded to 0.1 %) */
} reading_t;

/**
 * @brief Reads temperature and humidity registers, converts raw data, and populates a reading_t struct.
 *
 * Communicates with hardware via iom361_read for TEMP_REG and HUMID_REG registers.
 * Extracts bits 19..0 of each raw register, converts them to physical units using
 * iom361_r4.h formulas, rounds values to 0.1 precision, and stores them in *r.
 *
 * @param base Pointer to the base address of the iom361 register bank.
 * @param now Timestamp to assign to the reading.
 * @param r Pointer to the reading_t structure where results are stored.
 *
 * @return 0 on success.
 *         If an iom361 read fails, returns the non-zero iom361 error code
 *         and leaves *r completely unchanged.
 */
int sensor_read(uint32_t *base, time_t now, reading_t *r);

#endif /* SENSOR_H */