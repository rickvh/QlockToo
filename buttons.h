/* 
 * File:   buttons.h
 * Author: Rick van Hal
 *
 * Created on 25 april 2013, 22:01
 */
#include <stdbool.h>

#ifndef BUTTONS_H
#define	BUTTONS_H

/*
 * Debounced butonstate polling.
 */
void updateButtonReadings();

/*
 * Returns true when button is pressed AND it wasn't pressed
 * during last reading. So every push will only return true once.
 */
bool button1Pressed();

/*
 * Returns true when button is pressed AND it wasn't pressed
 * during last reading. So every push will only return true once.
 */
bool button2Pressed();

#endif	/* BUTTONS_H */

