/* 
 * File:   configuration.h
 * Author: Rick van Hal
 *
 * Created on 16 april 2013, 21:32
 */

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

#include "sw_uart.h"


#define ENABLE_LDR
#define LOCK_INPUT_TIME 250     /* time to wait after a button press */
#define BUTTON1_PIN PORTEbits.RE3
#define BUTTON2_PIN PORTBbits.RB7
#define USB_ATTACHED PORTEbits.RE0

//#define DEBUG   /* output debuginfo to serial interface */


#ifdef DEBUG
#define DEBUG_PRINTLN(x) putsUsART(x)
#else
#define DEBUG_PRINTLN(x)
#endif

#endif	/* CONFIGURATION_H */

