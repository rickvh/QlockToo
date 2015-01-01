#include <xc.h>
#include <plib/i2c.h>
#include "rtc.h"

// DS1307 address: 1101000 = 0xD0
#define DS1307_ADDRESS_READ     0xD1
#define DS1307_ADDRESS_WRITE    0xD0

unsigned char _hours, _minutes, _seconds;

void rtcSetSeconds(unsigned char seconds)
{
    _seconds = seconds;
}

void rtcSetMinutes(unsigned char minutes)
{
    _minutes = minutes;
}
void rtcSetHours(unsigned char hours)
{
    _hours = hours;
}

unsigned char rtcGetSeconds(void)
{
    return _seconds;
}

unsigned char rtcGetMinutes(void)
{
    return _minutes;
}

unsigned char rtcGetHours(void)
{
    return _hours;
}


// enable RTC.
// TODO: Init should only be called when RTC is not yet initialised, as
// writing the enabled flag will reset seconds.
void InitRtc(void)
{
    StartI2C(); // begin I2C communication
    IdleI2C();
    WriteI2C( DS1307_ADDRESS_WRITE );
    IdleI2C();
    WriteI2C( 0x00 ); // register: seconds
    IdleI2C();
    WriteI2C( 0x00 ); // 0sec + enable clock
    IdleI2C();
    StopI2C();

    // init: SQWE @1Hz
    StartI2C(); // begin I2C communication
    IdleI2C();
    WriteI2C( DS1307_ADDRESS_WRITE );
    IdleI2C();
    WriteI2C( 0x07 ); // register: control register
    IdleI2C();
    WriteI2C( 0x10 ); // SQWE @1Hz
    IdleI2C();
    StopI2C();
}


void ReadRtc()
{
    // Lees alles
    StartI2C(); // begin I2C communication
    IdleI2C();
    WriteI2C( DS1307_ADDRESS_WRITE );
    IdleI2C();
    WriteI2C( 0x00 ); // address to read
    IdleI2C();
    RestartI2C(); // Initiate a RESTART command
    IdleI2C();
    WriteI2C( DS1307_ADDRESS_READ );
    IdleI2C();
    _seconds = ReadI2C();
    IdleI2C();
    AckI2C();
    IdleI2C();
    _minutes = ReadI2C();
    IdleI2C();
    AckI2C();
    IdleI2C();
    _hours = ReadI2C();
    IdleI2C();
    NotAckI2C();
    IdleI2C();
    StopI2C();

    // convert BCD-format to decimal and eliminate non-time data
    _seconds = (((_seconds & 0x70) >> 4)*10+(_seconds & 0x0F));
    _minutes = ((_minutes >> 4)*10+(_minutes & 0x0F));
    _hours = (((_hours & 0x30) >> 4)*10+(_hours & 0x0F));
}

void WriteRtc()
{
    // write seconds
    StartI2C(); // begin I2C communication
    IdleI2C();
    WriteI2C( DS1307_ADDRESS_WRITE );
    IdleI2C();
    WriteI2C( 0x00 ); // register: seconds
    IdleI2C();
    WriteI2C( 0x00 ); // seconds
    IdleI2C();
    WriteI2C( (_minutes/10) << 4 | _minutes%10 ); // minutes
    IdleI2C();
    WriteI2C( (_hours/10) << 4 | _hours%10 ); // hours
    IdleI2C();
    StopI2C();
}