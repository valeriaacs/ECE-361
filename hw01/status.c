#include "status.h"
#include "bits.h"

status_t status_unpack(uint16_t word)
{
    status_t status;

    //Convert uint16_t word to uint32_t 
    uint32_t w = (uint32_t)word;

    //Extract 1-bit flags and convert to boolean
    status.heat     = (bool)get_field(w, POS_HEAT, WIDTH_HEAT);
    status.cool     = (bool)get_field(w, POS_COOL, WIDTH_COOL);
    status.fan      = (bool)get_field(w, POS_FAN, WIDTH_FAN);
    status.fault    = (bool)get_field(w, POS_FAULT, WIDTH_FAULT);
    status.reserved = (bool)get_field(w, POS_RESERVED, WIDTH_RESERVED);

    //Extract mode and validate range 
    status.mode = (uint8_t)get_field(w, POS_MODE, WIDTH_MODE);
    if (status.mode > MODE_FAN_ONLY)
    {
        status.is_mode_invalid = true;
    }
    else
    {
        status.is_mode_invalid = false;
    }

    //Extract setpoint and sign-extend the 8-bit value to a signed integer
    uint32_t raw_setpoint = get_field(w, POS_SETPOINT, WIDTH_SETPOINT);
    status.setpoint = (int8_t)sign_extend(raw_setpoint, WIDTH_SETPOINT);

    return status;
}