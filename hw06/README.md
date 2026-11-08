# HW06: Reading Sensor and Switchess Through Registers
Valeria Yanez Garcia

*Overview*
`scenario.c`: Handles file parsing and simulation timeline lookups. The `scenario_load()` function parses trace files, enforcing mandatory `start <timestamp>` headers, bounding capacity via `MAX_SCENARIO_LINES`, and verifying strictly increasing tick timestamps. The `scenario_get()` function performs step-holding lookups across simulation ticks.

`sensor.c`: Manages environmental telemetry by interacting with hardware registers via `iom361_readReg()`. It converts raw register values into physical units (°C and %RH) and applies error-guard threshold checks to reject out-of-spec hardware readings.

`panel.c`: Controls visual hardware output, driving the LED panel and multiplexed 7-segment displays. It utilizes bitwise register masking and shift operations to update system notifications while preserving existing register state bits.

`main.c`: Orchestrates the core simulation loop (`replay`). It reads scenario entries sequentially and coordinates the sensor and panel modules to reproduce real-world hardware behavior over time.

*How to Build and Test*
To build and verify the program, first ensure all scenario trace files in `scenarios/` (such as `valid.txt`, `scenario1.txt`, and `scenario2.txt`) contain the required `start <timestamp>` header as their first non-comment line, and grant execution permissions to the regression script using `chmod +x scenarios/regress.sh`. Open a terminal in the project root directory and execute `make clean` to remove previous object files, temporary trace outputs, and binaries. Follow this with `make test`, which compiles both the `replay` executable and `test_hw06` test runner under strict `-Wall -Wextra -Werror` compiler flags, executes the unit test suite, and runs `./scenarios/regress.sh` to validate trace outputs against expected results. To inspect a trace manually, run `./replay scenarios/valid.txt` to print the hardware register interactions directly to standard output.