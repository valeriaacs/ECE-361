#include <stddef.h>
#include <string.h>
#include "lock.h"

/* State transition table definition */
const transition_t lock_table[N_STATES][N_EVENTS] = {
    [LOCKED] = {
        [E_RIGHT]   = { GOT1,   A_NONE },
        [E_WRONG]   = { LOCKED, A_BEEP },
        [E_CLOSE]   = { LOCKED, A_NONE },
        [E_TIMEOUT] = { LOCKED, A_NONE },
    },
    [GOT1] = {
        [E_RIGHT]   = { GOT2,   A_NONE },
        [E_WRONG]   = { LOCKED, A_BEEP },
        [E_CLOSE]   = { GOT1, A_NONE },
        [E_TIMEOUT] = { LOCKED, A_NONE },
    },
    [GOT2] = {
        [E_RIGHT]   = { OPEN,   A_UNLOCK },
        [E_WRONG]   = { LOCKED, A_BEEP },
        [E_CLOSE]   = { GOT2, A_NONE },
        [E_TIMEOUT] = { LOCKED, A_NONE },
    },
    [OPEN] = {
        [E_RIGHT]   = { OPEN,   A_NONE },
        [E_WRONG]   = { OPEN, A_NONE },
        [E_CLOSE]   = { LOCKED, A_LOCK },
        [E_TIMEOUT] = { LOCKED, A_LOCK },
    }
};

state_t lock_step(state_t s, event_t e, action_t *action) {
    /* Validate input state and event bounds */
    if (s < 0 || s >= N_STATES || e < 0 || e >= N_EVENTS) {
        if (action != NULL) {
            *action = A_UNDEFINED;
        }
        return s;
    }

    /* Single table lookup */
    transition_t t = lock_table[s][e];

    /* Store action if pointer is valid */
    if (action != NULL) {
        *action = t.action;
    }

    /* Return next state */
    return t.next;
}

int lock_event(state_t s, char c) {
    /* Handle door close and timer events */
    if (c == 'c' || c == 'C') {
        return E_CLOSE;
    }
    if (c == 't' || c == 'T') {
        return E_TIMEOUT;
    }

    /* Handle digit entries (3-1-4 sequence) */
    if (c >= '0' && c <= '9') {
        /* When door is open, digits do not advance sequence */
        if (s == OPEN) {
            return E_RIGHT; /* Or E_WRONG; both yield A_NONE & stay OPEN */
        }

        /* Check expected digit per state */
        char expected = '\0';
        if (s == LOCKED) expected = '3';
        else if (s == GOT1) expected = '1';
        else if (s == GOT2) expected = '4';

        if (c == expected) {
            return E_RIGHT;
        } else {
            return E_WRONG;
        }
    }

    /* Return -1 for invalid characters */
    return -1;
}

const char *state_name(state_t s) {
    /* Define static lookup table for state names */
    static const char *names[N_STATES] = {
        [LOCKED] = "LOCKED",
        [GOT1]   = "GOT1",
        [GOT2]   = "GOT2",
        [OPEN]   = "OPEN"
    };

    /* Check input bounds */
    if (s < 0 || s >= N_STATES) {
        return "?";
    }

    /*Return string representation */
    return names[s];
}


const char *event_name(event_t e) {
    /* Define static lookup table for event names */
    static const char *names[N_EVENTS] = {
        [E_RIGHT]   = "E_RIGHT",
        [E_WRONG]   = "E_WRONG",
        [E_CLOSE]   = "E_CLOSE",
        [E_TIMEOUT] = "E_TIMEOUT"
    };

    /* Check input bounds */
    if (e < 0 || e >= N_EVENTS) {
        return "?";
    }

    /* Return string representation */
    return names[e];
}

const char *action_name(action_t a) {
    /* Define static lookup table for action names */
    static const char *names[N_ACTIONS] = {
        [A_UNDEFINED] = "A_UNDEFINED",
        [A_NONE]      = "A_NONE",
        [A_UNLOCK]    = "A_UNLOCK",
        [A_LOCK]      = "A_LOCK",
        [A_BEEP]      = "A_BEEP"
    };

    /* Check input bounds */
    if (a < 0 || a >= N_ACTIONS) {
        return "?";
    }

    /* Return corresponding string representation */
    return names[a];
}