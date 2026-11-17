# ECE 361 - Final Project
# Cold Chain Shipping Logger Specification
Valeria Yanez Garcia

## Device
**Device Type**: D (Monitor and Logger)
**Key**: K1
**Second Structure**: S4
**Report**: Q6


## States
`SEALED`: Box packed, not yet shipped (initial state)
`IN_TRANSIT`: Shipping, temperature within safe range (2.0 to 8.0 °C)
`EXCURSION`: Temperature outside safe range (2.0 to 8.0 °C)
`RECOVERED`: Back in range after an excursion
`BREACHED`: Too much time spent out of range; shipment rejected


## Events
`EVENT_DEPART`: DEPART switch pressed (Switch 0 transition from 0 to 1)
`EVENT_ARRIVE`: ARRIVE switch pressed (Switch 1 transition from 0 to 1)
`EVENT_TEMP_IN_RANGE`: Temperature reading is between 2.0 °C and 8.0 °C inclusive
`EVENT_TEMP_OUT_OF_RANGE`: Temperature reading is below 2.0 °C or above 8.0 °C
`EVENT_RECOVERY_COMPLETE`: 60 consecutive ticks logged in range while RECOVERED
`EVENT_BREACH_EXCEEDED`: Total out-of-range time exceeds 300 ticks 

## Transition Table
| Current State | `EVENT_DEPART` | `EVENT_ARRIVE` | `EVENT_TEMP_IN_RANGE` | `EVENT_TEMP_OUT_OF_RANGE` | `EVENT_RECOVERY_COMPLETE` | `EVENT_BREACH_EXCEEDED` |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **`SEALED`** | `IN_TRANSIT` (start log) | `SEALED` (ignore) | `SEALED` (none) | `SEALED` (none) | `SEALED` (none) | `SEALED` (none) |
| **`IN_TRANSIT`** | `IN_TRANSIT` (ignore) | `IN_TRANSIT` (stop log) | `IN_TRANSIT` (none) | `EXCURSION` (inc excursion) | `IN_TRANSIT` (none) | `BREACHED` (reject) |
| **`EXCURSION`** | `EXCURSION` (ignore) | `BREACHED` (reject) | `RECOVERED` (reset recovery) | `EXCURSION` (inc excursion) | `EXCURSION` (none) | `BREACHED` (reject) |
| **`RECOVERED`** | `RECOVERED` (ignore) | `RECOVERED` (stop log) | `RECOVERED` (inc recovery) | `EXCURSION` (inc excursion) | `IN_TRANSIT` (none) | `BREACHED` (reject) |
| **`BREACHED`** | `BREACHED` (ignore) | `BREACHED` (stop log) | `BREACHED` (none) | `BREACHED` (none) | `BREACHED` (none) | `BREACHED` (none) |

## Edge-Case Decisions
- **`ARRIVE` before `DEPART` (`SEALED` + `EVENT_ARRIVE`)**: Ignored; the device remains in `SEALED` because the shipment has not departed.
- **Duplicate `DEPART` (`EVENT_DEPART` after leaving `SEALED`)**: Ignored; logging is already active (or completed).
- **`ARRIVE` during `EXCURSION` (`EXCURSION` + `EVENT_ARRIVE`)**: Transitions to `BREACHED` and outputs `VERDICT: REJECTED` because the shipment arrived while actively outside the safe 2.0–8.0 °C window.
- **`ARRIVE` during `IN_TRANSIT` or `RECOVERED`**: Stops logging, retains the current accepted state, and outputs `VERDICT: ACCEPTED`.
- **`BREACHED` permanence**: Once `BREACHED` is entered (either via exceeding 300 cumulative out-of-range ticks or arriving during an active excursion), all subsequent temperature and recovery events are ignored; `ARRIVE` still stops logging and outputs `VERDICT: REJECTED`.
- **Switch handling**: Switch inputs (`SW0` = `DEPART`, `SW1` = `ARRIVE`) trigger events on rising edges (`0 -> 1`).

## Hardware Mapping & Trace Format
### Switches & Indicators
- **Switches (`SWITCHES_REG`)**:
  - Bit `0` (`0x1`): `DEPART`
  - Bit `1` (`0x2`): `ARRIVE`
- **Discrete LEDs (`LEDS_REG`)**:
  - Bit `0` (`0x1`): `IN_RANGE` (2.0 °C to 8.0 °C inclusive)
  - Bit `1` (`0x2`): `EXCURSION_LAMP` (< 2.0 °C or > 8.0 °C)
  - Bit `2` (`0x4`): `REJECT` (lit when state is `BREACHED`)
- **RGB LED (`RGB_LED_REG`)**:
  - `SEALED`: Blue (`0, 0, 255`)
  - `IN_TRANSIT`: Green (`0, 255, 0`)
  - `EXCURSION`: Amber (`255, 165, 0`)
  - `RECOVERED`: Cyan (`0, 255, 255`)
  - `BREACHED`: Red (`255, 0, 0`)

### 32-Bit Status Word Layout
- **Bits `[2:0]`**: Current `state_t` value (`0`–`4`)
- **Bit `[3]`**: `IN_RANGE` indicator
- **Bit `[4]`**: `EXCURSION_LAMP` indicator
- **Bit `[5]`**: `REJECT` indicator
- **Bit `[6]`**: Shipment arrived flag

### Trace Output Format
Each tick prints one UTC line followed by a final verdict upon arrival:
```text
YYYY-MM-DD HH:MM:SS state=<STATE> temp=<T.T> rh=<H.H> leds=0x<X> status=0x<XXXXXXXX>
VERDICT: <ACCEPTED|REJECTED>