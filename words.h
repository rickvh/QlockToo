/* 
 * File:   words.h
 * Author: Rick
 *
 * Created on 28 april 2013, 22:54
 */

#ifndef WORDS_H
#define	WORDS_H

/*
 * LED-MATRIX
 *
 *  H E T K I S A V I J F     HET, IS, VIJF
 *  T I E N B T Z V O O R     TIEN, VOOR
 *  O V E R M E K W A R T     OVER, KWART
 *  H A L F S P W O V E R     HALF, OVER
 *  V O O R T H G E E N S     VOOR, EEN
 *  T W E E P V C D R I E     TWEE, DRIE
 *  V I E R V I J F Z E S     VIER, VIJF, ZES
 *  Z E V E N O N E G E N     ZEVEN, NEGEN
 *  A C H T T I E N E L F     ACHT, TIEN, ELF
 *  T W A A L F B F U U R     TWAALF, UUR
 */

/**
 * Word definitions
 */
#define NL_VOOR         leddriver_screenbuffer[1] |= 0b0000000111100000
#define NL_OVER         leddriver_screenbuffer[2] |= 0b1111000000000000
#define NL_VOOR2        leddriver_screenbuffer[4] |= 0b1111000000000000
#define NL_OVER2        leddriver_screenbuffer[3] |= 0b0000000111100000
#define NL_HETIS        leddriver_screenbuffer[0] |= 0b1110110000000000
#define NL_UUR          leddriver_screenbuffer[9] |= 0b0000000011100000

#define NL_VIJF         leddriver_screenbuffer[0] |= 0b0000000111100000
#define NL_TIEN         leddriver_screenbuffer[1] |= 0b1111000000000000
#define NL_KWART        leddriver_screenbuffer[2] |= 0b0000001111100000
#define NL_ZWANZIG      leddriver_screenbuffer[1] |= 0b0000111111100000
#define NL_HALF         leddriver_screenbuffer[3] |= 0b1111000000000000

#define NL_H_EEN        leddriver_screenbuffer[4] |= 0b0000000111000000
#define NL_H_EENS       leddriver_screenbuffer[4] |= 0b0000000111100000
#define NL_H_TWEE       leddriver_screenbuffer[5] |= 0b1111000000000000
#define NL_H_DRIE       leddriver_screenbuffer[5] |= 0b0000000111100000
#define NL_H_VIER       leddriver_screenbuffer[6] |= 0b1111000000000000
#define NL_H_VIJF       leddriver_screenbuffer[6] |= 0b0000111100000000
#define NL_H_ZES        leddriver_screenbuffer[6] |= 0b0000000011100000
#define NL_H_ZEVEN      leddriver_screenbuffer[7] |= 0b1111100000000000
#define NL_H_ACHT       leddriver_screenbuffer[8] |= 0b1111000000000000
#define NL_H_NEGEN      leddriver_screenbuffer[7] |= 0b0000001111100000
#define NL_H_TIEN       leddriver_screenbuffer[8] |= 0b0000111100000000
#define NL_H_ELF        leddriver_screenbuffer[8] |= 0b0000000011100000
#define NL_H_TWAALF     leddriver_screenbuffer[9] |= 0b1111110000000000


#endif	/* WORDS_H */

