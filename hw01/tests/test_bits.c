#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "bits.h"
#include "status.h"

static int g_failed = 0;

// Helper assertion function
static void assert_test(bool condition, const char *test_name)
{
    if (condition)
    {
        printf("[PASS] %s\n", test_name);
    }
    else
    {
        printf("[FAIL] %s\n", test_name);
        g_failed++;
    }
}

int main(void)
{
    printf("=== Running bits.c Tests ===\n");

    //Boundary tests for get_field
    // Width = 1 (extract LSB and MSB)
    assert_test(get_field(0x00000001U, 0, 1) == 1, "get_field: width 1 at pos 0");
    
    // Width = 32
    assert_test(get_field(0x12345678U, 0, 32) == 0x12345678U, "get_field: width 32 at pos 0");
    
    // Pos = 31 (Highest bit position)
    assert_test(get_field(0x80000000U, 31, 1) == 1, "get_field: pos 31 with width 1");

    //Boundary tests for set_field
    // Width = 1 at pos 31
    assert_test(set_field(0x00000000U, 31, 1, 1) == 0x80000000U, "set_field: pos 31 with width 1");

    // Width = 32
    assert_test(set_field(0x12345678U, 0, 32, 0xABCDEF01U) == 0xABCDEF01U, "set_field: width 32");

    // Value too wide for field 
    assert_test(set_field(0x00000000U, 0, 4, 0xFFFFFFFFU) == 0x0000000FU, "set_field: value too wide for field");

    //Boundary tests for sign_extend
    // Width = 1 (1 should sign extend to -1)
    assert_test(sign_extend(1, 1) == -1, "sign_extend: width 1");

    //Width = 32
    assert_test(sign_extend(0xFFFFFFFFU, 32) == -1, "sign_extend: width 32");

    // ost negative value for width 
    assert_test(sign_extend(0x80, 8) == -128, "sign_extend: most negative value for 8-bit");


    printf("\n=== Running status_unpack Tests ===\n");

    //Test 1: Given example 0x1631
    // 0x1631 = 0001 0110 0011 0001
    // setpoint = 0x16 = 22 °C, mode = 3 (AUTO), HEAT = 1 (on), COOL = 0, FAN = 0, FAULT = 0, RESERVED = 0
    status_t s1 = status_unpack(0x1631);
    assert_test(s1.setpoint == 22 && s1.mode == 3 && s1.heat == true && 
                s1.cool == false && s1.fan == false && s1.fault == false && 
                s1.reserved == false && s1.is_mode_invalid == false, 
                "status_unpack: example 0x1631 (22 °C, AUTO, HEAT on)");

    //Test 2: Negative setpoint (-128 °C) with FAN_ONLY mode (4) and FAN on
    // setpoint = -128 (0x80), mode = 4 (0102), FAN = 1 (0x04) -> 0x8044
    status_t s2 = status_unpack(0x8044);
    assert_test(s2.setpoint == -128 && s2.mode == 4 && s2.fan == true && 
                s2.is_mode_invalid == false, 
                "status_unpack: min setpoint -128 °C with FAN_ONLY");

    //Test 3: Invalid Mode
    // setpoint = 10 (0x0A), mode = 5 (1012 = 0x50) -> 0x0A50
    status_t s3 = status_unpack(0x0A50);
    assert_test(s3.mode == 5 && s3.is_mode_invalid == true, 
                "status_unpack: invalid mode detection (mode 5)");


    
    printf("\n=== Test Summary ===\n");
    if (g_failed == 0)
    {
        printf("ALL TESTS PASSED!\n");
        return 0; //Exit code 0 when success
    }
    else
    {
        printf("%d TEST(S) FAILED.\n", g_failed);
        return 1; //Nonzero exit code on failure
    }
}