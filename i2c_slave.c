#include <pic18f4550.h>
#include <stdbool.h>

#include "i2c_slave.h"
#include "leddriver.h"

unsigned char registers[255];
unsigned char pointer;
bool read = false;
bool firstWrite = false;

void handleI2CISR()
{
    if (! BF)
    {
        // error?
        CKP = 1; // hele 'if' moet weg als dat kan!
        return;
    }
    
    if (D_A == 0 )                      // Data / Address bit received. 0 = address, 1 = data.
    {
        read = SSPBUF & 0x01;           // Last addressbit holds requested mode
        //BF = 0; // gebeurt impliciet door SSPBUF te lezen?
        
        firstWrite = true;
    }
    else
    {
        if (read)
        {
            SSPBUF = registers[pointer++];
        }
        else
        {
            if (firstWrite)
            {
                firstWrite = false;
                pointer = SSPBUF;
            }
            else
            {
                registers[pointer++] = SSPBUF;
            }
        }
    }

    //D_A = 0; // lijkt me overbodig!
    CKP = 1;                            // SCL may come high again to resume reception.
}


unsigned char getI2CRegister()
{
    return pointer;
}

unsigned char getI2CData()
{
    return registers[pointer];
}
