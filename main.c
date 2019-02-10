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
#define MAJOR_FW_VAL                '0'     // Firmware Version 0.5 (ASCII)
#define	MINOR_FW_VAL                '5'
#define	FW_VALID                    0xAA    // Flag to indicate valid firmware
/******************************************************************************/


typedef enum
{
    CLOCK,
    SECONDS,
    BRIGHTNESS,
    TIME_SET_HOURS,
    TIME_SET_MINS,
    BRIGHTNESS_MODE,
    SHOW_MINUTES,
    LEDTEST,
    LEDTEST_MANUAL,
    ANIMATION_1,
    OFF,
    ALL,
    LDR
} mode;

const mode enabled_modi[] = {CLOCK, SECONDS, BRIGHTNESS, TIME_SET_HOURS, TIME_SET_MINS, SHOW_MINUTES, ALL};
const far unsigned char Firmware_Version[] @ FIRMWARE_VERSION = {MAJOR_FW_VAL, MINOR_FW_VAL, FW_VALID};
const uint8_t enabled_modi_size = sizeof(enabled_modi) / sizeof(mode);

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
uint8_t current_mode;
volatile bool rtc_updated;
bool settings_updated, time_set, usb_attached_at_start, display_enabled = true;
uint16_t i,j, tmp_uur, tmp_min;//just a simple counter
uint8_t time_sec, time_min, time_hour, time_dow, ldr_value;
uint16_t temprow, currentrow;
struct QLOCKTOO_SETTINGS settings = { 9, 0, 0, true };



/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void main(void)
{
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
    loadSettings(&settings);
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
                saveSettings(&settings);
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
            if (current_mode >= enabled_modi_size)
                current_mode = 0;
        }

        // +
        if(button2Pressed())
        {
            switch(enabled_modi[current_mode])
            {
                case CLOCK:
                    display_enabled = !display_enabled;
                    break;
                case BRIGHTNESS:
                    if (++settings.brightness >= 10)
                        settings.brightness = 0;
                    leddriver_brightness = settings.brightness;
                    //settings_updated = true;
                    break;
                case BRIGHTNESS_MODE:
                    if (++settings.brightnessMode > 1)
                        settings.brightnessMode = 0;
                    break;
                case SHOW_MINUTES:
                    settings.showMinutes = !settings.showMinutes;
                    break;
                case TIME_SET_HOURS:
                    if (++time_hour == 24)
                        time_hour = 0;
                    rtcSetHours(time_hour);
                    time_set = true;
                    break;
                case TIME_SET_MINS:
                    if (++time_min == 60)
                        time_min = 0;
                    rtcSetMinutes(time_min);
                    rtcSetSeconds(0);
                    time_set = true;
                    break;
                case LEDTEST_MANUAL:
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

        if (rtc_updated && (enabled_modi[current_mode] == CLOCK || enabled_modi[current_mode] == SECONDS))
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

        switch(enabled_modi[current_mode])
        {
            case CLOCK:
                leddriver_clear();
                if (display_enabled)
                {
                    leddriver_setMinutes(time_hour, time_min);
                    if (settings.showMinutes)
                    {
                        leddriver_setCorners(time_min);
                    }
                }
                break;
            case SECONDS:
                leddriver_clear();
                for (i = 0; i < 7; i++) {
                    leddriver_screenbuffer[1 + i] |= numbers[time_sec/10][i] << 11;
                    leddriver_screenbuffer[1 + i] |= numbers[time_sec%10][i] << 5;
                }
                break;
            case BRIGHTNESS:
                leddriver_clear();
                for (i = 0; i < 7; i++) {
                    leddriver_screenbuffer[1 + i] |= symbols[0][i] << 11;
                    leddriver_screenbuffer[1 + i] |= numbers[settings.brightness][i] << 5;
                }
                break;
            case OFF:
                leddriver_clear();
                break;
            case TIME_SET_HOURS:
                leddriver_clear();
                leddriver_screenbuffer[3] = 0x0040;
                leddriver_screenbuffer[7] = 0x0040;
                for (i = 0; i < 5; i++) {
                    leddriver_screenbuffer[i + 3] |= numbers5[time_hour/10][i] << 13;
                    leddriver_screenbuffer[i + 3] |= numbers5[time_hour%10][i] << 9;
                }
                break;
            case TIME_SET_MINS:
                leddriver_clear();
                leddriver_screenbuffer[3] = 0x4000;
                leddriver_screenbuffer[7] = 0x4000;
                for (i = 0; i < 5; i++) {
                    leddriver_screenbuffer[i + 3] |= numbers5[time_min/10][i] << 9;
                    leddriver_screenbuffer[i + 3] |= numbers5[time_min%10][i] << 5;
                }
                break;
            case BRIGHTNESS_MODE:
                leddriver_clear();
                for (i = 0; i < 5; i++) {
                    leddriver_screenbuffer[1 + i] = symbols[0][i] << 11;
                    if (settings.brightnessMode == BRIGHTNESS_AUTO)
                        leddriver_screenbuffer[1 + i] |= letters['A' - 'A'][i] << 5;
                    else
                        leddriver_screenbuffer[1 + i] |= letters['M' - 'A'][i] << 5;
                }
                break;
            case SHOW_MINUTES:
                leddriver_clear();
                if (settings.showMinutes)
                {
                    leddriver_screenbuffer[10] |= 0xF000;
                }
                else
                {
                    for (i = 0; i < 5; i++) {
                        leddriver_screenbuffer[i + 3] |= letters['X' - 'A'][i] << 5;
                    }
                }
                break;
            case LEDTEST:
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
            case LEDTEST_MANUAL:
                leddriver_clear();
                leddriver_screenbuffer[currentrow] = temprow;
                break;
            case ANIMATION_1:
                leddriver_clear();
                for (i=0; i<11; i++)
                {
                    leddriver_screenbuffer[i] = TMR1H;
                    leddriver_screenbuffer[i] = leddriver_screenbuffer[i] << 8;
                    leddriver_screenbuffer[i] |= TMR1L;
                    __delay_ms(1);
                }
                break;
            case ALL:
                for (i=0; i<11; i++)
                {
                    leddriver_screenbuffer[i] = 0xFFFF;
                }
                break;
            case LDR:
                leddriver_clear();
                for (i=0; i<11; i++)
                {
                    if (i < ldr_read() /10)
                        leddriver_screenbuffer[i] = 0x0FFF;
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