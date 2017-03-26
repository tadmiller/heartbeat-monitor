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

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded!
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.
char cmd = 0;
boolean paused = false;
int showValue = 39;

/******************/
/* GAME MECHANICS */
/******************/

// SET THE SERIAL OUTPUT TYPE TO YOUR NEEDS
// PROCESSING_VISUALIZER works with Pulse Sensor Processing Visualizer
//      https://github.com/WorldFamousElectronics/PulseSensor_Amped_Processing_Visualizer
// SERIAL_PLOTTER outputs sensor data for viewing with the Arduino Serial Plotter
//      run the Serial Plotter at 115200 baud: Tools/Serial Plotter or Command+L
static int outputType = SERIAL_PLOTTER;

void setup()
{
    init();
    Serial.begin(9600);

    // Initialize RGB LED matrix
    initMatrix(); // Load 8x8 RGB Matrix device drivers
    interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS
}

void loop()
{
    serialOutput();

    if (Serial.available() > 0)
        cmd = Serial.read();

    //matrixWrite((int) cmd);
    //Serial.println("BYTE IS: " + cmd);
    
    if (QS == true && cmd == 0 && !paused)
    {
        matrixWrite(BPM > 99 ? 99 : BPM);

        serialOutputWhenBeatHappens();   // A Beat Happened, Output that to serial.
        QS = false;                      // reset the Quantified Self flag for next time
    }
    else
    {
        Serial.print("CMD != 0 (");
        Serial.print(String(cmd));
        Serial.println(")\n");
        
        if (cmd == 112)
            paused = true;
        else if (cmd == 114)
            paused = false;
        else if (cmd == 115)
        {
            while (showValue == 0 || showValue == 115)
            {
                showValue = Serial.read();
                delay(10);
            }
            
            matrixWrite(showValue);
            paused = true;
        }

        cmd = 0;
    }

    delay(20);
}

//void setup() {
//  Serial.begin(9600);
//  
//  while (!Serial);
//  // wait for serial port to connect
//}
//
//void loop() {
//        char incomingByte;
//        // send data only when you receive data:
//        if (Serial.available() > 0) {
//                incomingByte = Serial.read();
//                Serial.print("Got: ");
//                Serial.println(incomingByte);
//        }
//        
//}

