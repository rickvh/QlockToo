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
    
    if (SSPIF) {
        if (! D_NOT_A) {
            //
            // Slave Address; ignore 
            //
            i2c_byte_count = 0;

            if (BF) {
                // Discard slave address 
                sspBuf = SSPBUF;    // Clear BF
            }
            
            if (R_NOT_W) {                
                // Reading - read from register map
                SSPCON1bits.WCOL = 0;
                SSPBUF           = i2c_reg_addr;////getValue();
                i2c_reg_addr++;
            } 
        } else {
            //
            // Data bytes 
            //
            i2c_byte_count++;

            if (BF) {
                sspBuf = SSPBUF;    // Clear BF
            }

            if (R_NOT_W) {
                // Multi-byte read - advance to next address
                SSPCON1bits.WCOL = 0;
                SSPBUF           = i2c_reg_addr;//getValue();
                i2c_reg_addr++;
            } else {                
                if (i2c_byte_count == 1) {
                    // First write byte is register address
                    i2c_reg_addr = sspBuf;
                } else {
                    // Write to register address - auto advance
                    //   to allow multiple bytes to be written
                    setValue(sspBuf);
                    
                    // TODO: als deze aan staat loopt het lezen 2 bytes voor??
                    i2c_reg_addr+=4;
//                    i2c_reg_addr=0xA0;
                }
            }
        }
        
        // Finally
        CKP = 1;            // Release clock
        SSPIF  = 0;            // Clear MSSP interrupt flag        
    }    
}

unsigned char getValue()
{
    
    if (i2c_reg_addr == ADDRESS_VERSION_MAJOR)
    {
        return i2c_reg_addr;
//        return MAJOR_FW_VAL;
    }
    if (i2c_reg_addr == ADDRESS_VERSION_MINOR)
    {
        return i2c_reg_addr;
//        return MINOR_FW_VAL;
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
