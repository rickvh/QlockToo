/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include "sw_uart.h"

#endif

#include "system.h"

/* Refer to the device datasheet for information about available
oscillator configurations. */
void ConfigureOscillator(void)
{
    __delay_ms(30);
    __delay_ms(30);
    __delay_ms(30);
}

//------------------ UART Delay Functions----------------------------------------------------------------
// https://www.microchip.com/forums/m670226-print.aspx
//DelayTXBitUART delay for[(2 * f)/(4 * baud) + 1]/2 - 12 cycles --- f = 48MHz
 void DelayTXBitUART(void)
 {
     _delay(288); //38400 baud: calculated 301 cycle delay, confirmed 288 with oscilloscope
 }

 //DelayRXHalfBitUART delay for[(2 * f)/(8 * baud) + 1]/2 - 9 cycles
 void DelayRXHalfBitUART(void)
 {
//     _delay(148); //38400 baud: 147.8 cycles
     _delay(144); //38400 baud: 147.8 cycles
 }

 //DelayRXBitUART delay for[(2 * f)/(4 * baud) + 1]/2 - 14 cycles
 void DelayRXBitUART()
 {
//     _delay(299); //38400 baud: 299 cycles
     _delay(288); //38400 baud: 299 cycles
 }