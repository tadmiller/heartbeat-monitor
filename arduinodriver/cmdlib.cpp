#include "cmdlib.h"

boolean paused = false;
boolean ledStatus = false;

int dispVal = 0;
int cmd = 0;
int ledCount = 0;

//  Sends Data to Pulse Sensor Processing App, Native Mac App, or Third-party Serial Readers.
void sendToSerial(char symbol, int data)
{
	Serial.print(symbol);
	Serial.println(data);
}

void blinkLED(int bpms)
{
	ledCount++;
	
	if (ledCount > 900 / bpms)
	{
		digitalWrite(6, HIGH);
		ledCount = 0;
	}
	else
		digitalWrite(6, LOW);
}

void updateCmd()
{
	// Check and see if we can read a command.
	if (Serial.available() > 0)
		cmd = Serial.read();
	
	// If there's a new pulse, and we haven't received a command, and we're not paused from the cmd prompt
	if (getPulse() && cmd == 0 && !paused)
	{
		//serialOutput();
		receivedPulse(false);                      // reset the Quantified Self flag for next time

		// We can't send values higher than 99 since the display is only two numbers.
		// Then we write our new BPM to the display
		matrixWrite(getBPM() > 99 ? 99 : getBPM());
	}
	else
	{   // 112 == 'p' which means we pause the program state.
		if (cmd == 112)
			paused = true;
		else if (cmd == 114) // 114 = 'r' which means we resume the program state.
			paused = false;
		else if (cmd == 115) // 115 is 's' which means we're going to display the number that comes after
		{
			dispVal = 0;
			delay(100);

			// Wait until we receive a number. Then display it.
			while (dispVal <= 0 || dispVal >= 100)
			{
				dispVal = Serial.read();
				delay(30);
			}

			paused = true;
			matrixWrite(dispVal);
		}
		else if (cmd == 99)
			handleComs(getSysHour(), getSysMin(), getSysSec(), getBPM());
		else if (cmd == 111)
			receiveClock();
		// else if (cmd == 101)
		// 	printADJD_S311Values();

		cmd = 0;
	}
}