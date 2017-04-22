/*
 * @name Processor.ino 
 * @vers 0.3
 * @auth Theodore Miller, Danny Nsouli
 * @desc This code reads data from the heartbeat monitor
 *       and displays it on the RGB 8x8 LED matrix. It can
 *       also receive commands from a C backend running on
 *       a UNIX OS.
 *       Credit to Joel Murphy and Yury Gitman for the
 *       original code to read the analog data from the heartbeat 
 *       monitor. Link: http://www.pulsesensor.com
 * 
 * 
 ******************/

#include <Arduino.h>
#include <Wire.h>

#include "rgblib.h"
#include "comslib.h"
#include "cmdlib.h"
//#include "envlib.h"

#include "rtclib.h"
#include "heartbeatlib.h"

void setup()
{
	Serial.begin(9600);

	initMatrix();   // Initialize RGB Matrix
	initClock();    // Initialize clock
	initHeartbeat(); // sets up to read Pulse Sensor signal every 2mS
	//initLightSensor();
	pinMode(6, OUTPUT);
}

void loop()
{
	updateClock();
	updateCmd();
	//blinkLED(getBPM());

	delay(50);

	// Allows us to make the LED flash at the rate of the heart beat
//    if (showValue > 0 && paused)
//        delay(20 + (21000 / showValue));
//    else if (BPM == 0 || paused)
//        delay(100);
//    else
//        delay(20 + (21000 / BPM));

	// I like shorthands
	// This one checks to see that the BPM is > 0 OR we're showing something on the screen. Otherwise we don't blink.
	//digitalWrite(8, BPM > 0 || (showValue > 0 && paused) ? ledStatus = !ledStatus : LOW);
}
