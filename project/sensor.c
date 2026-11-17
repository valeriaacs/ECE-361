#include <math.h>
#include <stddef.h>
#include "sensor.h"
#include "iom361_r4.h"

int sensor_read(uint32_t *base, float *temp_c, float *rh_pct) {
    if (temp_c == NULL || rh_pct == NULL) {
        return 1;
    }

    int rc = 0;
    uint32_t raw_temp = iom361_readReg(base, TEMP_REG, &rc);
    if (rc != 0) {
        return rc;
    }

    uint32_t raw_humid = iom361_readReg(base, HUMID_REG, &rc);
    if (rc != 0) {
        return rc;
    }

    /* Mask to 20-bit unsigned values per AHT20 register format */
    uint32_t st = raw_temp & 0xFFFFFu;
    uint32_t srh = raw_humid & 0xFFFFFu;

    float t = ((float)st / 1048576.0f) * 200.0f - 50.0f;
    float h = ((float)srh / 1048576.0f) * 100.0f;

    /* Round to nearest 0.1 */
    *temp_c = roundf(t * 10.0f) / 10.0f;
    *rh_pct = roundf(h * 10.0f) / 10.0f;

    return 0;
}