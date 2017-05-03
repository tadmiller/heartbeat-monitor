#include "envlib.h"
#include <Wire.h>
#include <Math.h>

byte i2cWriteBuffer[10];
byte i2cReadBuffer[10];

unsigned int r = 0;
unsigned int g = 0;
unsigned int b = 0;

/*  
Send register address and the byte value you want to write the magnetometer and 
loads the destination register with the value you send
*/

void Writei2cRegisters(byte numberbytes, byte command)
{
	Wire.beginTransmission(SensorAddressWrite);   // Send address with Write bit set
	Wire.write(command);                          // Send command, normally the register address

	// Send data 
	for (byte i = 0; i < numberbytes; i++)
		Wire.write(i2cWriteBuffer[i]);

	Wire.endTransmission();

	delayMicroseconds(100);      // allow some time for bus to settle      
}

/*  
Send register address to this function and it returns byte value
for the magnetometer register's contents 
*/
byte Readi2cRegisters(int numberbytes, byte command)
{
	byte i = 0;

	Wire.beginTransmission(SensorAddressWrite);   // Write address of read to sensor
	Wire.write(command);
	Wire.endTransmission();

	delayMicroseconds(100);      // allow some time for bus to settle      

	Wire.requestFrom(SensorAddressRead,numberbytes);   // read data

	for (byte i = 0 ; i < numberbytes; i++)
		i2cReadBuffer[i] = Wire.read();

	Wire.endTransmission();   

	delayMicroseconds(100);      // allow some time for bus to settle      
}  

void init_TCS34725()
{
	i2cWriteBuffer[0] = 0x10;
	Writei2cRegisters(1, ATimeAddress);    // RGBC timing is 256 - contents x 2.4mS =  
	i2cWriteBuffer[0] = 0x00;
	Writei2cRegisters(1, ConfigAddress);   // Can be used to change the wait time
	i2cWriteBuffer[0] = 0x00;
	Writei2cRegisters(1, ControlAddress);  // RGBC gain control
	i2cWriteBuffer[0] = 0x03;
	Writei2cRegisters(1, EnableAddress);    // enable ADs and oscillator for sensor  
}

/*
Reads the register values for clear, red, green, and blue.
*/
void get_TCS34725ID()
{
	Readi2cRegisters(1, IDAddress);

	if (i2cReadBuffer[0] = 0x44)
		Serial.println("ENV loaded");    
	else
		Serial.println("RGB not responding");    
}

void printRGB()
{
	findColor();

	Serial.print("RED: ");
	Serial.println(r);
	Serial.print("GRN: ");
	Serial.println(g);
	Serial.print("BLU: ");
	Serial.println(b);
}

void findColor()
{
	unsigned int clear_color = 0;


	Readi2cRegisters(8,ColorAddress);
	clear_color = (unsigned int)(i2cReadBuffer[1] << 8) + (unsigned int)i2cReadBuffer[0];
	r = (unsigned int)(i2cReadBuffer[3] << 8) + (unsigned int)i2cReadBuffer[2];
	g = (unsigned int)(i2cReadBuffer[5] << 8) + (unsigned int)i2cReadBuffer[4];
	b = (unsigned int)(i2cReadBuffer[7] << 8) + (unsigned int)i2cReadBuffer[6];

	// Basic RGB color differentiation can be accomplished by
	// comparing the values and the largest reading will be the prominent color

}

void printColor()
{
	findColor();

	if (r > b && r > g)
		Serial.println("RED");
	else if (g > b && g > r)
		Serial.println("GREEN");
	else if (b > r && b > g)
		Serial.println("BLUE");
	else
		Serial.println("BLACK");
}


void initEnv()
{
	init_TCS34725();
	get_TCS34725ID();     // get the device ID, this is just a test to see if we're connected
}