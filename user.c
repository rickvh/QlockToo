#include <xc.h>             /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include <plib/i2c.h>

#include "configuration.h"
#include "user.h"
#include "i2c_slave.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitQlockToo(void)
{
#ifdef DEBUG
    /* Setup UART */
    //TRISBbits.TRISB6 = 1;
    ADCON0 = 0x00;
    ADCON1 = 0x0F;
    OpenUART();
#endif

    i2cInitMaster();


    /* Setup buttons */
    // TODO: enable RB7 pullup, RE3 heeft externe pullup
    RBPU = 0;       // enable internal pull-ups for PORTB
#ifndef DEBUG
    TRISBbits.TRISB7 = 1;  // RB7 as input. In DEBUG-mode RB7 is used for uart.
#endif

    /* Setup interrupts sources */
    INTEDG2 = 1;    // INT2 interrupt on rising edge; this is when RTC notifies us that one second has elapsed.
    INT2IE = 1;
#ifdef DEBUG
    T0CON = 0x8D;   // Timer0 enabled: 16-bit, internal clock, prescaler 1:64
#else
    T0CON = 0xC0;   // Timer0 enabled: 8-bit, internal clock, no prescaler
#endif
    T1CON = 0x85;   // Timer 1 enabled: 16-bit, no prescaler, oscillator disabled



#ifdef ENABLE_LDR
    /* Setup analog input for LDR */
    TRISAbits.TRISA0 = 1;   // AN0 set as input
    ADCON0 = 0x01;          // AN0 channel selected, AD-converter enabled but idle
    ADCON1 = 0x0E;          // AN0 analog, others digital I/O
    ADCON2 = 0x0E;          // Left justified, Tacq=2Tad, Tad=2*Tosc (or Fosc/2)
#endif



}

void i2cInitMaster()
{
    SSPIE = 0;
    /* Setup I2C */
    OpenI2C(MASTER, SLEW_OFF);
    // TODO: aanpassen aan 48MHZ
    SSPADD = 119;            // Clock generator: SSPADD = ((Fosc/Bitrate)/4)-1
}

void i2cInitSlave()
{
    TRISC |= 0b00011000;    // TRISC 3&4 (SCL & SDA) inputs
    
    SSPSTAT = 0x80; // 100Khz
    SSPADD = I2C_SLAVE_ADDR;
    SSPCON1 = SSPCON1 & 0b11110000 | 0b00000110; // Slave mode, 7bit addr, no start- and stopbit interrupts
    
    SEN = 1;                // Clock stretching enabled
    SSPIF = 0;
    SSPIE = 1;
}
