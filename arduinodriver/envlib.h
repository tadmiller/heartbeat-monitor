#ifndef _ENVLIB_H
#define _ENVLIB_H

/*
BM017_Arduino_color_sensing:  This program interfaces to the AMS TCS34725 color light
to digital converter IC.  It uses the Arduino I2C interface.  

Schematics associated with the BM017 module may be used for hardware wiring information.
See the user datasheet at www.solutions-cubed.com for additional information.

*/

#include <Arduino.h>

#include <Wire.h>
#include <Math.h>

#define SensorAddressWrite 0x29 //
#define SensorAddressRead 0x29 // 
#define EnableAddress 0xa0 // register address + command bits
#define ATimeAddress 0xa1 // register address + command bits
#define WTimeAddress 0xa3 // register address + command bits
#define ConfigAddress 0xad // register address + command bits
#define ControlAddress 0xaf // register address + command bits
#define IDAddress 0xb2 // register address + command bits
#define ColorAddress 0xb4 // register address + command bits

/*  
Send register address and the byte value you want to write the magnetometer and 
loads the destination register with the value you send
*/
void Writei2cRegisters(byte numberbytes, byte command);

/*  
Send register address to this function and it returns byte value
for the magnetometer register's contents 
*/
byte Readi2cRegisters(int numberbytes, byte command);

void init_TCS34725(void);

void get_TCS34725ID(void);

void init_env();

int findColor(void);

int get_RedVal(void);

int get_GreenVal(void);

int get_BlueVal(void);

#endif






