#include <xc.h>             /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include <plib/i2c.h>

#include "configuration.h"
#include "user.h"

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

    /* Setup I2C */
    TRISBbits.TRISB0 = 0;   // SDA
    TRISBbits.TRISB1 = 0;   // SCL
    PORTBbits.RB0 = 0;
    PORTBbits.RB1 = 0;
    OpenI2C(MASTER, SLEW_OFF);
    SSPADD = 49;            //Baud rate generator value, see formula in the answer (20MHZ=49)


    /* Setup buttons */
    // TODO: enable RB7 pullup, RE3 heeft externe pullup
    RBPU = 0;       // enable internal pull-ups for PORTB
#ifndef DEBUG
    TRISBbits.TRISB7 = 1;  // RB7 as input. In DEBUG-mode RB7 is used for uart.
#endif

    /* Setup interrupts sources */
    INTEDG2 = 1;    // INT2 interrupt on rising edge
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


    /* Configure the IPEN bit (1=on) in RCON to turn on/off int priorities */

}

