#ifndef _COMSLIB_H
#define _COMSLIB_H

#include <Arduino.h>
#include <Wire.h>

int sendReading(String data);

void handleComs(int hour, int min, int sec, int reading);

void sendRGB(int r, int g, int b);

#endif