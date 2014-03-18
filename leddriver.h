/* 
 * File:   leddriver.h
 * Author: Rick
 *
 * Created on 16 april 2013, 21:13
 * 
 */

#ifndef LEDDRIVER_H
#define	LEDDRIVER_H

#define ROW1 LATD7
#define ROW2 LATD6
#define ROW3 LATD5
#define ROW4 LATD4
#define ROW5 LATC7
#define ROW6 LATC6
#define ROW7 LATD3
#define ROW8 LATD2
#define ROW9 LATD1
#define ROW10 LATD0
#define ROW11 LATC2

#define COLUMN1 LATC1
#define COLUMN2 LATC0
#define COLUMN3 LATA5
#define COLUMN4 LATA4
#define COLUMN5 LATA3
#define COLUMN6 LATA2
#define COLUMN7 LATA1
#define COLUMN8 LATB6
#define COLUMN9 LATB5
#define COLUMN10 LATB4
#define COLUMN11 LATB3


#define ROW_PINS_OFF()    LATC &= 0b00111011; LATD = 0
#define COLUMN_PINS_OFF() LATA &= 0b11000001; LATB &= 0b10000111; LATC &= 0b11111100

/**
 * Screenbuffer for LED-display. Every byte represents a single row.
 */
extern unsigned int leddriver_screenbuffer[11];
extern unsigned int leddriver_brightness;

void leddriver_init();

/**
 * Clears screenbuffer
 */
void leddriver_clear();

/**
 * Writes to LED-display one row at a time. By repeatedly calling
 * this method (every few ms), the whole screenbuffer will be written to the LED-display.
 * Method takes care of cycling through all rows.
 */
void leddriver_writeNextRowToMatrix();

void leddriver_setMinutes(unsigned char hours, unsigned char minutes);

void leddriver_setCorners(unsigned char minutes);

#endif	/* LEDDRIVER_H */

