# AI USAGE STATEMENT
*Tools used:*
 Gemini 
 Claude 

*Used for:*
- Debugging
- Guidance completing stack functions in `istack.c`
- Used Claude to talk about part 4 of the homework and explaining the memory behavior in `README.md` 

*One thing a model got wrong*
When setting up the intentional memory bugs for Part 4, the model initially predicted that calling `istack_destroy(s)` twice would make AddressSanitizer report `attempting double-free`. In reality, when I ran it under `make memcheck`, AddressSanitizer reported `heap-use-after-free` instead—because `istack_destroy` dereferences the already-freed struct pointer `s` to read `s->data` before `free()` can ever be reached a second time.