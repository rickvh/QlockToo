#include <stdint.h>        /* For uint8_t definition */
#include "configuration.h"
/* 
 * File:   eeprom.h
 * Author: Rick
 *
 * Created on 15 mei 2013, 22:33
 */

#ifndef EEPROM_H
#define	EEPROM_H

#define BRIGHTNESS_MANUAL   0
#define BRIGHTNESS_AUTO     1
#define ENERGYSAVE_OFF      0
#define ENERGYSAVE_TIME     1
#define ENERGYSAVE_LDR      2
#define CORNERMODE_OFF      0
#define CORNERMODE_ON       1

struct settings{
    uint8_t brightness;
    uint8_t brightnessMode;
    uint8_t energysaveMode;
    uint8_t cornerMode;
};

void int_EEPROM_putc(unsigned char address, unsigned char data);
unsigned char int_EEPROM_getc(unsigned char address);

void loadSettings(struct settings *settingsptr);

void saveSettings(struct settings *settingsptr);

#endif	/* EEPROM_H */

