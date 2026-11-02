#include <stdio.h>
#include <stdlib.h>
#include "lock.h"

int main(void) {
    state_t current_state = LOCKED;
    int c;

    /* Read standard input character by character until EOF */
    while ((c = getchar()) != EOF) {
        /* Ignore newlines or whitespace if desired, or pass directly */
        if (c == '\n' || c == '\r') {
            continue;
        }

        /* Convert character into FSM event */
        int evt = lock_event(current_state, (char)c);
        if (evt < 0) {
            /* Skip or ignore invalid non-event characters */
            continue;
        }

        action_t action = A_UNDEFINED;
        state_t next_state = lock_step(current_state, (event_t)evt, &action);

        printf("%s %c %s -> %s %s\n",
               state_name(current_state),
               (char)c,
               event_name((event_t)evt),
               state_name(next_state),
               action_name(action));

        /* Advance to next state */
        current_state = next_state;
    }

    /* Output final state at end of input */
    printf("final: %s\n", state_name(current_state));

    return 0;
}