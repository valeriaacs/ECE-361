# AI USAGE STATEMENT
*Tools used:*
 Gemini 
*Used for:*
- Explaning each function in the tally.h file, what they take and assumed
- Debugging
- Create make file

*One thing a model got wrong*
In tally.c, the loop inside tally_add shifts elements using `for (int j = t -> size; j > i; j--)` which relies on `t -> size` being less than `TALLY_NAMES`. This safety check is important because C arrays do not automatically resize or stop you from writing past their boundaries. In my initial code, t->size was allowed to equal TALLY_NAMES (16) when inserting a new name. Since arrays range only from 0 to 15, j starting at 16 and attempting to execute caused a buffer overflow. This is also a common problem in static arrays. Without this boundary check the code could crash doue to overwriting critical values.