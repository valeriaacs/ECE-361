#include <stddef.h>
#include "fsm.h"

/*
 * 2D Transition Table indexed by [current_state][event].
 *
 * Design decisions where the specification is silent:
 *  - ARRIVE while SEALED: ignored (shipment has not departed yet).
 *  - DEPART pressed after departure: ignored (already in transit/logged).
 *  - ARRIVE while EXCURSION: transitions to BREACHED (rejected if delivered
 *    while actively out of the 2.0 to 8.0 C safe range).
 *  - ARRIVE while IN_TRANSIT or RECOVERED: stops logging and retains state.
 *  - BREACHED: terminal state; subsequent temperature/timer events do not
 *    clear the rejection, though ARRIVE still stops logging.
 */
static const transition_t transition_table[STATE_COUNT][EVENT_COUNT] = {
    /* Box packed, waiting for DEPART switch press */
    [STATE_SEALED] = {
        [EVENT_DEPART]            = { STATE_IN_TRANSIT, ACTION_START_LOG },
        [EVENT_ARRIVE]            = { STATE_SEALED,     ACTION_NONE },
        [EVENT_TEMP_IN_RANGE]     = { STATE_SEALED,     ACTION_NONE },
        [EVENT_TEMP_OUT_OF_RANGE] = { STATE_SEALED,     ACTION_NONE },
        [EVENT_RECOVERY_COMPLETE] = { STATE_SEALED,     ACTION_NONE },
        [EVENT_BREACH_EXCEEDED]   = { STATE_SEALED,     ACTION_NONE },
    },

    /* Actively shipping and within safe temperature range (2.0 to 8.0 C) */
    [STATE_IN_TRANSIT] = {
        [EVENT_DEPART]            = { STATE_IN_TRANSIT, ACTION_NONE },
        [EVENT_ARRIVE]            = { STATE_IN_TRANSIT, ACTION_STOP_LOG },
        [EVENT_TEMP_IN_RANGE]     = { STATE_IN_TRANSIT, ACTION_NONE },
        [EVENT_TEMP_OUT_OF_RANGE] = { STATE_EXCURSION,  ACTION_INC_EXCURSION },
        [EVENT_RECOVERY_COMPLETE] = { STATE_IN_TRANSIT, ACTION_NONE },
        [EVENT_BREACH_EXCEEDED]   = { STATE_BREACHED,   ACTION_REJECT },
    },

    /* Temperature is outside safe range (< 2.0 C or > 8.0 C) */
    [STATE_EXCURSION] = {
        [EVENT_DEPART]            = { STATE_EXCURSION,  ACTION_NONE },
        [EVENT_ARRIVE]            = { STATE_BREACHED,   ACTION_REJECT },
        [EVENT_TEMP_IN_RANGE]     = { STATE_RECOVERED,  ACTION_RESET_RECOVERY },
        [EVENT_TEMP_OUT_OF_RANGE] = { STATE_EXCURSION,  ACTION_INC_EXCURSION },
        [EVENT_RECOVERY_COMPLETE] = { STATE_EXCURSION,  ACTION_NONE },
        [EVENT_BREACH_EXCEEDED]   = { STATE_BREACHED,   ACTION_REJECT },
    },

    /* Back within 2.0 to 8.0 C after an excursion; counting 60 ticks to recover */
    [STATE_RECOVERED] = {
        [EVENT_DEPART]            = { STATE_RECOVERED,  ACTION_NONE },
        [EVENT_ARRIVE]            = { STATE_RECOVERED,  ACTION_STOP_LOG },
        [EVENT_TEMP_IN_RANGE]     = { STATE_RECOVERED,  ACTION_INC_RECOVERY },
        [EVENT_TEMP_OUT_OF_RANGE] = { STATE_EXCURSION,  ACTION_INC_EXCURSION },
        [EVENT_RECOVERY_COMPLETE] = { STATE_IN_TRANSIT, ACTION_NONE },
        [EVENT_BREACH_EXCEEDED]   = { STATE_BREACHED,   ACTION_REJECT },
    },

    /* Cumulative out-of-range time exceeded 300 ticks; shipment rejected */
    [STATE_BREACHED] = {
        [EVENT_DEPART]            = { STATE_BREACHED,   ACTION_NONE },
        [EVENT_ARRIVE]            = { STATE_BREACHED,   ACTION_STOP_LOG },
        [EVENT_TEMP_IN_RANGE]     = { STATE_BREACHED,   ACTION_NONE },
        [EVENT_TEMP_OUT_OF_RANGE] = { STATE_BREACHED,   ACTION_NONE },
        [EVENT_RECOVERY_COMPLETE] = { STATE_BREACHED,   ACTION_NONE },
        [EVENT_BREACH_EXCEEDED]   = { STATE_BREACHED,   ACTION_NONE },
    },
};

/**
 * Looks up (current, event) in the transition table.
 * Guards against out-of-bounds enum values by returning the current state
 * with ACTION_NONE.
 */
state_t fsm_step(state_t current, event_t event, action_t *action_out) {
    if (current < 0 || current >= STATE_COUNT ||
        event < 0 || event >= EVENT_COUNT) {
        if (action_out != NULL) {
            *action_out = ACTION_NONE;
        }
        return current;
    }

    transition_t t = transition_table[current][event];
    if (action_out != NULL) {
        *action_out = t.action;
    }
    return t.next_state;
}

/**
 * Returns the human-readable string identifier for a state_t value.
 */
const char *fsm_state_name(state_t state) {
    switch (state) {
        case STATE_SEALED:     return "SEALED";
        case STATE_IN_TRANSIT: return "IN_TRANSIT";
        case STATE_EXCURSION:  return "EXCURSION";
        case STATE_RECOVERED:  return "RECOVERED";
        case STATE_BREACHED:   return "BREACHED";
        default:               return "UNKNOWN";
    }
}

/**
 * Returns the human-readable string identifier for an event_t value.
 */
const char *fsm_event_name(event_t event) {
    switch (event) {
        case EVENT_DEPART:            return "DEPART";
        case EVENT_ARRIVE:            return "ARRIVE";
        case EVENT_TEMP_IN_RANGE:     return "TEMP_IN_RANGE";
        case EVENT_TEMP_OUT_OF_RANGE: return "TEMP_OUT_OF_RANGE";
        case EVENT_RECOVERY_COMPLETE: return "RECOVERY_COMPLETE";
        case EVENT_BREACH_EXCEEDED:   return "BREACH_EXCEEDED";
        default:                      return "UNKNOWN";
    }
}