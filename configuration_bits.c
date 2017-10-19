/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

/******************************************************************************/
/* Configuration Bits                                                         */
/*                                                                            */
/* Refer to 'HI-TECH PICC and PICC18 Toolchains > PICC18 Configuration        */
/* Settings' under Help > Contents in MPLAB X IDE for available PIC18         */
/* Configuration Bit Settings for the correct macros when using the C18       */
/* compiler.  When using the Hi-Tech PICC18 compiler, refer to the compiler   */
/* manual.pdf in the compiler installation doc directory section on           */
/* 'Configuration Fuses'.  The device header file in the HiTech PICC18        */
/* compiler installation directory contains the available macros to be        */
/* embedded.  The XC8 compiler contains documentation on the configuration    */
/* bit macros within the compiler installation /docs folder in a file called  */
/* pic18_chipinfo.html.                                                       */
/*                                                                            */
/* For additional information about what the hardware configurations mean in  */
/* terms of device operation, refer to the device datasheet.                  */
/*                                                                            */
/* General C18/XC8 syntax for configuration macros:                           */
/* #pragma config <Macro Name>=<Setting>, <Macro Name>=<Setting>, ...         */
/*                                                                            */
/* General HiTech PICC18 syntax:                                              */
/* __CONFIG(n,x);                                                             */
/*                                                                            */
/* n is the config word number and x represents the anded macros from the     */
/* device header file in the PICC18 compiler installation include directory.  */
/*                                                                            */
/* A feature of MPLAB X is the 'Generate Source Code to Output' utility in    */
/* the Configuration Bits window.  Under Window > PIC Memory Views >          */
/* Configuration Bits, a user controllable configuration bits window is       */
/* available to Generate Configuration Bits source code which the user can    */
/* paste into this project.                                                   */
/*                                                                            */
/******************************************************************************/

//#pragma config	XINST = ON		// Extended Mode
#pragma config	USBDIV = 2			// USB Clock from 96MHz PLL / 2
#pragma config	CPUDIV = OSC1_PLL2              // CPU Clock Postscaler (OSC1 / 1, 96MHz PLL Src / 2)
#pragma config	PLLDIV = 5			// PLL Prescaler (20MHz input)
#pragma config	FOSC = HSPLL_HS                 // HS Oscillator, HS-USB
#pragma config  FCMEN = OFF			// Fail Safe Clock Monitor
#pragma config  IESO = OFF			// Int/Ext Switchover Mode
#pragma config	PWRT = ON			// PWRT Enabled
#pragma config	BOR = ON			// Hardware Brown-Out Reset
#pragma config	BORV = 1			// 4.33 BOR Level
#pragma config	VREGEN = ON			// USB voltage Regulator Enabled
#pragma config  WDT = OFF			// No Watchdog Timer
#pragma config  WDTPS = 256                     // Watchdog Timer Prescaler
#pragma config	MCLRE = OFF			// MCLR Disabled (RE3 available as input)
#pragma config	LPT1OSC = OFF                   // Normal Power Timer 1 Osc
#pragma config	PBADEN = OFF                    // PORTB<4:0) are Digital I/O
#pragma config  CCP2MX = ON			// CCP2 Mux RC1
#pragma config  STVREN = ON			// Stack Overflow Reset
#pragma config	LVP = OFF			// Disable Single-Supply ICSP
#pragma config	ICPRT = OFF			// Dedicated ICPORT

#pragma config	WRT0 = OFF			// No Write Protection
#pragma config	WRT1 = OFF
#pragma config	WRT2 = OFF
#pragma config	WRT3 = OFF
#pragma config	WRTB = OFF
#pragma config	WRTC = OFF
#pragma config	WRTD = OFF

#pragma config	EBTR0 = OFF			// No Table Read Protection
#pragma config	EBTR1 = OFF
#pragma config	EBTR2 = OFF
#pragma config	EBTR3 = OFF
#pragma config	EBTRB = OFF


#pragma config	CP0 = OFF                       // No Code Protection
#pragma config	CP1 = OFF
#pragma config	CP2 = OFF
#pragma config	CP3 = OFF
#pragma config	CPB = OFF
#pragma config	CPD = OFF

