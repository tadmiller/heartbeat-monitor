#include "comslib.h"

int sendReading(String data)
{
	Serial.println(data);

	return 0;
}

void sendBPM(int hour, int min, int sec, int reading)
{
	sendReading(String(reading) + "," + String(hour) + "," + String(min) + "," + String(sec) + ",");
}

void sendRGB(int r, int g, int b)
{
	sendReading(String(r) + "," + String(g) + "," + String(b) + ",");
}

void receiveClock()
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
	updateClock();
	Serial.println("Successfully synced time!");
	printTime();
	//Serial.print()
}

void sendTime()
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