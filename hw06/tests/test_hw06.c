#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  
#include <time.h>    
#include <math.h>
/* Relative paths to project headers from tests/ directory */
#include "../iom361_r4.h"
#include "../sensor.h"
#include "../panel.h"
#include "../scenario.h"

static int g_failed = 0;

#define RUN_TEST(fn) do { \
    printf("Running %s... ", #fn); \
    if (fn()) { \
        printf("PASS\n"); \
    } else { \
        printf("FAIL\n"); \
        g_failed = 1; \
    } \
} while(0)

// 1. Test sensor read of 91.0 C
static int test_sensor_read_val(void) {
    uint32_t *base = iom361_initialize(4, 4, 0);
    _iom361_set_sensor1(91.0f, 50.0f);
    
    reading_t r;
    int status = sensor_read(base, 1000, &r);
    if (status != 0) return 0;
    if (fabs(r.temp_c - 91.0f) > 0.05f) return 0;
    return 1;
}

// 2. Test failed read changes nothing in *r
static int test_sensor_fail_guard(void) {
    uint32_t *base = iom361_initialize(4, 4, NULL);
    (void)base; // Supress unused variable warning
    reading_t r = { .t = 1234, .temp_c = 25.0f, .rh_pct = 40.0f };
    
    // Pass NULL base address to trigger a failed read
    int status = sensor_read(NULL, 5000, &r);
    if (status == 0) return 0; // Should fail
    if (r.t != 1234 || r.temp_c != 25.0f || r.rh_pct != 40.0f) return 0; // Must be unchanged
    return 1;
}

// 3. Test switch edge detection (pressed, released, held)
static int test_panel_edges(void) {
    uint32_t before = 0x01; // Bit 0 was ON
    uint32_t now    = 0x02; // Bit 0 OFF, Bit 1 ON

    uint32_t pressed  = panel_pressed(now, before);   // Bit 1 pressed
    uint32_t released = panel_released(now, before);  // Bit 0 released

    if (pressed != 0x02) return 0;
    if (released != 0x01) return 0;

    // Held switch test: Bit 1 remains ON
    uint32_t next_now = 0x02;
    if (panel_pressed(next_now, now) != 0x00) return 0; // No new press
    if (panel_released(next_now, now) != 0x00) return 0; // No new release

    return 1;
}

// 4. Test panel_led and panel_toggle preserve other LEDs
static int test_panel_preserves_leds(void) {
    uint32_t *base = iom361_initialize(4, 4, 0);
    
    // Set LED 1 ON (0x02)
    panel_led(base, 1, 1);
    
    // Set LED 3 ON, ensure LED 1 is preserved
    panel_led(base, 3, 1);
    uint32_t leds = 0;
    int status = 0;
    leds = iom361_readReg(base, LEDS_REG, &status);
    if (leds != 0x0A) return 0; // Bits 1 and 3 ON (0x2 | 0x8 = 0xA)

    // Toggle LED 0 ON, ensure LED 1 and 3 are preserved
    panel_toggle(base, 0);
    leds = iom361_readReg(base, LEDS_REG, &status);
    if (leds != 0x0B) return 0; // Bits 0, 1, 3 ON (0xA | 0x1 = 0xB)

    return 1;
}

// 5. Test scenario loading comments, blank lines, and holding values
static int test_scenario_load_valid(void) {
    scenario_t sc;
    
    // Try primary path first, fall back to relative parent path if needed
    int err = scenario_load("scenarios/valid.txt", &sc);
    if (err != 0) {
        err = scenario_load("../scenarios/valid.txt", &sc);
    }
    
    if (err != 0) return 0;
    if (sc.count < 2) return 0;

    // Verify values hold forward between ticks
    const scenario_entry_t *e = scenario_get(&sc, 3); // Tick 3 (between tick 0 and 5)
    if (e == NULL || e->tick != 0) return 0; // Should return entry from tick 0

    return 1;
}

// 6. Test scenario load errors (missing start line, missing field, non-increasing ticks)
static int test_scenario_load_errors(void) {
    scenario_t sc;

    // Test missing start line
    if (scenario_load("tests/bad_no_start.txt", &sc) == 0) return 0;

    // Test missing field
    if (scenario_load("tests/bad_missing_field.txt", &sc) == 0) return 0;

    // Test non-increasing ticks
    if (scenario_load("tests/bad_ticks.txt", &sc) == 0) return 0;

    return 1;
}

int main(void) {
    RUN_TEST(test_sensor_read_val);
    RUN_TEST(test_sensor_fail_guard);
    RUN_TEST(test_panel_edges);
    RUN_TEST(test_panel_preserves_leds);
    RUN_TEST(test_scenario_load_valid);
    RUN_TEST(test_scenario_load_errors);

    return g_failed ? 1 : 0;
}