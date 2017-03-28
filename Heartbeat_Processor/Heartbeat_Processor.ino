/*
 * @name Heartbeat_Processor.ino 
 * @vers 0.21
 * @auth Theodore Miller, Danny Nsouli
 * @desc This code reads data from the heartbeat monitor
 *       and displays it on the RGB 8x8 LED matrix. It can
 *       also receive commands from a C backend running on
 *       a UNIX OS.
 *       Credit to Joel Murphy and Yury Gitman for the
 *       original code to read the analog data from the heartbeat 
 *       monitor. Link: http://www.pulsesensor.com
 * 
 * 
 ******************/

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
boolean ledStatus = false;
int showValue = 0;
static int outputType = SERIAL_PLOTTER;

void setup()
{
    init();
    Serial.begin(9600);

    // Initialize RGB LED matrix
    initMatrix();
    
    interruptSetup(); // sets up to read Pulse Sensor signal every 2mS
    pinMode(8, OUTPUT);
}

void loop()
{
    // Check and see if we can read a command.
    if (Serial.available() > 0)
        cmd = Serial.read();

    // If there's a new pulse, and we haven't received a command, and we're not paused from the cmd prompt
    if (QS == true && cmd == 0 && !paused)
    {
        serialOutput();
        BPM = BPM > 99 ? 99 : BPM; // We can't send values higher than 99 since the display is only two numbers.

        //serialOutputWhenBeatHappens();   // A Beat Happened, Output that to serial.
        QS = false;                      // reset the Quantified Self flag for next time

        // Then we write our new BPM to the display
        matrixWrite(BPM);
    }
    else
    {   // 112 == 'p' which means we pause the program state.
        if (cmd == 112)
            paused = true;
        else if (cmd == 114) // 114 = 'r' which means we resume the program state.
            paused = false;
        else if (cmd == 115) // 115 is 's' which means we're going to display the number that comes after
        {
            showValue = 0;
            delay(100);

            // Wait until we receive a number. Then display it.
            while (showValue <= 0 || showValue >= 100)
            {
                showValue = Serial.read();
                delay(30);
            }

            BPM = showValue;
            paused = true;
            matrixWrite(BPM);
        }

        cmd = 0;
    }

    // Allows us to make the LED flash at the rate of the heart beat
    if (showValue > 0 && paused)
        delay(20 + (21000 / showValue));
    else if (BPM == 0 || paused)
        delay(100);
    else
        delay(20 + (21000 / BPM));

    // I like shorthands
    // This one checks to see that the BPM is > 0 OR we're showing something on the screen. Otherwise we don't blink.
    digitalWrite(8, BPM > 0 || (showValue > 0 && paused) ? ledStatus = !ledStatus : LOW);
}
