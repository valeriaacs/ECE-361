#include <stdio.h>
#include <stdlib.h>
#include "lock.h"

static int g_failed = 0;

/* Helper macro for running assertions and printing PASS/FAIL */
#define ASSERT_TEST(cond, name) \
    do { \
        if (cond) { \
            printf("[PASS] %s\n", name); \
        } else { \
            printf("[FAIL] %s\n", name); \
            g_failed++; \
        } \
    } while(0)

/* Test 1: Verify all 16 entries in lock_table */
void test_table_entries(void) {
    action_t act;
    
    /* LOCKED row */
    ASSERT_TEST(lock_step(LOCKED, E_RIGHT, &act) == GOT1 && act == A_NONE, "Table: LOCKED + E_RIGHT");
    ASSERT_TEST(lock_step(LOCKED, E_WRONG, &act) == LOCKED && act == A_BEEP, "Table: LOCKED + E_WRONG");
    ASSERT_TEST(lock_step(LOCKED, E_CLOSE, &act) == LOCKED && act == A_NONE, "Table: LOCKED + E_CLOSE");
    ASSERT_TEST(lock_step(LOCKED, E_TIMEOUT, &act) == LOCKED && act == A_NONE, "Table: LOCKED + E_TIMEOUT");

    /* GOT1 row */
    ASSERT_TEST(lock_step(GOT1, E_RIGHT, &act) == GOT2 && act == A_NONE, "Table: GOT1 + E_RIGHT");
    ASSERT_TEST(lock_step(GOT1, E_WRONG, &act) == LOCKED && act == A_BEEP, "Table: GOT1 + E_WRONG");
    ASSERT_TEST(lock_step(GOT1, E_CLOSE, &act) == GOT1 && act == A_NONE, "Table: GOT1 + E_CLOSE");
    ASSERT_TEST(lock_step(GOT1, E_TIMEOUT, &act) == LOCKED && act == A_NONE, "Table: GOT1 + E_TIMEOUT");

    /* GOT2 row */
    ASSERT_TEST(lock_step(GOT2, E_RIGHT, &act) == OPEN && act == A_UNLOCK, "Table: GOT2 + E_RIGHT");
    ASSERT_TEST(lock_step(GOT2, E_WRONG, &act) == LOCKED && act == A_BEEP, "Table: GOT2 + E_WRONG");
    ASSERT_TEST(lock_step(GOT2, E_CLOSE, &act) == GOT2 && act == A_NONE, "Table: GOT2 + E_CLOSE");
    ASSERT_TEST(lock_step(GOT2, E_TIMEOUT, &act) == LOCKED && act == A_NONE, "Table: GOT2 + E_TIMEOUT");

    /* OPEN row */
    ASSERT_TEST(lock_step(OPEN, E_RIGHT, &act) == OPEN && act == A_NONE, "Table: OPEN + E_RIGHT");
    ASSERT_TEST(lock_step(OPEN, E_WRONG, &act) == OPEN && act == A_NONE, "Table: OPEN + E_WRONG");
    ASSERT_TEST(lock_step(OPEN, E_CLOSE, &act) == LOCKED && act == A_LOCK, "Table: OPEN + E_CLOSE");
    ASSERT_TEST(lock_step(OPEN, E_TIMEOUT, &act) == LOCKED && act == A_LOCK, "Table: OPEN + E_TIMEOUT");
}

/* Test 2: Verify character mapping in lock_event() */
void test_lock_events(void) {
    /* Right and wrong digits in each state */
    ASSERT_TEST(lock_event(LOCKED, '3') == E_RIGHT, "Event: LOCKED + '3'");
    ASSERT_TEST(lock_event(LOCKED, '9') == E_WRONG, "Event: LOCKED + '9'");

    ASSERT_TEST(lock_event(GOT1, '1') == E_RIGHT, "Event: GOT1 + '1'");
    ASSERT_TEST(lock_event(GOT1, '5') == E_WRONG, "Event: GOT1 + '5'");

    ASSERT_TEST(lock_event(GOT2, '4') == E_RIGHT, "Event: GOT2 + '4'");
    ASSERT_TEST(lock_event(GOT2, '2') == E_WRONG, "Event: GOT2 + '2'");

    /* Digits while OPEN */
    ASSERT_TEST(lock_event(OPEN, '3') >= 0, "Event: OPEN + digit '3'");
    ASSERT_TEST(lock_event(OPEN, '7') >= 0, "Event: OPEN + digit '7'");

    /* Control & invalid characters */
    ASSERT_TEST(lock_event(LOCKED, 'c') == E_CLOSE, "Event: 'c' -> E_CLOSE");
    ASSERT_TEST(lock_event(LOCKED, 't') == E_TIMEOUT, "Event: 't' -> E_TIMEOUT");
    ASSERT_TEST(lock_event(LOCKED, 'x') == -1, "Event: invalid 'x' -> -1");
}

/* Helper function to simulate driving an input string through the FSM */
static state_t run_sequence(const char *seq) {
    state_t s = LOCKED;
    for (int i = 0; seq[i] != '\0'; i++) {
        int evt = lock_event(s, seq[i]);
        if (evt >= 0) {
            action_t act;
            s = lock_step(s, (event_t)evt, &act);
        }
    }
    return s;
}

/* Test 3: Required combination sequences */
void test_sequences(void) {
    ASSERT_TEST(run_sequence("314") == OPEN, "Seq: '314' opens lock");
    ASSERT_TEST(run_sequence("3314") != OPEN, "Seq: '3314' fails to open");
    ASSERT_TEST(run_sequence("31t4") != OPEN, "Seq: '31t4' fails to open");
    ASSERT_TEST(run_sequence("314c") == LOCKED, "Seq: '314c' opens then locks again");
}

int main(void) {
    printf("--- Running Lock FSM Tests ---\n");
    test_table_entries();
    test_lock_events();
    test_sequences();

    printf("\n--- Test Summary ---\n");
    if (g_failed == 0) {
        printf("ALL TESTS PASSED!\n");
        return 0;
    } else {
        printf("%d TEST(S) FAILED.\n", g_failed);
        return 1; 
    }
}