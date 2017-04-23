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
	sendReading("R[" + String(r) + "]G[" + String(g) + "]B[" + String(b) + "]");
}

void receiveClock()
{

}

char receiveByte()
{
	char rec = 0;

	while (rec == 0)
		if (Serial.available() > 0)
			rec = Serial.read();

	return rec;
}