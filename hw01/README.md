# HW01: Bit-Manipulation Library & Thermostat Status Word
Valeria Yanez Garcia

*Overview*
This library implements bit-manipulation functions and a status unpacking system for a 16-bit thermostat control unit.

- `print_binary` prints a 32-bit binary representation of an unsigned integer to standard output. Iterates bit 31 to 0 (MSB to LSB).
- `get_field` Extracts a bit field of width "width" starting at position pos from a 32-bit word, returning the unmodified word if the input are out of bounds, pos (0-31), width (1-32) or if pos+width > 32. 
- `set_field` Replaces a width-bit field starting at bit pos with the lowest bits of value while leaving surrounding bits unchanged, returning the original word for invalid pos (0-31) or width (1-32) inputs.
- `sign_extend` Converts an n-bit two's complement value with width (1-32 bits) into a 32-bit integer (int).
- `status_unpacked` Parses a 16-bit thermostat status word (`0x0000`–`0xFFFF`) into a structured `status_t` representation, converting setpoints (-128 to 127 °C) and flagging invalid modes (5–7) via `is_mode_invalid`.

*How to Build and Run Tests*
- `make` - Compiles object files
- `make test` - Builds and runs the test harness
- `make clean` - Removes built artifacts

*Valid Ranges of Inputs* 

*Out-of-Range Behavior (Part 2)*
When the arguments fall outside bounds for `get_field` and `set_field`, the out of range function will return the word unmodified.

*Invalid Mode Handling (Part 3)*
When `status_unpack` report an invalid mode, the function will set a flag for mode 5-7.  