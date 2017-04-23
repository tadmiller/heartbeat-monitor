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

// Initialize the connection to the Arduino.
int init_tty(int fd)
{
	struct termios tty;
	/*
	 * Configure the serial port.
	 * First, get a reference to options for the tty
	 * Then, set the baud rate to 9600 (same as on Arduino)
	 */
	memset(&tty, 0, sizeof(tty));
	if (tcgetattr(fd, &tty) == -1)
	{
		perror("tcgetattr");
		return -1;
	}

	if (cfsetospeed(&tty, (speed_t)B9600) == -1)
	{
		perror("ctsetospeed");
		return -1;
	}
	if (cfsetispeed(&tty, (speed_t)B9600) == -1)
	{
		perror("ctsetispeed");
		return -1;
	}

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
	{
		perror("tcsetattr");
		return -1;
	}

	return 0;
}

// Send a byte to the Arduino. Read the response into the character array "buf".
char *send_byte(char send)
{
	if (fd == 0)
		return NULL;

	printf("\nFD is: %d\n", fd);

	int count = 0;
	char *buffer = NULL;

	while (readingBuffer == true)
	{
		printf("\nWaiting for thread to finish reading buffer...");
		usleep(1000 * 1000);
	}

	printf("\nsending: %d", send);
	readingBuffer = true;
	count = write(fd, &send, 1);

	printf("\ndone write");

	if (count == -1)
	{
		printf("\nWrite error\n");
		close(fd);

		readingBuffer = false;
		return NULL;
	}
	else if (count == 0)
	{
		printf("\nNo data written\n");
		close(fd);

		readingBuffer = false;
		return NULL;
	}

	// Wait for data to transmit
	sleep(1);

	printf("\nBuilding response\n");

	// Read the response
	buffer = malloc(sizeof(char) * BUFFER_SIZE);
	printf("\nReading...");
	count = read(fd, buffer, BUFFER_SIZE - 1);

	printf("\nDone read.");
	if (count == -1) 
	{
		printf("\nRead error. Device connection closed\n");
		close(fd);
		free(buffer);

		readingBuffer = false;
		return NULL;
	}
	else if (count == 0)
	{
		printf("\nNull response\n");
		free(buffer);

		readingBuffer = false;
		return NULL;
	}

	printf("Count is: %d", count);
	readingBuffer = false;

	// Ensure the response is null-terminated
	buffer[count] = 0;

	if (buffer != NULL)
	{
		printf("Returning: %s", buffer);
		return buffer;
	}

	printf("\nData malformed or no data returned\n");
	free(buffer);
	return NULL;
}

char *arduino_rate(bool keep)
{
	char *bpm = send_byte('c');

	if (keep)
		return bpm;

	printf("BPM: %s", bpm);
	free(bpm);

	return NULL;
}

// Sends char r to arduino to provoke actions on display
void arduino_resume()
{
	send_byte('r');
}

// Sends char p to arduino to provoke actions on display
void arduino_pause()
{
	send_byte('p');
}

// Sends char s to arduino to provoke actions on display
void arduino_show(char **args)
{
	int num = *(args + 1) != NULL ? atoi(*(args + 1)) : 0;

	printf("num is: %d:%d\n", num, fd);

	send_byte('s');
	send_byte(num);

	printf("\ndone show\n");
}

// Data visualizer. Reads data from Arduino and stores into 10 character array.
// Gets sent to mmap'd file after 10 reads.
void process_rate()
{
	/*int i;
	int count = 0;
	int bpms[10];
	int hours[10];
	int mins[10];
	int secs[10];

	int bpm;
	int hour;
	int min;
	int sec;

	while (1)
	{
		char *buffer = send_byte('c');

		i = 0;

		// // Read the message count. Can be used to retransmit data from Arduino
		// while (buf[i] != 'C')
		//     i++;
		// msg = buf[i + 2];

		// Read the bpm
		while (buffer[i] != 'B')
			i++;
		bpm = (int) buffer[i + 2];

		// Read the hour of the BPM recorded
		while (buffer[i] != 'H')
			i++;
		hour = (int) buffer[i + 2] - 32;

		// Read the minute of the BPM recorded
		while (buffer[i] != 'M')
			i++;
		min = (int) buffer[i + 2] - 32;
		
		// Read the second of the BPM recorded
		while (buffer[i] != 'S')
			i++;
		sec = (int) buffer[i + 2] - 32;

		//printf("\nBPM: %d at %d:%d:%d", bpm, hour, min, sec);

		hours[count] = hour;
		mins[count] = min;
		secs[count] = sec;
		bpms[count] = bpm;

		count++;

		// If we have 10 records, print out a histogram. Reset the count to zero.
		if (count == 10)
		{
			//printHistogram(bpms, hours, mins, secs);
			mmap_write(bpms, hours, mins, secs);
			count = 0;
		}

		usleep(1000 * 1000 * 3);
	}*/
}

void flush()
{
	fflush(stdin);
	fflush(stdout);
	send_byte('c');
	fflush(stdin);
	fflush(stdout);
	tcflush(fd, TCIFLUSH);

	usleep(1000 * 1000 * 2);

	fflush(stdin);
	fflush(stdout);
	send_byte('c');
	fflush(stdin);
	fflush(stdout);
	tcflush(fd, TCIFLUSH);
}

/*
**ConnectArduino********************************************************************
*/

int arduino_connect(char **args)
{   // Read the device path from input, or default to /dev/ttyACM0   
	char *device = *(args + 1) != NULL ? *(args + 1) : "/dev/ttyACM0";

	printf("Connecting to %s\n", device);
	/*
	 * Need the following flags to open:
	 * O_RDWR: to read from/write to the devices
	 * O_NOCTTY: Do not become the process's controlling terminal
	 * O_NDELAY: Open the resource in nonblocking mode
	 */
	fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
	{
		printf("\nError opening serial");
		return -1;
	}

	/* Configure settings on the serial port */
	if (init_tty(fd) == -1)
	{
		printf("\nFailed init_tty");
		close(fd);
		return -1;
	}

	/* Wait for Arduino to reboot */
	usleep(1000 * 1000);

	/* Flush whatever is remaining in the buffer */
	tcflush(fd, TCIFLUSH);

	printf("\nSuccessfully connected to device");

	//flush();

	return 0;
}

void arduino_close()
{
	tcflush(fd, TCIFLUSH);

	close(fd);
}

void read_response(char* txt, int len, char* buff, int buffLen, char identifier)
{
	for (size_t i = 0; i < buffLen; i++)
	{
		if (buff[i] == identifier)
		{
			if (i + 1 < buffLen)
			{
				if (buff[i + 1] == '[')
					break;
			}
			else
				printf("\nMessage cut off");
		}
	}
}

void arduino_heartrate()
{
	int i = 0;

	int bpm;
	int hour;
	int min;
	int sec;

	char *buffer = send_byte('c');

	// Read the bpm
	while (buffer[i] != 'B')
		i++;
	bpm = (int) buffer[i + 2];

	// Read the hour of the BPM recorded
	while (buffer[i] != 'H')
		i++;
	hour = (int) buffer[i + 2] - 32;

	// Read the minute of the BPM recorded
	while (buffer[i] != 'M')
		i++;
	min = (int) buffer[i + 2] - 32;
	
	// Read the second of the BPM recorded
	while (buffer[i] != 'S')
		i++;
	sec = (int) buffer[i + 2] - 32;
	free(buffer);

	fflush(stdin);
	fflush(stdout);
	//printf("%s", buffer);
	printf("\n%.3d BPM at %.2d:%.2d:%.2d", bpm, hour, min, sec);
	printf("\n\n");
}

void arduino_env()
{
	char *buffer = send_byte('e');

	printf("\n%.25s", buffer);
	free(buffer);
	printf("\n\n");
}

void sys_exit()
{
	arduino_close();

	printf("\n\n\n\n");
	exit(0);
}

void get_hist()
{
	//scanf("%s", r_time);

	//mmap_read();
}

// void inputCmd()
// {
//     char input[30];


//     while(1)    //infinitely looping while
//     {
//         printf("processor/shell$ ");  //ask user to enter command
//         scanf("%s", input); 

//         if (strcmp(input, "show") == 0) //if input is show X then start show method
//             show();
//         else if (strcmp(input, "pause") == 0) //if input is pause then start pause method
//             pauseProg();
//         else if (strcmp(input, "resume") == 0)   //if input is resume then start resume method
//             resume();
//         else if (strcmp(input, "rate") == 0)
//             arduinoRate();
//         else if (strcmp(input, "env") == 0)
//             arduinoEnv();
//         else if (strcmp(input, "hist") == 0)
//             getHist();
//         else if (strcmp(input, "reset") == 0)
//             clearFile();
//         else if(strcmp(input, "q") == 0 || strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0 || strcmp(input, "x") == 0) //if exit or quit then exit program
//             sysExit();
//         else if (strcmp(input, "c") == 0 || strcmp(input, "connect") == 0)
//             arduinoConnect();
//         else if (strcmp(input, "close") == 0)
//             arduinoClose();
//         else if (strcmp(input, "flush") == 0)
//             flush();
//         //else
//         //    process_rate();
//     }
//    // printf("You exited the program\n");
   
// }

void arduino_clock(char **args)
{
	char *time;

	if (*(args + 1) != NULL)
		if (strcmp(*(args + 1), "sync") == 0)
			arduino_clock_sync();

	time = send_byte('t');

	printf("%s", time);

	free(time);
}

void arduino_clock_sync()
{
	time_t t;
	struct tm *curr_time;

	time(&t);
	curr_time = localtime(&t);

	send_byte('c');

	printf("\nSending hour...");

	send_byte('h');
	send_byte(curr_time -> tm_hour);

	printf("\nSending minute...");

	send_byte('m');
	send_byte(curr_time -> tm_min);

	printf("\nSending second...");

	send_byte('s');
	send_byte(curr_time -> tm_sec);
}

void fork_heartrate()
{
	pid_t pid = fork();

	if (pid == 0)
		return;
	else
		process_rate();
}