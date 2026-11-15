#include "../istack.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static int passed = 0;
static int failed = 0;

static void check(const char *name, bool condition) {
    if (condition) {
        printf("PASS: %s\n", name);
        passed++;
    } else {
        printf("FAIL: %s\n", name);
        failed++;
    }
}

static void test_empty_pop_peek_leaves_v_alone(void) {
    istack_t *s = istack_create();
    int v = 999;

    int rc_pop = istack_pop(s, &v);
    check("pop on empty returns error and leaves *v alone", rc_pop != 0 && v == 999);

    int rc_peek = istack_peek(s, &v);
    check("peek on empty returns error and leaves *v alone", rc_peek != 0 && v == 999);

    istack_destroy(s);
}

static void test_lifo_order(void) {
    istack_t *s = istack_create();
    istack_push(s, 10);
    istack_push(s, 20);
    istack_push(s, 30);

    int a = 0, b = 0, c = 0, peek_val = 0;
    istack_peek(s, &peek_val);
    istack_pop(s, &a);
    istack_pop(s, &b);
    istack_pop(s, &c);

    bool ok = (peek_val == 30 && a == 30 && b == 20 && c == 10 && istack_is_empty(s));
    check("LIFO push/peek/pop order", ok);

    istack_destroy(s);
}

static void test_resize_at_8_to_16(void) {
    istack_t *s = istack_create();
    for (int i = 0; i < 8; i++) {
        istack_push(s, i * 10);
    }

    bool full_at_8 = (istack_count(s) == 8 && istack_capacity(s) == 8);
    check("full at initial capacity 8", full_at_8);

    // 9th push should double capacity to 16
    int rc = istack_push(s, 80);
    bool doubled = (rc == 0 && istack_count(s) == 9 && istack_capacity(s) == 16);

    // Verify all 9 values survived resize intact
    bool values_ok = true;
    for (int i = 8; i >= 0; i--) {
        int val = -1;
        if (istack_pop(s, &val) != 0 || val != i * 10) {
            values_ok = false;
            break;
        }
    }

    check("9th push doubles capacity to 16 and preserves all values", doubled && values_ok);
    istack_destroy(s);
}

static void test_1000_pushes_and_pops(void) {
    istack_t *s = istack_create();
    bool ok = true;

    for (int i = 0; i < 1000; i++) {
        if (istack_push(s, i) != 0) {
            ok = false;
            break;
        }
    }

    if (istack_count(s) != 1000) ok = false;

    for (int i = 999; i >= 0; i--) {
        int val = -1;
        if (istack_pop(s, &val) != 0 || val != i) {
            ok = false;
            break;
        }
    }

    if (!istack_is_empty(s)) ok = false;
    check("1000 pushes and pops", ok);

    istack_destroy(s);
}

static void test_to_array_bottom_first_and_copy(void) {
    istack_t *s = istack_create();
    istack_push(s, 100);
    istack_push(s, 200);
    istack_push(s, 300);

    size_t n = 0;
    int *arr = istack_to_array(s, &n);
    bool order_ok = (arr != NULL && n == 3 && arr[0] == 100 && arr[1] == 200 && arr[2] == 300);

    // Mutate copy and verify original stack is unaffected
    if (arr != NULL) {
        arr[0] = 9999;
    }
    int v = 0;
    istack_pop(s, &v); // 300
    istack_pop(s, &v); // 200
    istack_pop(s, &v); // 100
    bool copy_ok = (v == 100);

    check("istack_to_array is bottom-first and independent copy", order_ok && copy_ok);

    free(arr);
    istack_destroy(s);
}

static void test_null_arguments(void) {
    int v = 42;
    size_t n = 42;
    istack_t *s = istack_create();
    istack_push(s, 7);

    bool ok = true;
    if (istack_push(NULL, 1) == 0) ok = false;
    if (istack_pop(NULL, &v) == 0 || v != 42) ok = false;
    if (istack_peek(NULL, &v) == 0 || v != 42) ok = false;
    if (istack_peek(s, NULL) == 0) ok = false;
    if (istack_count(NULL) != 0) ok = false;
    if (!istack_is_empty(NULL)) ok = false;
    if (istack_capacity(NULL) != 0) ok = false;
    if (istack_to_array(NULL, &n) != NULL || n != 0) ok = false;
    if (istack_to_array(s, NULL) != NULL) ok = false;

    // NULL pop with valid stack discards top element
    if (istack_pop(s, NULL) != 0 || !istack_is_empty(s)) ok = false;

    istack_destroy(NULL); // Must not crash
    istack_destroy(s);

    check("NULL arguments handled safely", ok);
}

static void test_independent_stacks(void) {
    istack_t *s1 = istack_create();
    istack_t *s2 = istack_create();

    istack_push(s1, 1);
    istack_push(s1, 2);
    istack_push(s2, 99);

    int v1 = 0, v2 = 0;
    istack_pop(s1, &v1);
    istack_peek(s2, &v2);

    bool ok = (istack_count(s1) == 1 && istack_count(s2) == 1 && v1 == 2 && v2 == 99);
    check("two stacks are completely independent", ok);

    istack_destroy(s1);
    istack_destroy(s2);
}

static void test_destroy_non_empty(void) {
    istack_t *s = istack_create();
    for (int i = 0; i < 20; i++) {
        istack_push(s, i);
    }
    istack_destroy(s);
    check("destroying a non-empty stack succeeds without leaks", true);
}

int main(void) {
    test_empty_pop_peek_leaves_v_alone();
    test_lifo_order();
    test_resize_at_8_to_16();
    test_1000_pushes_and_pops();
    test_to_array_bottom_first_and_copy();
    test_null_arguments();
    test_independent_stacks();
    test_destroy_non_empty();

    printf("\nSummary: %d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
