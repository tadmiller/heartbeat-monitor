/**************************/
/* RGB LED DISPLAY COLORS */
/**************************/

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
byte default_color = RED; // What color the display is by default
byte tmpDisplay[8][8] = {{4, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}};
/**************************/

// Initialize the matrix. Might need to be in an object eventually
void initMatrix()
{
    pinMode(clock, OUTPUT); // sets the digital pin as output 
    pinMode(data, OUTPUT); 
    pinMode(cs, OUTPUT); 

    byte start[8][8];

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            start[i][j] = default_color;

    updateDisplay(start);
}

void updateDisplay(byte frame[8][8])
{
    displayFrame(frame);
    delay(20);
    displayFrame(frame);
    delay(20);
    displayFrame(frame);
}

//used to change frame, constantly updated when needed
void displayFrame(byte frame[8][8])  //draws frame on 8x8 matrix
{
    Serial.flush();
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
