#include "bits.h"
#include <stdio.h>

//print_binary function
void print_binary(uint32_t x, int width)
{
    for (int i = width - 1; i >= 0; i--)
    {
        if ((x >> i) & 1U)
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
    }
    printf("\n");
}
//get_field function
uint32_t get_field(uint32_t word, int pos, int width)
{
    //Out-of-range inputs
    if(pos < 0 || width <= 0 || pos + width > 32)
    {
        return word; // Return unmodified word
    }

    //Right shift the unmodified word 
    uint32_t shifted = word >> pos;

    //Create a mask of width ones
    uint32_t mask;
    if(width == 32)
    {
        mask = 0xFFFFFFFFU; 
    }
    else{
        mask = (1u << width) - 1u;
    }

    //Return field
    return shifted & mask;
    
}

//set_field function
uint32_t set_field(uint32_t word, int pos, int width, uint32_t value)
{
    //Out-of-range inputs
    if(pos < 0 || width <= 0 || pos + width > 32)
    {
        return word; //Return unmodified word
    }

    //Create a mask of width ones
    uint32_t mask;
    if(width == 32)
    {
       mask = 0xFFFFFFFFU; 
    }
    else
    {
        mask = (1U << width) - 1U;
    }

    //Clear bits
    uint32_t cleared_word = word & ~(mask << pos);

    //Replace new value to width bits, shift it to pos and combine
    uint32_t new_value = (value & mask) << pos;

    return cleared_word | new_value;
    
}

//sign_extend function
int32_t sign_extend(uint32_t value, int width)
{
    int shift = 32 - width;

    return(int32_t)(value << shift) >> shift;
    
}
