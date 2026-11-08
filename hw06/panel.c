#include <stddef.h>
#include "panel.h"
#include "iom361_r4.h"

uint32_t panel_pressed(uint32_t now, uint32_t before) {
    return now & (~before);
}

uint32_t panel_released(uint32_t now, uint32_t before) {
    return (~now) & before;
}

int panel_led(uint32_t *base, int n, int on) {
    // Bounds check on LED index n (must be 0..31)
    if (base == NULL || n < 0 || n > 31) {
        return -1;
    }

    int rtn_code = 0;
    uint32_t reg_val = iom361_readReg(base, LEDS_REG, &rtn_code);
    if (rtn_code != 0) return -1;

    // Set or clear bit n
    if (on) {
        reg_val |= (1U << n);
    } else {
        reg_val &= ~(1U << n);
    }

    // Write modified value back to LED_REG
    iom361_writeReg(base, LEDS_REG, reg_val, &rtn_code);
    return (rtn_code == 0) ? 0 : -1;
}

int panel_toggle(uint32_t *base, int n) {
    // Bounds check on LED index n (must be 0..31)
    if (base == NULL || n < 0 || n > 31) {
        return -1;
    }

    int rtn_code = 0;
    uint32_t reg_val = iom361_readReg(base, LEDS_REG, &rtn_code);
    if (rtn_code != 0) return -1;

    // Modify: XOR bit n to flip its state
    reg_val ^= (1U << n);

    // Write modified value back to LED_REG
    iom361_writeReg(base, LEDS_REG, reg_val, &rtn_code);
    return (rtn_code == 0) ? 0 : -1;
}