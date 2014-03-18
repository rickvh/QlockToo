#include <xc.h>

#include "ldr.h"

unsigned char current_value;

unsigned char ldr_read()
{
    if (!GO_nDONE)
    {
        current_value = ADRESH;
        GO_nDONE = 1;   // Start reading new value
    }
    return current_value;
}
