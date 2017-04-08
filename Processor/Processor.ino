/*/
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
	Serial.begin(9600);
	//init();
	initMatrix();   // Initialize RGB Matrix
	initClock();    // Initialize clock
	initHeartbeat(); // sets up to read Pulse Sensor signal every 2mS
	//pinMode(8, OUTPUT);
}

void loop()
{
	updateClock();
	updateCmd();
	delay(20);

	// Allows us to make the LED flash at the rate of the heart beat
//    if (showValue > 0 && paused)
//        delay(20 + (21000 / showValue));
//    else if (BPM == 0 || paused)
//        delay(100);
//    else
//        delay(20 + (21000 / BPM));

	// I like shorthands
	// This one checks to see that the BPM is > 0 OR we're showing something on the screen. Otherwise we don't blink.
	//digitalWrite(8, BPM > 0 || (showValue > 0 && paused) ? ledStatus = !ledStatus : LOW);
}
