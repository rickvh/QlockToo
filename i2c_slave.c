#include <pic18f4550.h>
#include <stdbool.h>

#include "i2c_slave.h"
#include "leddriver.h"

volatile unsigned char i2c_reg_addr     = 0;
volatile unsigned char i2c_reg_map[16]  = {0,};
volatile unsigned char i2c_byte_count   = 0;

// TODO: implementeer hier gewoon een statemachine!!!!
// ben je nu devver of niet ?!

// https://github.com/harrybraviner/I2C_slave_example/blob/master/web_PIC18F45K20.c

unsigned int handleI2CISR()
{
    unsigned char sspBuf;
    
    if (SSPIF) {
        
        if (! D_NOT_A) {
            //
            // Slave Address 
            //
            i2c_byte_count = 0;

            if (BF) {
                // Discard slave address 
                sspBuf = SSPBUF;    // Clear BF
            }
            
            if (R_NOT_W) {                
                // Reading - read from register map
                SSPCON1bits.WCOL = 0;
                SSPBUF           = i2c_reg_map[i2c_reg_addr++];
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
                SSPBUF           = i2c_reg_map[i2c_reg_addr++];
                // LATDbits.LATD6 = 1;
                
            } else {                

                if (i2c_byte_count == 1) {
                    // First write byte is register address
                    i2c_reg_addr = sspBuf;

                } else {
                    // Write to register address - auto advance
                    //   to allow multiple bytes to be written
                    i2c_reg_map[i2c_reg_addr++] = sspBuf;
                }
            }
        }
        // Limit address to size of register map
        i2c_reg_addr %= sizeof(i2c_reg_map);
        
        // Finally
        SSPIF  = 0;            // Clear MSSP interrupt flag
        CKP = 1;            // Release clock        
    }    

    return 0x8000;
}


unsigned char getI2CRegister()
{
    return i2c_reg_addr;
}

unsigned char getI2CData()
{
    return i2c_reg_map[i2c_reg_addr];
}
