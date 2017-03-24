/*  Pulse Sensor Amped 1.5    by Joel Murphy and Yury Gitman   http://www.pulsesensor.com

----------------------  Notes ----------------------  ----------------------
This code:
1) Blinks an LED to User's Live Heartbeat   PIN 13
2) Fades an LED to User's Live HeartBeat    PIN 5
3) Determines BPM
4) Prints All of the Above to Serial

Read Me:
https://github.com/WorldFamousElectronics/PulseSensor_Amped_Arduino/blob/master/README.md
 ----------------------       ----------------------  ----------------------
*/

#define PROCESSING_VISUALIZER 1
#define SERIAL_PLOTTER  2

//  Variables
int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 13;                // pin to blink led at each beat
int fadePin = 5;                  // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded!
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

/******************/
/* GAME MECHANICS */
/******************/
bool shipsPlaced = false;
byte myShipsDisplay[8][8] = {{2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}};
byte tmpDisplay[8][8] = {{2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}};
/************************/
/* RGB LED DISPLAY VARS */
/************************/
#define BLUE 2
#define ORANGE 100
#define RED 64
#define GREEN 8
#define WHITE 110

/*********************/
/*    RGB DISPLAY    */
/*********************/
int bits[8] = {128, 64, 32, 16, 8, 4, 2, 1};
int clock = 11; // Pin SCK del display
int data = 13;  // Pin DI del display
int cs = 12;    // Pin CS del display
/******/

// SET THE SERIAL OUTPUT TYPE TO YOUR NEEDS
// PROCESSING_VISUALIZER works with Pulse Sensor Processing Visualizer
//      https://github.com/WorldFamousElectronics/PulseSensor_Amped_Processing_Visualizer
// SERIAL_PLOTTER outputs sensor data for viewing with the Arduino Serial Plotter
//      run the Serial Plotter at 115200 baud: Tools/Serial Plotter or Command+L
static int outputType = SERIAL_PLOTTER;

// Initialize the matrix. Might need to be in an object eventually
void initMatrix()
{
    pinMode(clock, OUTPUT); // sets the digital pin as output 
    pinMode(data, OUTPUT); 
    pinMode(cs, OUTPUT); 

    updateDisplay(tmpDisplay);
}

void setup()
{
    init();
    Serial.begin(9600);

    // Initialize RGB LED matrix
    initMatrix(); // Load 8x8 RGB Matrix device drivers

        //  cpyTmpDisplay();
        //displayDots(row, col, orientation, size);
        updateDisplay(tmpDisplay);
    pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
    pinMode(fadePin,OUTPUT);          // pin that will fade to your heartbeat!
    Serial.begin(115200);             // we agree to talk fast!
    interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS
    // IF YOU ARE POWERING The Pulse Sensor AT VOLTAGE LESS THAN THE BOARD VOLTAGE,
    // UN-COMMENT THE NEXT LINE AND APPLY THAT VOLTAGE TO THE A-REF PIN
    //   analogReference(EXTERNAL);
}


//  Where the Magic Happens
void loop()
{
    serialOutput();

    if (QS == true)
    {     // A Heartbeat Was Found
                       // BPM and IBI have been Determined
                       // Quantified Self "QS" true when arduino finds a heartbeat
        fadeRate = 255;         // Makes the LED Fade Effect Happen
                                // Set 'fadeRate' Variable to 255 to fade LED with pulse
        serialOutputWhenBeatHappens();   // A Beat Happened, Output that to serial.
        QS = false;                      // reset the Quantified Self flag for next time
    }

    ledFadeToBeat();                      // Makes the LED Fade Effect Happen
    delay(20);                             //  take a break
}

void ledFadeToBeat()
{
    fadeRate -= 15;                         //  set LED fade value
    fadeRate = constrain(fadeRate,0,255);   //  keep LED fade value from going into negative numbers!
    analogWrite(fadePin,fadeRate);          //  fade LED
}

void updateDisplay(byte frame[8][8]) //used to change frame, constantly updated when needed
{
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

/**
 * void setup() {
  Serial.begin(9600);
  
  while (!Serial);
  // wait for serial port to connect
}

void loop() {
        char incomingByte;
        // send data only when you receive data:
        if (Serial.available() > 0) {
                incomingByte = Serial.read();
                Serial.print("Got: ");
                Serial.println(incomingByte);
        }
        
}*/
 */
