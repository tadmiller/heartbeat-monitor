#ifndef _CMDLIB_H
#define _CMDLIB_H

#include <Arduino.h>
#include <Wire.h>
#include "rgblib.h"
#include "heartbeatlib.h"
#include "rtclib.h"
#include "comslib.h"

#define PROCESSING_VISUALIZER 1
#define SERIAL_PLOTTER 2

void serialOutput();

//  Decides How To OutPut BPM and IBI Data
void serialOutputWhenBeatHappens();

//  Sends Data to Pulse Sensor Processing App, Native Mac App, or Third-party Serial Readers.
void sendToSerial(char symbol, int data);

void blinkLED(int bpms);

void updateCmd();

#endif