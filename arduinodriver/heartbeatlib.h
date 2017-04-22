#ifndef _HEARTBEATLIB_H
#define _HEARTBEATLIB_H

#include <Arduino.h>
#include <Wire.h>

#define PIN_HEARTBEAT A0                 // Pulse Sensor purple wire connected to analog pin 0

void initHeartbeat();

int getBPM();

// THIS IS THE TIMER 2 INTERRUPT SERVICE ROUTINE.
// Timer 2 makes sure that we take a reading every 2 miliseconds
ISR(TIMER2_COMPA_vect);

#endif