/******************************************************************************
DS1307_RTC_Demo.ino
Jim Lindblom @ SparkFun Electronics
original creation date: October 2, 2016
https://github.com/sparkfun/SparkFun_DS1307_RTC_Arduino_Library

Configures, sets, and reads from the DS1307 real-time clock (RTC).

Resources:
Wire.h - Arduino I2C Library

Development environment specifics:
Arduino 1.6.8
SparkFun RedBoard
SparkFun Real Time Clock Module (v14)
******************************************************************************/
#include <SparkFunDS1307RTC.h>
#include <Wire.h>
#include <Time.h>

// Comment out the line below if you want month printed before date.
// E.g. October 31, 2016: 10/31/16 vs. 31/10/16
#define PIN_CLOCK_INT 2   // Pin to plug clock INT into
#define PIN_CLOCK_LED 13 // LED to indicate SQW's state

String sys_time;
int sys_hour;
int sys_minute;
int sys_second;

String sys_date;

void initClock()
{
	pinMode(PIN_CLOCK_INT, INPUT_PULLUP);
	pinMode(PIN_CLOCK_LED, OUTPUT);
	digitalWrite(PIN_CLOCK_LED, digitalRead(PIN_CLOCK_INT));
	
	rtc.begin(); // Call rtc.begin() to initialize the library
	// (Optional) Sets the SQW output to a 1Hz square wave.
	// (Pull-up resistor is required to use the SQW pin.)
	rtc.writeSQW(SQW_SQUARE_1);
	
	rtc.setHour(String(__TIME__).substring(0, 2).toInt());
	rtc.setMinute(String(__TIME__).substring(3, 5).toInt());
	rtc.setSecond(String(__TIME__).substring(6, 8).toInt());
}

void updateClock()
{
	static int8_t lastSecond = -1;
	
	// Call rtc.update() to update all rtc.seconds(), rtc.minutes(),
	// etc. return functions.
	rtc.update();
	
	if (rtc.second() != lastSecond) // If the second has changed
	{
		//printTime(); // Print the new time
		lastSecond = rtc.second(); // Update lastSecond value

		sys_time = String(rtc.hour()) + ":";//+  + ;
		sys_time += rtc.minute() < 10 ? "0" : "" + String(rtc.minute()) + ":";
		sys_time += rtc.second() < 10 ? "0" : "" + String(rtc.second());

		sys_hour = rtc.hour();
		sys_minute = rtc.minute();
		sys_second = rtc.second();
	}

	digitalWrite(PIN_CLOCK_LED, digitalRead(PIN_CLOCK_INT));
}

void printTime()
{
	Serial.print(String(rtc.hour()) + ":"); // Print hour

	if (rtc.minute() < 10)
		Serial.print('0'); // Print leading '0' for minute

	Serial.print(String(rtc.minute()) + ":"); // Print minute

	if (rtc.second() < 10)
		Serial.print('0'); // Print leading '0' for second

	Serial.print(String(rtc.second())); // Print second

	if (rtc.is12Hour()) // If we're in 12-hour mode
	{	// Use rtc.pm() to read the AM/PM state of the hour
		if (rtc.pm()) Serial.print(" PM"); // Returns true if PM
		else Serial.print(" AM");
	}

	Serial.print(" | ");

	// Few options for printing the day, pick one:
	Serial.print(rtc.dayStr()); // Print day string
	//Serial.print(rtc.dayC()); // Print day character
	//Serial.print(rtc.day()); // Print day integer (1-7, Sun-Sat)
	Serial.print(" - ");
	Serial.print(String(rtc.month()) + "/" + String(rtc.date()) + "/"); // Print month // Print date 
	Serial.println(String(rtc.year()));        // Print year
}
