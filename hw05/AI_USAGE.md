# AI USAGE STATEMENT
*Tools used:*
 Gemini 
*Used for:*
- Check my table and state diagram
- Explain each function in the `lock.h` file
- Create a Make file 
-  Debugging

*One thing a model got wrong*
When I first wrote `lock.c`, I ran into two bugs that broke my program pretty quickly. First, in `lock_step`, I tried writing the output action straight to `*action` without checking if the pointer was actually valid. Whenever a call passed in `NULL`, my program dereferenced invalid memory and crashed with a segmentation fault. Second, in `lock_event`, I forgot to handle the control characters for closing the door (`c`) and timer timeouts (`t`) before checking for digits. Because of that, typing `c` or `t` fell through and got treated like a bad passcode attempt (`E_WRONG`), resetting the lock to `LOCKED` with an annoying `A_BEEP`. I used AI to help debug what was going wrong, added `if (action != NULL)` guards to protect memory, and moved the character checks up front to fix the control flow.