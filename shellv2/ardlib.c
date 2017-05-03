/*
 * @auth: Danny Nsouli and Tad Miller
 * @desc: This program extends the shell of part 1. It incorporates a data
 *        visualizer.
 * 
 */
 // http://www.wassen.net/sqlite-c.html

#include "ardlib.h"

int fd;
bool readingBuffer = false;
bool quitFork = false;

// Initialize the connection to the Arduino.
int init_tty(int fd)
{
	struct termios tty;

	// Configure the serial port.
	memset(&tty, 0, sizeof(tty));

	// Get a reference to options for the tty
	if (tcgetattr(fd, &tty) == -1)
		return -1;

	// Set the baud rate to 9600 (same as on Arduino)
	if (cfsetospeed(&tty, (speed_t) B9600) == -1)
		return -1;

	if (cfsetispeed(&tty, (speed_t) B9600) == -1)
		return -1;

	// 8 bits, no parity, no stop bits
	tty.c_cflag &= ~PARENB;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;

	// No flow control
	tty.c_cflag &= ~CRTSCTS;

	// Set local mode and enable the receiver
	tty.c_cflag |= (CLOCAL | CREAD);

	// Disable software flow control
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);

	// Make raw
	tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	tty.c_oflag &= ~OPOST;

	// Infinite timeout and return from read() with >1 byte available
	tty.c_cc[VMIN]  = 0;
	tty.c_cc[VTIME] = 0;

	// Update options for the port
	if (tcsetattr(fd, TCSANOW, &tty) == -1)
		return -1;

	return 0;
}

// Send a byte to the Arduino. Read the response into the character array "buf".
char *send_byte(char send)
{
	if (fd == 0)
		return NULL;

	int count = 0;
	char *buffer = NULL;

	while (readingBuffer == true)
	{
		//printf("\nWaiting for thread to finish reading buffer...");
		usleep(1000 * 1000);
	}

	readingBuffer = true;
	count = write(fd, &send, 1);

	if (count == -1)
	{
		//printf("\nWrite error\n");
		close(fd);

		readingBuffer = false;
		return NULL;
	}
	else if (count == 0)
	{
		//printf("\nNo data written\n");
		close(fd);

		readingBuffer = false;
		return NULL;
	}

	// Wait for data to transmit
	sleep(1);

	// Read the response
	buffer = malloc(sizeof(char) * BUFFER_SIZE);
	count = read(fd, buffer, BUFFER_SIZE - 1);

	if (count < 1) 
	{
		// Read error. Device connection closed
		if (count < 0)
			close(fd);
			//printf("\nNULL");

		free(buffer);

		readingBuffer = false;
		return NULL;
	}

	readingBuffer = false;

	// Ensure the response is null-terminated
	buffer[count] = 0;

	if (buffer != NULL)
		return buffer;

	//printf("\nData malformed or no data returned\n");
	free(buffer);
	return NULL;
}

char *arduino_rate(bool keep)
{
	char *bpm = send_byte('b');

	if (keep)
		return bpm;

	if (bpm != NULL)
	{
		bpm[2] = '\0';
		printf("\nBPM: %s\n", bpm);
		free(bpm);
	}
	else
		printf("\nBPM is NULL\n");

	return NULL;
}

// Sends char r to arduino to provoke actions on display
void arduino_resume()
{
	send_byte('r');

	printf("\nResumed.\n");
}

// Sends char p to arduino to provoke actions on display
void arduino_pause()
{
	send_byte('p');

	printf("\nPaused.\n");
}

// Sends char s to arduino to provoke actions on display
void arduino_show(char **args)
{
	int num = *(args + 1) != NULL ? atoi(*(args + 1)) : 0;

	send_byte('s');
	send_byte(num);

	printf("\nDone.\n");
}

/*
**ConnectArduino********************************************************************
*/

int arduino_connect(char **args)
{   // Read the device path from input, or default to /dev/ttyACM0   
	char *device = *(args + 1) != NULL ? *(args + 1) : "/dev/ttyACM0";

	printf("\nConnecting to %s\n", device);
	/*
	 * Need the following flags to open:
	 * O_RDWR: to read from/write to the devices
	 * O_NOCTTY: Do not become the process's controlling terminal
	 * O_NDELAY: Open the resource in nonblocking mode
	 */
	fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
	{
		printf("\nError opening serial\n");
		return -1;
	}

	/* Configure settings on the serial port */
	if (init_tty(fd) == -1)
	{
		printf("\nFailed init_tty\n");
		close(fd);
		return -1;
	}

	/* Wait for Arduino to reboot */
	usleep(1000 * 1000);

	/* Flush whatever is remaining in the buffer */
	tcflush(fd, TCIFLUSH);

	printf("\nSuccessfully connected to device\n");

	usleep(1000 * 1000);

	fflush(stdin);
	fflush(stdout);
	tcflush(fd, TCIFLUSH);

	return 0;
}

void arduino_close()
{
	tcflush(fd, TCIFLUSH);

	close(fd);
}

char *arduino_env(bool keep)
{
	char *env = send_byte('e');

	if (keep)
		return env;

	printf("\nRGB SENSOR: %s", env);
	free(env);

	return NULL;
}

void sys_exit()
{
	arduino_close();

	printf("\n\n\n\n");
	exit(0);
}

// Data visualizer. Reads data from Arduino and stores into 10 character array.
// Gets sent to mmap'd file after 10 reads.
void process_rate()
{
	while (!quitFork)
	{
		char *res = arduino_rate(true);

		mmap_write(res, NULL, 'A');

		free(res);

		usleep(1000 * 1000 * 3);
	}
}

void arduino_clock(char **args)
{
	char *time;

	if (*(args + 1) != NULL)
		if (strcmp(*(args + 1), "sync") == 0)
			arduino_clock_sync();

	time = send_byte('t');

	printf("\n%s\n", time);

	free(time);
}

void arduino_clock_sync()
{
	time_t t;
	struct tm *curr_time;

	time(&t);
	curr_time = localtime(&t);

	send_byte('c');

	printf("\nSyncing hour...");

	send_byte(curr_time -> tm_hour + 32);
	send_byte('h');

	printf("\nSyncing minute...");

	send_byte(curr_time -> tm_min + 32);
	send_byte('m');

	printf("\nSyncing second...");

	send_byte(curr_time -> tm_sec + 32);
	char *time = send_byte('s');

	printf("\n%s", time);
}

void fork_heartrate()
{
	pid_t pid = fork();

	if (pid == 0)
		return;
	else
		process_rate();
}