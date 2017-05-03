#include "comslib.h"

// Sends the data from the sensors
int sendReading(String data) 
{
	Serial.println(data);

	return 0;
}

// String intToString(int s, int len)
// {
// 	String res = String(s);

// 	for (int i = )
// }

// Sends the heart beats per minute from the heart rate sensor
void sendBPM(int hour, int min, int sec, int reading)	
{
	char buff[14];
	sprintf(buff, "%.3d,%.2d,%.2d,%.2d,", reading, hour, min, sec);
	buff[13] = '\0';
	sendReading(String(buff));
}

void sendRGB(int r, int g, int b)
{
	sendReading(String(r) + "," + String(g) + "," + String(b) + ",");
}

// Taking in the clock's data in order to send to c files
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

		msg = receiveByte();
	}
	while (msg != 'h');

	do
	{
		m = msg - 32;

		msg = receiveByte();
	}
	while (msg != 'm');

	do
	{
		s = msg - 32;

		msg = receiveByte();
	}
	while (msg != 's');

	setClock(h, m, s);
	Serial.println("Successfully synced time!");
	delay(100);
	printTime();
	//Serial.print()
}

// Send the time to be read by the computer
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