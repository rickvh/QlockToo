#include <xc.h>             /* XC8 General Include File */
#include <stdio.h>
#include <stdbool.h>
#include "string.h"
#include "configuration.h"
#include "words.h"
#include "leddriver.h"


unsigned int current_row;
unsigned int brightness_counter;
unsigned int leddriver_screenbuffer[11];
unsigned int leddriver_brightness;

/* Function prototypes */
void setHours(unsigned char hours, bool round);


/**
 * Initializes I/O-pins for the LED-display.
 */
void leddriver_init()
{
    COLUMN_PINS_OFF();
    ROW_PINS_OFF();
    TRISA &= 0b11000001;    // pins RA5, RA4, RA3, RA2, RA1 configured as output
#ifdef DEBUG
    TRISB &= 0b01000111;    // pins RB5, RB4, RB3 configured as output, RB6 and RB7 untouched (for UART in debug mode)
#else
    TRISB &= 0b10000111;    // pins RB6, RB5, RB4, RB3 configured as output
#endif
    TRISC &= 0b00111000;
    TRISD = 0;
    leddriver_clear();
}

/**
 * Clears screenbuffer
 */
void leddriver_clear()
{
    memset(leddriver_screenbuffer, 0, sizeof(leddriver_screenbuffer));
}

/**
 * Writes to LED-display one row at a time. By repeatedly calling
 * this method (every few ms), the whole screenbuffer will be written to the LED-display.
 * Method takes care of cycling through all rows.
 */
void leddriver_writeNextRowToMatrix()
{
    current_row++;
    if (current_row > 10)
        current_row = 0;

#ifdef DEBUG
    //unsigned char message[60];
    //sprintf(message,"Current row: %d, value: 0x%04X\r\n", current_row, leddriver_screenbuffer[current_row]);
    //DEBUG_PRINTLN(message);
#endif

    /* turn off all leds */
    ROW_PINS_OFF();
    COLUMN_PINS_OFF();
    
    if (--brightness_counter <= leddriver_brightness)
        brightness_counter = 10;
    else
        return;

    /* enable columns */
    COLUMN1 = (leddriver_screenbuffer[current_row] & 0x8000) != 0;
    COLUMN2 = (leddriver_screenbuffer[current_row] & 0x4000) != 0;
    COLUMN3 = (leddriver_screenbuffer[current_row] & 0x2000) != 0;
    COLUMN4 = (leddriver_screenbuffer[current_row] & 0x1000) != 0;
    COLUMN5 = (leddriver_screenbuffer[current_row] & 0x0800) != 0;
    COLUMN6 = (leddriver_screenbuffer[current_row] & 0x0400) != 0;
    COLUMN7 = (leddriver_screenbuffer[current_row] & 0x0200) != 0;
    COLUMN8 = (leddriver_screenbuffer[current_row] & 0x0100) != 0;
    COLUMN9 = (leddriver_screenbuffer[current_row] & 0x0080) != 0;
    COLUMN10 = (leddriver_screenbuffer[current_row] & 0x0040) != 0;
    COLUMN11 = (leddriver_screenbuffer[current_row] & 0x0020) != 0;

    /* enable 1 row */
    ROW1 = current_row == 0;
    ROW2 = current_row == 1;
    ROW3 = current_row == 2;
    ROW4 = current_row == 3;
    ROW5 = current_row == 4;
    ROW6 = current_row == 5;
    ROW7 = current_row == 6;
    ROW8 = current_row == 7;
    ROW9 = current_row == 8;
    ROW10 = current_row == 9;
    ROW11 = current_row == 10;
}


void leddriver_setMinutes(unsigned char hours, unsigned char minutes)
{
    NL_HETIS;

    switch (minutes / 5) {
    case 0:
      // heel uur
      setHours(hours, true);
      break;
    case 1:
      // 5 over
      NL_VIJF;
      NL_OVER;
      setHours(hours, false);
      break;
    case 2:
      // 10 over
      NL_TIEN;
      NL_OVER;
      setHours(hours, false);
      break;
    case 3:
      // kwart over
      NL_KWART;
      NL_OVER2;
      setHours(hours, false);
      break;
    case 4:
      // 10 voor half
      NL_TIEN;
      NL_VOOR;
      NL_HALF;
      setHours(hours + 1, false);
      break;
    case 5:
      // 5 voor half
      NL_VIJF;
      NL_VOOR;
      NL_HALF;
      setHours(hours + 1, false);
      break;
    case 6:
      // half
      NL_HALF;
      setHours(hours + 1, false);
      break;
    case 7:
      // 5 over half
      NL_VIJF;
      NL_OVER;
      NL_HALF;
      setHours(hours + 1, false);
      break;
    case 8:
      // 20 voor
      NL_TIEN;
      NL_OVER;
      NL_HALF;
      setHours(hours + 1, false);
      break;
    case 9:
      // viertel voor
      NL_KWART;
      NL_VOOR2;
      setHours(hours + 1, false);
      break;
    case 10:
      // 10 voor
      NL_TIEN;
      NL_VOOR;
      setHours(hours + 1, false);
      break;
    case 11:
      // 5 voor
      NL_VIJF;
      NL_VOOR;
      setHours(hours + 1, false);
      break;
    }
}

void setHours(unsigned char hours, bool round)
{
    while (hours >= 12) {
      hours -= 12;
    }

    if (round) {
      NL_UUR;
    }

    switch (hours) {
    case 0:
      NL_H_TWAALF;
      break;
    case 1:
      NL_H_EEN;
      break;
    case 2:
      NL_H_TWEE;
      break;
    case 3:
      NL_H_DRIE;
      break;
    case 4:
      NL_H_VIER;
      break;
    case 5:
      NL_H_VIJF;
      break;
    case 6:
      NL_H_ZES;
      break;
    case 7:
      NL_H_ZEVEN;
      break;
    case 8:
      NL_H_ACHT;
      break;
    case 9:
      NL_H_NEGEN;
      break;
    case 10:
      NL_H_TIEN;
      break;
    case 11:
      NL_H_ELF;
      break;
    }
}

void leddriver_setCorners(unsigned char minutes)
{
    switch (minutes % 5)
    {
        case 4:
            leddriver_screenbuffer[10] |= 0x8000;
        case 3:
            leddriver_screenbuffer[10] |= 0x4000;
        case 2:
            leddriver_screenbuffer[10] |= 0x2000;
        case 1:
            leddriver_screenbuffer[10] |= 0x1000;
        default:
            break;
    }
}
    