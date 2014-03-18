/* 
 * File:   configuration.h
 * Author: Rick
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

//#define DEBUG   /* output debuginfo to serial interface */


#ifdef DEBUG
#define DEBUG_PRINTLN(x) putsUART(x)
#else
#define DEBUG_PRINTLN(x)
#endif

#endif	/* CONFIGURATION_H */

