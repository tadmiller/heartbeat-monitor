#include "comslib.h"

int sendReading(String data) //Sends the data from the sensors
{
	Serial.println(data);

	return 0;
}

void sendBPM(int hour, int min, int sec, int reading)	//Sends the heart beats per minute from the heart rate sensor
{
	sendReading(String(reading) + "," + String(hour) + "," + String(min) + "," + String(sec) + ",");
}

void sendRGB(int r, int g, int b)
{
	sendReading(String(r) + "," + String(g) + "," + String(b) + ",");
}

void receiveClock()	//Taking in the clock's data in order to send to c files
{
	Serial.println("Receiving clock...");
	char h;
	char m;
	char s;
	char msg = ' ';

	do
	{
		h = msg - 32;
		Serial.print("h is: ");
		Serial.println(h);

		msg = receiveByte();
	}
	while (msg != 'h');

	do
	{
		m = msg - 32;
		Serial.print("m is: ");
		Serial.println(m);

		msg = receiveByte();
	}
	while (msg != 'm');

	do
	{
		s = msg - 32;
		Serial.print("s is: ");
		Serial.println(s);

		msg = receiveByte();
	}
	while (msg != 's');

	setClock(h, m, s);
	Serial.println("Successfully synced time!");
	printTime();
	//Serial.print()
}

void sendTime()	//Send the time to be read by the computer
{
	sendReading(getSysTime());
}

char receiveByte()
{
	char rec = 0;

	while (rec == 0)
		if (Serial.available() > 0)
			rec = Serial.read();

	return rec;
}