#include "istack.h"
#include <stdio.h>
#include <stdlib.h>

// Helper: returns the matching closing bracket for an opening bracket
static char matching_close(char open) {
    switch (open) {
        case '(': return ')';
        case '[': return ']';
        case '{': return '}';
        default:  return '\0';
    }
}

int main(void) {
    // Create the integer stack (stores ASCII values of opening brackets)
    istack_t *s = istack_create();
    if (s == NULL) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    size_t line = 1;
    size_t col = 0;
    size_t pairs = 0;
    size_t deepest = 0;
    int ch;

    // Read standard input one character at a time until EOF
    while ((ch = getchar()) != EOF) {
        // Update line and column counters (1-based)
        if (ch == '\n') {
            line++;
            col = 0;
            continue;
        }
        col++;

        // Push opening brackets onto the stack
        if (ch == '(' || ch == '[' || ch == '{') {
            if (istack_push(s, ch) != 0) {
                fprintf(stderr, "out of memory\n");
                istack_destroy(s);
                return 1;
            }
            // Update maximum nesting depth seen so far
            size_t depth = istack_count(s);
            if (depth > deepest) {
                deepest = depth;
            }
        }
        // Check closing brackets against the top of the stack
        else if (ch == ')' || ch == ']' || ch == '}') {
            int open_ch;
            // Error 1: closing bracket encountered with nothing open
            if (istack_pop(s, &open_ch) != 0) {
                printf("line %zu, column %zu: found '%c', nothing open\n",
                       line, col, (char)ch);
                istack_destroy(s);
                return 1;
            }

            // Error 2: closing bracket doesn't match the innermost open bracket
            char expected = matching_close((char)open_ch);
            if ((char)ch != expected) {
                printf("line %zu, column %zu: found '%c', expected '%c'\n",
                       line, col, (char)ch, expected);
                istack_destroy(s);
                return 1;
            }

            // Valid pair matched
            pairs++;
        }
    }

    // Error 3: EOF reached with unclosed brackets remaining on the stack
    if (!istack_is_empty(s)) {
        int innermost;
        istack_peek(s, &innermost);
        printf("end of input: %zu still open, innermost '%c'\n",
               istack_count(s), (char)innermost);
        istack_destroy(s);
        return 1;
    }

    // Success: all brackets matched cleanly
    printf("balanced: %zu pairs, deepest %zu\n", pairs, deepest);
    istack_destroy(s);
    return 0;
}