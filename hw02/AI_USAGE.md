# AI USAGE STATEMENT
*Tools used:*
 Gemini 
*Used for:*
- Defining local and global variables. Also to identify each function
- Explain function's implementations
- Debugging when my test were failing
- Explaining why my code was incorrect
- Checking homework specifications

*One thing I had to fix and how I found it*
When running the test and my program was failing the edge.txt regression test. When I ran diff, my logic was formatting high values incorrectly when the temperature readings were avoid 100 degrees celsius. Another reason why this was happening could be overflowing the bin arrays bounds.

I keep double checking histogram.c and asking AI what my issue was if I was following all the required rules. I realize the program didn't drop out-of-range values. Instead it was clamped any reading greater than 100C. 

The fix was to explicity calculated bin index to MX_BINS - 1 when the values exceeded the upper boundery limit. This allowed all tests to pass byte-for-byte.  