/* 
 * File:   rtc.h
 * Author: Rick
 *
 * Created on 22 december 2012, 22:09
 */

#ifndef RTC_H
#define	RTC_H


void InitRtc();

/**
 * Read current time from RTC into memory.
 */
void ReadRtc();
void WriteRtc();

/**
 * Returns buffered seconds. Use ReadRtc() to update this value.
 * @param seconds
 */
void rtcSetSeconds(unsigned char seconds);
void rtcSetMinutes(unsigned char minutes);
void rtcSetHours(unsigned char hours);

unsigned char rtcGetSeconds(void);
unsigned char rtcGetMinutes(void);
unsigned char rtcGetHours(void);

#endif	/* RTC_H */

