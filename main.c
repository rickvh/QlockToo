#include <xc.h>             /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "configuration.h"
#include "system.h"        /* System funct/params, like osc/peripheral config */
#include <delays.h>
#include "rtc.h"
#include "eeprom.h"
#include "user.h"          /* User funct/params, such as InitApp */
#include "leddriver.h"
#include "ldr.h"
#include "buttons.h"
#include "characters.h"

/******************************************************************************/
/* For use with USB-MSD bootloader                                            */
/******************************************************************************/
#define FIRMWARE_VERSION            0x002428// Firmware version is located here
                                            // 3 bytes: Major Ver, Minor Ver, FW Valid
#define MAJOR_FW_VAL                '0'     // Firmware Version 0.4 (ASCII)
#define	MINOR_FW_VAL                '4'
#define	FW_VALID                    0xAA    // Flag to indicate valid firmware
/******************************************************************************/


#define MODE_CLOCK              0
#define MODE_SECONDS            1
#define MODE_BRIGHTNESS         99
#define MODE_TIME_SET_HOURS     2
#define MODE_TIME_SET_MINS      3
#define MODE_BRIGHTNESS_MODE  99
#define MODE_CORNER_MODE      99
#define MODE_LEDTEST            4
#define MODE_LEDTEST_MANUAL            5
#define MODE_ANIMATION_1        99
#define MODE_ANIMATION_2      99
#define MODE_ANIMATION_3      99
#define MODE_OFF                6
#define MODE_ALL                7
#define MODE_LDR                99
#define MODE_DEBUG              99


const far unsigned char Firmware_Version[] @ FIRMWARE_VERSION = {MAJOR_FW_VAL, MINOR_FW_VAL, FW_VALID};

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
uint8_t current_mode, reset_cause;
bool rtc_updated, settings_updated, time_set, usb_attached_at_start;
uint16_t i,j, tmp_uur, tmp_min;//just a simple counter
uint8_t time_sec, time_min, time_hour, ldr_value;
uint16_t temprow, currentrow;
struct settings *settingsptr = {0};


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void main(void)
{
    // detect reset cause
    reset_cause = 0;
    if (! nPOR) // Power-on-reset
    {
        reset_cause |= 0x01;
        nPOR = 1;
        nBOR = 1;
    }
    if (!nBOR) // Brown-out-reset
    {
        reset_cause |= 0x02;
        nBOR = 1;
    }
    if (STKFUL) // Stack-overflow
    {
        reset_cause |= 0x04;
        STKFUL = 0;
    }
    if (STKOVF) // Stack-underflow
    {
        reset_cause |= 0x08;
        STKOVF = 0;
    }
/*
        i = 240;
        while(i--)
        {
            __delay_ms(10);
        }

/*    if (reset_cause > 0)
    {
        current_mode = MODE_DEBUG;
    }
*/

    i=0;

    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitQlockToo();
    usb_attached_at_start = USB_ATTACHED;
    // TODO
//    if (rtc_not_running)
//    {
        InitRtc();
//    }
    loadSettings(settingsptr);
    leddriver_init();
    leddriver_brightness = 9;//settingsptr->brightness;

    INT2IE = 1;
    T0IE = 1;
    GIE = 1;

    // main loop
    while(1)
    {
        // MODE
        if (button1Pressed())
        {
            COLUMN_PINS_OFF();
            GIE = 0;
            if (settings_updated)
            {
                saveSettings(settingsptr);
//                int_EEPROM_putc(0x13, settingsptr->brightness);
                settings_updated = false;
            }

            if (time_set)
            {
                WriteRtc();
                time_set = false;
            }
            GIE = 1;

            current_mode++;
            if (current_mode > MODE_ALL)
                current_mode = 0;
        }

        // +
        if(button2Pressed())
        {
            switch(current_mode)
            {
                case MODE_BRIGHTNESS:
                    if (++settingsptr->brightness >= 10)
                        settingsptr->brightness = 0;
                    leddriver_brightness = settingsptr->brightness;
                    //settings_updated = true;
                    break;
//                case MODE_BRIGHTNESS_MODE:
//                    if (++settingsptr->brightnessMode > 1)
//                        settingsptr->brightnessMode = 0;
//                    break;
//                case MODE_CORNER_MODE:
//                    settingsptr->cornerMode = !settingsptr->cornerMode;
//                    break;
                case MODE_TIME_SET_HOURS:
                    if (++time_hour == 24)
                        time_hour = 0;
                    rtcSetHours(time_hour);
                    time_set = true;
                    break;
                case MODE_TIME_SET_MINS:
                    if (++time_min == 60)
                        time_min = 0;
                    rtcSetMinutes(time_min);
                    rtcSetSeconds(0);
                    time_set = true;
                    break;
                case MODE_LEDTEST_MANUAL:
                    temprow = temprow >> 1;
                    if (temprow < 0x0020)
                    {
                        temprow = 0x8000;
                        if (time_min % 10 == 1) // 1100
                        {
                            temprow = 0xC000;
                        }
                        if (time_min % 10 == 2) // 1110
                        {
                            temprow = 0xE000;
                        }
                        if (time_min % 10 == 3) // 1111
                        {
                            temprow = 0xF000;
                        }
                        if (time_min % 10 == 4) // 1001
                        {
                            temprow = 0x9000;
                        }
                        if (time_min % 10 == 5) // 1010
                        {
                            temprow = 0xA000;
                        }
                        if (time_min % 10 == 6) // 1011
                        {
                            temprow = 0xD000;
                        }
                        currentrow++;
                        if (currentrow > 9)
                        {
                            currentrow = 0;
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        if (rtc_updated && (current_mode == MODE_CLOCK || current_mode == MODE_SECONDS))
        {
            ReadRtc();
            time_hour = rtcGetHours();
            time_min = rtcGetMinutes();
            time_sec = rtcGetSeconds();
            rtc_updated = 0;
        }

        if (!usb_attached_at_start && USB_ATTACHED)
        {
            // enter bootloader
            Reset(); 
        }

        i = 5;
        while(i--)
        {
            __delay_ms(10);
        }

        switch(current_mode)
        {
            case MODE_DEBUG:
                leddriver_clear();
                for (i = 0; i < 7; i++) {
                    leddriver_screenbuffer[1 + i] |= numbers[reset_cause][i] << 11;
                }
                break;
            case MODE_CLOCK:
                leddriver_clear();
                leddriver_setMinutes(time_hour, time_min);
                //if (settingsptr->cornerMode == CORNERMODE_ON)
                    leddriver_setCorners(time_min);
                break;
            case MODE_SECONDS:
                leddriver_clear();
                for (i = 0; i < 7; i++) {
                    leddriver_screenbuffer[1 + i] |= numbers[time_sec/10][i] << 11;
                    leddriver_screenbuffer[1 + i] |= numbers[time_sec%10][i] << 5;
                }
                break;
            case MODE_BRIGHTNESS:
                leddriver_clear();
                for (i = 0; i < 7; i++) {
                    leddriver_screenbuffer[1 + i] |= symbols[0][i] << 11;
                    leddriver_screenbuffer[1 + i] |= numbers[settingsptr->brightness][i] << 5;
                }
                break;
            case MODE_OFF:
                leddriver_clear();
                break;
            case MODE_TIME_SET_HOURS:
                leddriver_clear();
                leddriver_screenbuffer[3] = 0x0040;
                leddriver_screenbuffer[7] = 0x0040;
                for (i = 0; i < 5; i++) {
                    leddriver_screenbuffer[i + 3] |= numbers5[time_hour/10][i] << 13;
                    leddriver_screenbuffer[i + 3] |= numbers5[time_hour%10][i] << 9;
                }
                break;
            case MODE_TIME_SET_MINS:
                leddriver_clear();
                leddriver_screenbuffer[3] = 0x4000;
                leddriver_screenbuffer[7] = 0x4000;
                for (i = 0; i < 5; i++) {
                    leddriver_screenbuffer[i + 3] |= numbers5[time_min/10][i] << 9;
                    leddriver_screenbuffer[i + 3] |= numbers5[time_min%10][i] << 5;
                }
                break;
//            case MODE_BRIGHTNESS_MODE:
//                leddriver_clear();
//                for (i = 0; i < 5; i++) {
//                    leddriver_screenbuffer[1 + i] = symbols[0][i] << 11;
//                    if (settingsptr->brightnessMode == BRIGHTNESS_AUTO)
//                        leddriver_screenbuffer[1 + i] |= letters['A' - 'A'][i] << 5;
//                    else
//                        leddriver_screenbuffer[1 + i] |= letters['M' - 'A'][i] << 5;
//                }
//                break;
//            case MODE_CORNER_MODE:
//                leddriver_screenbuffer[0] = 0x0400;
//                break;
            case MODE_LEDTEST:
                temprow = temprow >> 1;
                if (temprow < 0x0020)
                {
                    temprow = 0x8000;
                    currentrow++;
                    if (currentrow > 9)
                    {
                        currentrow = 0;
                    }
                }
                leddriver_clear();
                leddriver_screenbuffer[currentrow] = temprow;
                break;
            case MODE_LEDTEST_MANUAL:
                leddriver_clear();
                leddriver_screenbuffer[currentrow] = temprow;
                break;
            case MODE_ANIMATION_1:
                leddriver_clear();
                for (i=0; i<11; i++)
                {
                    leddriver_screenbuffer[i] = TMR1H;
                    leddriver_screenbuffer[i] = leddriver_screenbuffer[i] << 8;
                    leddriver_screenbuffer[i] |= TMR1L;
                    __delay_ms(1);
                }
                break;
//            case MODE_ANIMATION_2:
//                leddriver_screenbuffer[0] = 0x0200;
//                break;
//            case MODE_ANIMATION_3:
//                leddriver_screenbuffer[0] = 0x0100;
//                break;
            case MODE_ALL:
                for (i=0; i<11; i++)
                {
                    leddriver_screenbuffer[i] = 0xFFFF;
                }
                break;
            case MODE_LDR:
                leddriver_clear();
                for (i=0; i<11; i++)
                {
                    if (i < ldr_read() /10)
                        leddriver_screenbuffer[i] = 0xFFFF;
                }
                break;
            default:
                break;
        }
    }
}


/******************************************************************************/
/* Interrupt handler                                                          */
/******************************************************************************/
void interrupt isr(void)
{
    static unsigned char debounce_timer;

    // TIMER0
    if(TMR0IF)
    {
        debounce_timer++;
        if (debounce_timer == 0)
            updateButtonReadings();
        leddriver_writeNextRowToMatrix();   // write next row to LED-matrix
        TMR0 = 0x80;
        TMR0IF = 0;                         // Clear Interrupt Flag
    }
    // INTERRUPT PIN INT2 (RTC signals every 1 second)
    else if(INT2IF)
    {
        rtc_updated = 1;
        INT2IF = 0;                         // Clear Interrupt Flag
    }
    else
    {
        /* Unhandled interrupts */
    }
}