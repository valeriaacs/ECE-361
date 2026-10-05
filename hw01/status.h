#ifndef STATUS_H
#define STATUS_H

#include <stdint.h>
#include <stdbool.h>

//Field bit positions
#define POS_HEAT     0
#define POS_COOL     1
#define POS_FAN      2
#define POS_FAULT    3
#define POS_MODE     4
#define POS_RESERVED 7
#define POS_SETPOINT 8

//Field bit widths
#define WIDTH_HEAT     1
#define WIDTH_COOL     1
#define WIDTH_FAN      1
#define WIDTH_FAULT    1
#define WIDTH_MODE     3
#define WIDTH_RESERVED 1
#define WIDTH_SETPOINT 8

//Named constants for modes
#define MODE_OFF       0
#define MODE_HEAT      1
#define MODE_COOL      2
#define MODE_AUTO      3
#define MODE_FAN_ONLY  4

//Status structure representing unpacked fields
typedef struct {
    bool heat;
    bool cool;
    bool fan;
    bool fault;
    uint8_t mode;
    bool reserved;
    int8_t setpoint;
    bool is_mode_invalid; //Flag to report an invalid mode (5–7)
} status_t;

status_t status_unpack(uint16_t word);

#endif // STATUS_H