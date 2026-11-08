#ifndef PANEL_H
#define PANEL_H

#include <stdint.h>

/**
 * @brief Returns a bitmask of switches that went from OFF to ON (rising edge).
 *
 * @param now Current switch bitmask.
 * @param before Previous switch bitmask.
 * @return Bitmask with bits set to 1 for switches pressed in this step.
 */
uint32_t panel_pressed(uint32_t now, uint32_t before);

/**
 * @brief Returns a bitmask of switches that went from ON to OFF (falling edge).
 *
 * @param now Current switch bitmask.
 * @param before Previous switch bitmask.
 * @return Bitmask with bits set to 1 for switches released in this step.
 */
uint32_t panel_released(uint32_t now, uint32_t before);

/**
 * @brief Sets or clears a single LED using a Read-Modify-Write cycle.
 *
 * @param base Base address pointer to iom361 register bank.
 * @param n LED index (0 to 31).
 * @param on Non-zero to turn LED on, zero to turn LED off.
 * @return 0 on success, or -1 if n is out of range (not 0..31).
 */
int panel_led(uint32_t *base, int n, int on);

/**
 * @brief Toggles a single LED using a Read-Modify-Write cycle.
 *
 * @param base Base address pointer to iom361 register bank.
 * @param n LED index (0 to 31).
 * @return 0 on success, or -1 if n is out of range (not 0..31).
 */
int panel_toggle(uint32_t *base, int n);

#endif /* PANEL_H */