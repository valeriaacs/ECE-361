#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "iom361_r4.h"
#include "sensor.h"
#include "panel.h"
#include "scenario.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <scenario_file>\n", argv[0]);
        return 1;
    }

    scenario_t sc;
    int err_line = scenario_load(argv[1], &sc);
    if (err_line != 0) {
        fprintf(stderr, "Error loading scenario file on line %d\n", err_line);
        return 1;
    }

    // Initialize hardware: 4 switches (0xF), 4 LEDs (0xF), display OFF (0)
    uint32_t *base = iom361_initialize(0xF, 0xF, 0);
    if (base == NULL) {
        fprintf(stderr, "Failed to initialize iom361 hardware.\n");
        return 1;
    }

    uint32_t last_switches = 0;
    uint32_t max_tick = sc.entries[sc.count - 1].tick;

    for (uint32_t tick = 0; tick <= max_tick; tick++) {
        const scenario_entry_t *cur = scenario_get(&sc, tick);

        // Set world simulation state
        _iom361_set_sensor1(cur->temp_c, cur->rh_pct);
        _iom361_set_switches(cur->switches);

        // Read current hardware values via registers
        time_t now = sc.start_time + tick;
        reading_t r;
        sensor_read(base, now, &r);

        int status = 0;
        uint32_t raw_switches = iom361_readReg(base, SWITCHES_REG, &status);

        // Detect newly pressed switches and toggle corresponding LEDs
        uint32_t pressed = panel_pressed(raw_switches, last_switches);
        for (int k = 0; k < 32; k++) {
            if (pressed & (1U << k)) {
                panel_toggle(base, k);
            }
        }
        last_switches = raw_switches;

        // Read final LED state for trace logging
        uint32_t leds = iom361_readReg(base, LEDS_REG, &status);

        // Format UTC timestamp
        struct tm *utc_time = gmtime(&r.t);
        char time_str[64];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", utc_time);

        // Print trace output matching required format
        printf("%s temp=%.1f rh=%.1f switches=0x%x pressed=0x%x leds=0x%x\n",
               time_str, r.temp_c, r.rh_pct, raw_switches, pressed, leds);
    }

    return 0;
}