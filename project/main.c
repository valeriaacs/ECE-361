#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "iom361_r4.h"
#include "sensor.h"
#include "fsm.h"
#include "store.h"
#include "aux.h"
#include "report.h"

/* Switch and LED bit masks */
#define SW_DEPART_MASK       (1u << 0)
#define SW_ARRIVE_MASK       (1u << 1)

#define LED_IN_RANGE_MASK    (1u << 0)
#define LED_EXCURSION_MASK   (1u << 1)
#define LED_REJECT_MASK      (1u << 2)

/* Safe temperature bounds in degrees C */
#define TEMP_MIN_SAFE        2.0f
#define TEMP_MAX_SAFE        8.0f

/* Operating rule thresholds (in ticks / seconds) */
#define RECOVERY_TICKS_REQ   60
#define BREACH_TICKS_MAX     300

/* Status word bit layout:
 *  bits [2:0] : state (3 bits)
 *  bit  [3]   : IN_RANGE (1 bit)
 *  bit  [4]   : EXCURSION_LAMP (1 bit)
 *  bit  [5]   : REJECT (1 bit)
 *  bit  [6]   : shipment arrived (1 bit)
 */
static uint32_t pack_status_word(state_t state, uint32_t leds, bool arrived) {
    uint32_t status = 0;
    status |= ((uint32_t)state & 0x7u);
    if (leds & LED_IN_RANGE_MASK)  status |= (1u << 3);
    if (leds & LED_EXCURSION_MASK) status |= (1u << 4);
    if (leds & LED_REJECT_MASK)    status |= (1u << 5);
    if (arrived)                   status |= (1u << 6);
    return status;
}

/* Distinct RGB LED color per state */
static uint32_t rgb_for_state(state_t state) {
    switch (state) {
        case STATE_SEALED:     return build_rgb_reg(1, 0,   0,   255); /* Blue */
        case STATE_IN_TRANSIT: return build_rgb_reg(1, 0,   255, 0);   /* Green */
        case STATE_EXCURSION:  return build_rgb_reg(1, 255, 165, 0);   /* Amber */
        case STATE_RECOVERED:  return build_rgb_reg(1, 0,   255, 255); /* Cyan */
        case STATE_BREACHED:   return build_rgb_reg(1, 255, 0,   0);   /* Red */
        default:               return build_rgb_reg(0, 0,   0,   0);
    }
}

/* Single scheduled change from a scenario file */
typedef struct {
    long tick;
    float temp_c;
    float rh_pct;
    uint32_t switches;
} scenario_step_t;

typedef struct {
    time_t start_time;
    scenario_step_t *steps;
    size_t count;
    size_t capacity;
} scenario_t;

static void scenario_free(scenario_t *sc) {
    if (sc != NULL) {
        free(sc->steps);
        sc->steps = NULL;
        sc->count = 0;
        sc->capacity = 0;
    }
}

static bool scenario_load(const char *path, scenario_t *sc) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        perror("fopen");
        return false;
    }

    memset(sc, 0, sizeof(*sc));
    bool got_start = false;
    char line[256];

    while (fgets(line, sizeof(line), fp) != NULL) {
        /* Strip inline comments */
        char *hash = strchr(line, '#');
        if (hash != NULL) {
            *hash = '\0';
        }

        /* Skip leading whitespace */
        char *p = line;
        while (*p != '\0' && isspace((unsigned char)*p)) {
            p++;
        }
        if (*p == '\0') {
            continue;
        }

        if (!got_start) {
            long long ts = 0;
            if (sscanf(p, "start %lld", &ts) == 1) {
                sc->start_time = (time_t)ts;
                got_start = true;
                continue;
            } else {
                fprintf(stderr, "Expected 'start <timestamp>' first in %s\n", path);
                fclose(fp);
                scenario_free(sc);
                return false;
            }
        }

        long tick = 0;
        float temp = 0.0f;
        float rh = 0.0f;
        unsigned int sw = 0;
        if (sscanf(p, "%ld %f %f %x", &tick, &temp, &rh, &sw) == 4) {
            if (sc->count == sc->capacity) {
                size_t new_cap = (sc->capacity == 0) ? 16 : sc->capacity * 2;
                scenario_step_t *tmp = realloc(sc->steps, new_cap * sizeof(*tmp));
                if (tmp == NULL) {
                    fclose(fp);
                    scenario_free(sc);
                    return false;
                }
                sc->steps = tmp;
                sc->capacity = new_cap;
            }
            sc->steps[sc->count].tick = tick;
            sc->steps[sc->count].temp_c = temp;
            sc->steps[sc->count].rh_pct = rh;
            sc->steps[sc->count].switches = (uint32_t)sw;
            sc->count++;
        }
    }

    fclose(fp);
    if (!got_start || sc->count == 0) {
        fprintf(stderr, "Scenario file %s is empty or missing start time\n", path);
        scenario_free(sc);
        return false;
    }
    return true;
}

static void format_utc_timestamp(time_t ts, char *buf, size_t len) {
    struct tm *utc = gmtime(&ts);
    if (utc != NULL) {
        strftime(buf, len, "%Y-%m-%d %H:%M:%S", utc);
    } else {
        snprintf(buf, len, "0000-00-00 00:00:00");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <scenario.txt> [--report]\n", argv[0]);
        return 1;
    }

    const char *scenario_path = argv[1];
    bool report_only = (argc >= 3 && strcmp(argv[2], "--report") == 0);

    scenario_t sc;
    if (!scenario_load(scenario_path, &sc)) {
        return 1;
    }

    int rc = 0;
    uint32_t *base = iom361_initialize(2, 3, &rc);
    if (base == NULL || rc != 0) {
        fprintf(stderr, "iom361_initialize failed: %d\n", rc);
        scenario_free(&sc);
        return 1;
    }

    /* Suppress emulator LED printing so stdout matches trace format cleanly */
    iom361_set_display(false);

    store_t *store = store_create();
    if (store == NULL) {
        fprintf(stderr, "Failed to allocate store\n");
        scenario_free(&sc);
        return 1;
    }

    /* Device state & counters */
    state_t state = STATE_SEALED;
    uint32_t prev_sw = 0;
    bool logging = false;
    bool arrived = false;
    int total_excursion_ticks = 0;
    int recovery_ticks = 0;

    float cur_temp = sc.steps[0].temp_c;
    float cur_rh = sc.steps[0].rh_pct;
    uint32_t cur_sw = sc.steps[0].switches;

    long max_tick = sc.steps[sc.count - 1].tick;
    size_t step_idx = 0;

    for (long tick = 0; tick <= max_tick; tick++) {
        /* Apply any scheduled world changes at this tick */
        while (step_idx < sc.count && sc.steps[step_idx].tick == tick) {
            cur_temp = sc.steps[step_idx].temp_c;
            cur_rh = sc.steps[step_idx].rh_pct;
            cur_sw = sc.steps[step_idx].switches;
            step_idx++;
        }

        _iom361_set_sensor1(cur_temp, cur_rh);
        _iom361_set_switches(cur_sw);

        /* Read hardware registers */
        reading_t reading;
        reading.timestamp = sc.start_time + (time_t)tick;
        if (sensor_read(base, &reading.temp_c, &reading.rh_pct) != 0) {
            fprintf(stderr, "sensor_read failed at tick %ld\n", tick);
            break;
        }

        uint32_t sw = iom361_readReg(base, SWITCHES_REG, &rc);
        uint32_t rising = sw & ~prev_sw;
        prev_sw = sw;

        action_t action = ACTION_NONE;

        /* 1. Switch press events (rising edges) */
        if (rising & SW_DEPART_MASK) {
            state = fsm_step(state, EVENT_DEPART, &action);
            if (action == ACTION_START_LOG) {
                logging = true;
            }
        }

        if (rising & SW_ARRIVE_MASK) {
            state = fsm_step(state, EVENT_ARRIVE, &action);
            if (action == ACTION_STOP_LOG) {
                logging = false;
                arrived = true;
            } else if (action == ACTION_REJECT) {
                logging = false;
                arrived = true;
            }
        }

        /* 2. Temperature & duration rules while actively logging */
        bool in_range = (reading.temp_c >= TEMP_MIN_SAFE &&
                         reading.temp_c <= TEMP_MAX_SAFE);

        if (logging && !arrived) {
            event_t temp_ev = in_range ? EVENT_TEMP_IN_RANGE : EVENT_TEMP_OUT_OF_RANGE;
            state = fsm_step(state, temp_ev, &action);

            if (action == ACTION_INC_EXCURSION) {
                total_excursion_ticks++;
                recovery_ticks = 0;
                if (total_excursion_ticks > BREACH_TICKS_MAX) {
                    state = fsm_step(state, EVENT_BREACH_EXCEEDED, &action);
                }
            } else if (action == ACTION_RESET_RECOVERY) {
                recovery_ticks = 1;
                if (recovery_ticks >= RECOVERY_TICKS_REQ) {
                    state = fsm_step(state, EVENT_RECOVERY_COMPLETE, &action);
                }
            } else if (action == ACTION_INC_RECOVERY) {
                recovery_ticks++;
                if (recovery_ticks >= RECOVERY_TICKS_REQ) {
                    state = fsm_step(state, EVENT_RECOVERY_COMPLETE, &action);
                }
            }
        }

        /* 3. Update LEDs and RGB LED */
        uint32_t leds = 0;
        if (in_range) {
            leds |= LED_IN_RANGE_MASK;
        } else {
            leds |= LED_EXCURSION_MASK;
        }
        if (state == STATE_BREACHED) {
            leds |= LED_REJECT_MASK;
        }

        iom361_writeReg(base, LEDS_REG, leds, &rc);
        iom361_writeReg(base, RGB_LED_REG, rgb_for_state(state), &rc);

        /* 4. Pack status word and store record */
        uint32_t status = pack_status_word(state, leds, arrived);
        record_t rec = {
            .reading = reading,
            .state = (int)state,
            .status = status
        };
        store_insert(store, &rec);

        /* 5. Print trace line if not in --report mode */
        if (!report_only) {
            char ts_buf[32];
            format_utc_timestamp(reading.timestamp, ts_buf, sizeof(ts_buf));
            printf("%s state=%s temp=%.1f rh=%.1f leds=0x%X status=0x%08X\n",
                   ts_buf,
                   fsm_state_name(state),
                   reading.temp_c,
                   reading.rh_pct,
                   (unsigned)leds,
                   (unsigned)status);
        }

        /* Print verdict when ARRIVE occurs */
        if (arrived && !report_only) {
            printf("VERDICT: %s\n", (state == STATE_BREACHED) ? "REJECTED" : "ACCEPTED");
            break;
        }
    }

    store_destroy(store);
    scenario_free(&sc);
    return 0;
}