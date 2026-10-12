# Cold Chain Shipping Logger Specification
Valeria Yanez Garcia

## Device
- **Device Type**: D (Monitor and Logger)
- **Key**: K1
- **Second Structure**: S4
- **Report**: Q6

## States
1. `SEALED`: Box packed, not yet shipped (initial state)
2. `IN_TRANSIT`: Shipping, temperature within safe range (2.0 to 8.0 °C)
3. `EXCURSION`: Temperature outside safe range (2.0 to 8.0 °C)
4. `RECOVERED`: Back in range after an excursion
5. `BREACHED`: Too much time spent out of range; shipment rejected

## Events
- `EVENT_DEPART`: DEPART switch pressed (Switch 0 transition from 0 to 1)
- `EVENT_ARRIVE`: ARRIVE switch pressed (Switch 1 transition from 0 to 1)
- `EVENT_TEMP_IN_RANGE`: Temperature reading is between 2.0 °C and 8.0 °C inclusive
- `EVENT_TEMP_OUT_OF_RANGE`: Temperature reading is below 2.0 °C or above 8.0 °C
- `EVENT_RECOVERY_COMPLETE`: 60 consecutive ticks logged in range while RECOVERED
- `EVENT_BREACH_EXCEEDED`: Total out-of-range time exceeds 300 ticks