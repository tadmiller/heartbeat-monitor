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
	char h = receiveByte();
	while (h != 'h')
	{
		Serial.print("h is: ");
		Serial.println(h);

		h = receiveByte();
	}
}

char receiveByte()
{
	char rec = 0;

	while (rec == 0)
		if (Serial.available() > 0)
			rec = Serial.read();

	return rec;
}