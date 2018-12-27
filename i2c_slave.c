#include <pic18f4550.h>
#include <stdbool.h>

#include "i2c_slave.h"
#include "leddriver.h"
#include "version.h"

#define ADDRESS_VERSION_MAJOR           0x00
#define ADDRESS_VERSION_MINOR           0x01
#define ADDRESS_BUTTONS_RESET           0x02
#define ADDRESS_LDR                     0x03
#define ADDRESS_BRIGHTNESS              0x04
#define ADDRESS_RAM_START               0x05
#define ADDRESS_RAM_END                 0x0F
#define ADDRESS_SCREENBUFFER_START      0x10
#define ADDRESS_SCREENBUFFER_END        0x25

#define LowB(x) (*((unsigned char*)  &(##x)+1)) 
#define HighB(x) (*((unsigned char*) &(##x)+0)) 

unsigned char i2c_reg_addr     = 0;
unsigned char i2c_ram[11];
unsigned char i2c_byte_count   = 0;

unsigned char getValue();
void setValue(unsigned char data);

void handleI2CISR()
{
    unsigned char sspBuf;
    
    if (BF) {
        sspBuf = SSPBUF;    // Clear BF
    }

    /*
     * Cases:
     * - Write initiated: clear i2c_byte_count (next byte will be the address)
     * - Write data: i2c_byte_count == 0 ? set address : set data
     * - Read initiated: clear i2c_byte_count (unnecessary) + get data
     * - Read data: get data
     */

    if (R_NOT_W) {
        // Read - advance to next address
        SSPCON1bits.WCOL = 0;
        SSPBUF           = getValue();
        i2c_reg_addr++;
    } else if (D_NOT_A) {               
        if (i2c_byte_count == 0) {
            // First write byte is register address
            i2c_reg_addr = sspBuf;
        } else {
            // Write to register address - auto advance
            //   to allow multiple bytes to be written
            setValue(sspBuf);

            i2c_reg_addr++;
        }
    }
        
    if (D_NOT_A) {
        //
        // Data bytes 
        //
        i2c_byte_count++;
    } else {
        //
        // Slave Address
        //
        i2c_byte_count = 0;
    }
        
    // Finally
    CKP = 1;                // Release clock
    SSPIF  = 0;             // Clear MSSP interrupt flag        
}

unsigned char getValue()
{
    if (i2c_reg_addr == ADDRESS_VERSION_MAJOR)
    {
        return MAJOR_FW_VAL;
    }
    if (i2c_reg_addr == ADDRESS_VERSION_MINOR)
    {
        return MINOR_FW_VAL;
    }
    if (i2c_reg_addr == ADDRESS_BUTTONS_RESET)
    {
        return 0x11;
    }
    if (i2c_reg_addr == ADDRESS_BRIGHTNESS)
    {
        return 0x22;
    }
    if (i2c_reg_addr >= ADDRESS_RAM_START && i2c_reg_addr <= ADDRESS_RAM_END)
    {
        unsigned char tt = i2c_reg_addr - ADDRESS_RAM_START;
        return i2c_ram[i2c_reg_addr - ADDRESS_RAM_START];
    }
    if (i2c_reg_addr >= ADDRESS_SCREENBUFFER_START && i2c_reg_addr <= ADDRESS_SCREENBUFFER_END)
    {
        unsigned char relative_address = i2c_reg_addr - ADDRESS_SCREENBUFFER_START;
        if (relative_address & 1)
        {
            return LowB(leddriver_screenbuffer[relative_address / 2]);
        }
        else
        {
            return HighB(leddriver_screenbuffer[relative_address / 2]);
        }
    }
    return 0xFF;
}

void setValue(unsigned char data)
{
    if (i2c_reg_addr == ADDRESS_BUTTONS_RESET)
    {
        // TODO
    }
    else if (i2c_reg_addr == ADDRESS_BRIGHTNESS)
    {
        // TODO
    }
    else if (i2c_reg_addr >= ADDRESS_RAM_START && i2c_reg_addr <= ADDRESS_RAM_END)
    {
        i2c_ram[i2c_reg_addr - ADDRESS_RAM_START] = data;
    }
    else if (i2c_reg_addr >= ADDRESS_SCREENBUFFER_START && i2c_reg_addr <= ADDRESS_SCREENBUFFER_END)
    {
        unsigned char relative_address = i2c_reg_addr - ADDRESS_SCREENBUFFER_START;
        if (relative_address & 1)
        {
            LowB(leddriver_screenbuffer[relative_address / 2]) = data;
        }
        else
        {
            HighB(leddriver_screenbuffer[relative_address / 2]) = data;
        }
    }
}
