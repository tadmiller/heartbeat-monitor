#include "comslib.h"

char msgCount = 32;

int sendReading(String data)
{
	Serial.print("C[");
	Serial.print(msgCount);
	Serial.print("]");
	Serial.print(data);
	Serial.println("");

	msgCount += 1;

	if (msgCount >= 127)
		msgCount = 32;

	return 0;
}

void handleComs(int hour, int min, int sec, int reading)
{
	sendReading("B[" + String(reading) + "]H[" + String(hour) + "]M[" + String(min) + "]S[" + String(sec) + "]");
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