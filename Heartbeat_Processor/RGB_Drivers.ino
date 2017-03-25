#include <stdio.h>
#include <stdarg.h>

/**************************/
/* RGB LED DISPLAY COLORS */
/**************************/

#define BLACK 0
#define BLUE 2
#define ORANGE 100
#define RED 32
#define GREEN 8
#define WHITE 122
#define YELLOW 40

/**************************/
/*    RGB DISPLAY VARS    */
/**************************/
int bits[8] = {128, 64, 32, 16, 8, 4, 2, 1};
int clock = 11; // Pin SCK del display
int data = 13;  // Pin DI del display
int cs = 12;    // Pin CS del display
byte clr = BLUE; // What color the display is by default
byte txt = WHITE;
byte disp[8][8] = {{clr, clr, clr, clr, clr, clr, clr, clr}, {clr, clr, clr, clr, clr, clr, clr, clr}, {clr, clr, clr, clr, clr, clr, clr, clr}, {clr, clr, clr, clr, clr, clr, clr, clr}, {clr, clr, clr, clr, clr, clr, clr, clr}, {clr, clr, clr, clr, clr, clr, clr, clr}, {clr, clr, clr, clr, clr, clr, clr, clr}, {clr, clr, clr, clr, clr, clr, clr, clr}};
byte tmpDisp[8][8];

//byte nums[10][4][8] = {{4, 2, 2, 2, 2, 2, 2, 4}, {2, 2, 2, 2, 2, 2, 2, 2}, {txt, txt, txt, txt, txt, clr, clr, clr}, {2, 2, 2, 2, 2, 2, 2, 2}};
/**************************/

// Initialize the matrix. Might need to be in an object eventually
void initMatrix()
{
    pinMode(clock, OUTPUT); // sets the digital pin as output 
    pinMode(data, OUTPUT); 
    pinMode(cs, OUTPUT);

    digitalWrite(clock, HIGH);  //sets the clock for each display, running through 0 then 1
    digitalWrite(cs, HIGH);     //ditto for cs.

    memcpy(tmpDisp, disp, sizeof(disp));
    updateDisplay(disp);
}

void matrixWrite(int num)
{
    memcpy(tmpDisp, disp, sizeof(disp));
    drawChar(((num / 10) % 10), 0, 2);
    drawChar(num % 10, 4, 2);
    updateDisplay(tmpDisp);
}

// Method to draw a letter/number at xy coords
void drawChar(char c, int x, int y)
{
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
    digitalWrite(clock, LOW);  //sets the clock for each display, running through 0 then 1
    digitalWrite(data, LOW);   //ditto for data.
    delayMicroseconds(10);
    digitalWrite(cs, LOW);     //ditto for cs.
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
    digitalWrite(cs, HIGH);
}

// prints out bytes. Each colour is printed out.
void writeByte(byte myByte)
{
    for (int b = 0; b < 8; b++)
    {  // converting it to binary from colour code.
        digitalWrite(clock, LOW);
        
        if ((myByte & bits[b]) > 0)
            digitalWrite(data, HIGH);
        else
            digitalWrite(data, LOW);
            
        digitalWrite(clock, HIGH); 
        delayMicroseconds(10);
        digitalWrite(clock, LOW); 
    }
}
