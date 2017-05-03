#ifndef _RTCLIB_H
#define _RTCLIB_H

#include <SparkFunDS1307RTC.h>
#include <Wire.h>
#include <Time.h>
#include <Arduino.h>

// Comment out the line below if you want month printed before date.
// E.g. October 31, 2016: 10/31/16 vs. 31/10/16
#define PIN_CLOCK_INT 2   // Pin to plug clock INT into
#define PIN_CLOCK_LED 13 // LED to indicate SQW's state

// Initialize the Arduino clock
void initClock();

String getSysTime();

int getSysHour();

int getSysMin();

int getSysSec();

void setClock(int h, int m, int s);

// Update the reading of the clock to the latest recorded time.
void updateClock();

void printTime();

#endif