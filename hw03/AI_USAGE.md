# AI USAGE STATEMENT
*Tools used:*
 Gemini 
*Used for:*
- Explaining the four functions and pointers paramters. 
- Helping checking my pointer arithmetic
- Revising my code but not give me the solution untilI understand why is wrong
- Explaining each line of the Makefile

*One thing I had to fix and how I found it*
In my `minmax` function my code evaluated the output variables before validating the input argumets. I founded this when running my tests, after two of them failed I checked the ptr.c file and I place a guard clause at the top of the function.