#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>
#include "configuration.h"
/* 
 * File:   eeprom.h
 * Author: Rick van Hal
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

struct QLOCKTOO_SETTINGS{
    uint8_t brightness;
    uint8_t brightnessMode;
    uint8_t energysaveMode;
    bool showMinutes;
};

void int_EEPROM_putc(unsigned char address, unsigned char data);
unsigned char int_EEPROM_getc(unsigned char address);

void loadSettings(struct QLOCKTOO_SETTINGS *settingsptr);

void saveSettings(struct QLOCKTOO_SETTINGS *settingsptr);

#endif	/* EEPROM_H */

