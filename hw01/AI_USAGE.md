# AI USAGE STATEMENT
*Tools used:*
 Gemini 
*Used for:*
- Installing toolchain and repository.
- Reviewing function implementation for `print_binary`, `get_field`, `set_field`, and `sign_extend`.
- Explaining why I need `Makefile` and drafting it for the test harness. 
- To check that I sucessfully completed all requirements listed in the homework document.

*One thing I had to fix and how I found it*
When implementing the `print_binary` function, the initial code used a for loop moving in the wrong direction, starting at bit 0 and incrementing up to 31 instead of starting at bit 31 and decrementing down to 0. When I asked Gemini to verify if the function implementation was correct, it simply answered with yes/not. I found the issue by doing a line-by-line manual code trace, realizing that printing from bit 0 to 31 prints the binary representation in reverse (Least Significant Bit first instead of Most Significant Bit first). 