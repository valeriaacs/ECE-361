#ifndef FSM_H
#define FSM_H

#include <stdbool.h>
#include <stdint.h>

/* Device states  */
typedef enum {
    STATE_SEALED = 0,
    STATE_IN_TRANSIT,
    STATE_EXCURSION,
    STATE_RECOVERED,
    STATE_BREACHED,
    STATE_COUNT
} state_t;

/* Events the state machine reacts to */
typedef enum {
    EVENT_DEPART = 0,
    EVENT_ARRIVE,
    EVENT_TEMP_IN_RANGE,
    EVENT_TEMP_OUT_OF_RANGE,
    EVENT_RECOVERY_COMPLETE,
    EVENT_BREACH_EXCEEDED,
    EVENT_COUNT
} event_t;

/* Actions triggered by state transitions */
typedef enum {
    ACTION_NONE = 0,
    ACTION_START_LOG,
    ACTION_STOP_LOG,
    ACTION_RESET_RECOVERY,
    ACTION_INC_EXCURSION,
    ACTION_INC_RECOVERY,
    ACTION_REJECT
} action_t;

/* Single cell in the transition table */
typedef struct {
    state_t next_state;
    action_t action;
} transition_t;

/**
 * fsm_step() - looks up (state, event) in the transition table, optionally
 * outputs the associated action via *action_out, and returns the next state.
 */
state_t fsm_step(state_t current, event_t event, action_t *action_out);

/**
 * fsm_state_name() - returns a printable string name for a state.
 */
const char *fsm_state_name(state_t state);

/**
 * fsm_event_name() - returns a printable string name for an event.
 */
const char *fsm_event_name(event_t event);

#endif /* FSM_H */