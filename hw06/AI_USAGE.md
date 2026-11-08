# AI USAGE STATEMENT
*Tools used:*
 Gemini 
*Used for:*
- Create make file
- Debugging

*One thing a model got wrong*
During my troubleshoothing I encounter a warning strictness under `-Werror` with the loop counter. When updating `scenario_get()` in `scenario.c`, the inital code used an unsigned loop counter (`size_t i`). Because `sc->count` in the `scenario_t` struct was defined as a signed integer (`int count`), the compiler threw a signedness mismatch error under -Wall -Wextra -Werror. To fix this, I matched all data type of the loop directly to `sc->count` signed type.