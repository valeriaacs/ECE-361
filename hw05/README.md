# HW05: Abstract data types: Finite State Machine as a Table: A Combination Lock
Valeria Yanez Garcia

*Overview*
`lock_step(state_t s, event_t e, action_t *action)` takes your current state `s` and incoming event `e`, looks up what happens next in `lock_table`, and returns that new state. It also fills `*action` with whatever action the lock should take (unless `action` is `NULL`). If `s` or `e` are total garbage values out of range, it keeps you in state `s` and sets `*action` to `A_UNDEFINED`.

`lock_event(state_t s, char c)` takes a raw keypress `c` and turns it into an event based on state `s`. It maps `'c'`/`'C'` to `E_CLOSE` and `'t'`/`'T'` to `E_TIMEOUT`. For numbers, it checks against the `3-1-4` combo to throw back `E_RIGHT` on a match or `E_WRONG` on a typo. Anything else returns `-1`.

`state_name(state_t s)` grabs the string version of a state enum (like `"LOCKED"` or `"OPEN"`) out of a static array. If you pass in a state that doesn't exist, it gives you back `"?"`.

`event_name(event_t e)` returns the string version of an event enum (like `"E_RIGHT"` or `"E_TIMEOUT"`) from a static array. Gives back `"?"` if out of bounds.

`action_name(action_t a)` gets the string name of an action enum (like `"A_UNLOCK"` or `"A_BEEP"`) from a static array. Gives back `"?"` if out of bounds.

*How to Build and Test*
To build and test the project, open a terminal in the root `hw05` directory and run `make`. That command compiles lock.c and main.c together using GCC with strict flags `(-Wall -Wextra -Werror -std=c99 -g)`, which produces the `lock` executable. To run the automated unit test suite, execute `make test` (or run `./test_lock` directly) to verify that all 16 state-event transition table entries, character-to-event parsing, string helper bounds checks, and specific input sequences like `314` and `31t4` pass completely. The main executable can also be tested manually using input redirection, such as `./lock < data/codes.txt.` Finally, running `make clean` removes all compiled `.o` object files and binaries for a fresh build.
