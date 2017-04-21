#include "rgblib.h"

byte disp[8][8] = {{clr, clr, clr, clr, clr, clr, clr, clr}, {clr, clr, clr, clr, clr, clr, clr, clr}, {clr, clr, clr, clr, clr, clr, clr, clr}, {clr, clr, clr, clr, clr, clr, clr, clr}, {clr, clr, clr, clr, clr, clr, clr, clr}, {clr, clr, clr, clr, clr, clr, clr, clr}, {clr, clr, clr, clr, clr, clr, clr, clr}, {clr, clr, clr, clr, clr, clr, clr, clr}};
byte tmpDisp[8][8];

// Initialize the matrix. Might need to be in an object eventually
void initMatrix()
{
	pinMode(PIN_RGB_SCLK, OUTPUT); // sets the digital pin as output 
	pinMode(PIN_RGB_MOSI, OUTPUT); 
	pinMode(PIN_RGB_CS, OUTPUT);

	digitalWrite(PIN_RGB_SCLK, HIGH);  //sets the PIN_RGB_SCLK for each display, running through 0 then 1
	digitalWrite(PIN_RGB_CS, HIGH);     //ditto for PIN_RGB_CS.

	memcpy(tmpDisp, disp, sizeof(disp));
	updateDisplay(disp);
}

// Given a number, write it to the screen. Must be two digits.
void matrixWrite(int num)
{
	memcpy(tmpDisp, disp, sizeof(disp));
	drawChar(((num / 10) % 10), 0, 1);
	drawChar(num % 10, 4, 1);
	updateDisplay(tmpDisp);
}

// Method to draw a letter/number at xy coords
void drawChar(char c, int x, int y)
{
	// This switch statement handles nums 0 - 9. Can be augmented to include A-Z. Seems to be the best way I could think of to do this.
	switch(c)
	{
		case 0:
			placeDots(x, y, 5, 1, 1, 1, 1, 1);
			placeDots(x + 1, y, 5, 1, 0, 0, 0, 1);
			placeDots(x + 2, y, 5, 1, 1, 1, 1, 1);

			break;
		case 1:
			placeDots(x, y, 5, 1, 0, 0, 0, 1);
			placeDots(x + 1, y, 5, 1, 1, 1, 1, 1);
			placeDots(x + 2, y, 5, 0, 0, 0, 0, 1);

			break;
		case 2:
			placeDots(x, y, 5, 1, 0, 1, 1, 1);
			placeDots(x + 1, y, 5, 1, 0, 1, 0, 1);
			placeDots(x + 2, y, 5, 1, 1, 1, 0, 1);

			break;
		case 3:
			placeDots(x, y, 5, 1, 0, 1, 0, 1);
			placeDots(x + 1, y, 5, 1, 0, 1, 0, 1);
			placeDots(x + 2, y, 5, 1, 1, 1, 1, 1);

			break;
		case 4:
			placeDots(x, y, 5, 1, 1, 1, 0, 0);
			placeDots(x + 1, y, 5, 0, 0, 1, 0, 0);
			placeDots(x + 2, y, 5, 1, 1, 1, 1, 1);

			break;
		case 5:
			placeDots(x, y, 5, 1, 1, 1, 0, 1);
			placeDots(x + 1, y, 5, 1, 0, 1, 0, 1);
			placeDots(x + 2, y, 5, 1, 0, 1, 1, 1);

			break;
		case 6:
			placeDots(x, y, 5, 1, 1, 1, 1, 1);
			placeDots(x + 1, y, 5, 1, 0, 1, 0, 1);
			placeDots(x + 2, y, 5, 1, 0, 1, 1, 1);
			
			break;
		case 7:
			placeDots(x, y, 5, 1, 0, 0, 0, 0);
			placeDots(x + 1, y, 5, 1, 0, 0, 0, 0);
			placeDots(x + 2, y, 5, 1, 1, 1, 1, 1);

			break;
		case 8:
			placeDots(x, y, 5, 1, 1, 1, 1, 1);
			placeDots(x + 1, y, 5, 1, 0, 1, 0, 1);
			placeDots(x + 2, y, 5, 1, 1, 1, 1, 1);

			break;
		case 9:
			placeDots(x, y, 5, 1, 1, 1, 0, 1);
			placeDots(x + 1, y, 5, 1, 0, 1, 0, 1);
			placeDots(x + 2, y, 5, 1, 1, 1, 1, 1);

			break;
	}
}

// This function takes dynamic arguments
// The args are used as binary numbers in correlation to the coordinates to determine whether to place a dot or not.
void placeDots(int x, int y, int num, ...)
{
	va_list arguments;
	va_start(arguments, num);
	
	for (byte b = 0; b < num && b < 8; b++)
		if (x < 8 && y + b < 8)
			va_arg(arguments, int) == 1 ? tmpDisp[x][y + b] = txt : true;
		else
			break;
			
	va_end(arguments);
}

void updateDisplay(byte frame[8][8]) //used to change frame, constantly updated when needed
{
	drawFrame(frame);
	delay(20);
	drawFrame(frame);
	delay(20);
	drawFrame(frame);
}

void drawFrame(byte frame[8][8])  //draws frame on 8x8 matrix
{
	digitalWrite(PIN_RGB_SCLK, LOW);  //sets the PIN_RGB_SCLK for each display, running through 0 then 1
	digitalWrite(PIN_RGB_MOSI, LOW);   //ditto for PIN_RGB_MOSI.
	delayMicroseconds(10);
	digitalWrite(PIN_RGB_CS, LOW);     //ditto for PIN_RGB_CS.
	delayMicroseconds(10);
	
	for(int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			// Drawing the grid. x across then down to next y then x across.
			writeByte(frame[x][y]);  
			delayMicroseconds(10);
		}
	}
	
	delayMicroseconds(10);
	digitalWrite(PIN_RGB_CS, HIGH);
}

// prints out bytes. Each colour is printed out.
void writeByte(byte myByte)
{
	for (int b = 0; b < 8; b++)
	{  // converting it to binary from colour code.
		digitalWrite(PIN_RGB_SCLK, LOW);
		
		if ((myByte & bits[b]) > 0)
			digitalWrite(PIN_RGB_MOSI, HIGH);
		else
			digitalWrite(PIN_RGB_MOSI, LOW);
			
		digitalWrite(PIN_RGB_SCLK, HIGH); 
		delayMicroseconds(10);
		digitalWrite(PIN_RGB_SCLK, LOW); 
	}
}