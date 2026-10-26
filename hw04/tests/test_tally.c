#include "../tally.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_create_and_destroy(void) {
    printf("Testing create and pool limits...\n");
    tally_t *t[TALLY_MAX];

    /* Exhaust all static slots */
    for (int i = 0; i < TALLY_MAX; i++) {
        t[i] = tally_create();
        assert(t[i] != NULL);
    }

    /* TALLY_MAX + 1 must fail */
    tally_t *fail = tally_create();
    assert(fail == NULL);

    /* Destroy one and re-create */
    tally_destroy(t[0]);
    t[0] = tally_create();
    assert(t[0] != NULL);

    /* Clean up all */
    for (int i = 0; i < TALLY_MAX; i++) {
        tally_destroy(t[i]);
    }
}

void test_add_and_sort(void) {
    printf("Testing addition, sorting, and counts...\n");
    tally_t *t = tally_create();
    assert(t != NULL);

    /* Add out of order: zebra, apple, banana */
    assert(tally_add(t, "zebra") == 1);
    assert(tally_add(t, "apple") == 1);
    assert(tally_add(t, "banana") == 1);
    assert(tally_add(t, "apple") == 2); /* Increment count */

    assert(tally_size(t) == 3);
    assert(tally_count(t, "apple") == 2);
    assert(tally_count(t, "banana") == 1);
    assert(tally_count(t, "zebra") == 1);
    assert(tally_count(t, "cherry") == 0); /* Non-existent */

    tally_destroy(t);
}

void test_edge_cases(void) {
    printf("Testing string length limits and NULL handling...\n");
    tally_t *t = tally_create();

    /* Invalid string tests */
    assert(tally_add(t, "") == -1);                                     /* Empty string */
    assert(tally_add(t, "1234567890123456") == -1);                     /* 16 chars > TALLY_LEN (15) */
    assert(tally_add(t, "123456789012345") == 1);                      /* Exactly 15 chars (valid) */

    /* NULL guards */
    assert(tally_add(NULL, "valid") == -1);
    assert(tally_add(t, NULL) == -1);
    assert(tally_count(NULL, "valid") == 0);
    assert(tally_size(NULL) == 0);

    tally_destroy(t);
}

int main(void) {
    test_create_and_destroy();
    test_add_and_sort();
    test_edge_cases();
    printf("ALL TALLY TESTS PASSED SUCCESSFULLY!\n");
    return 0;
}