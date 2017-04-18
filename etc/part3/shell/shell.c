/*
 * @auth: Danny Nsouli and Tad Miller
 * @desc: This program extends the shell of part 1. It incorporates a data
 *        visualizer.
 * 
 */

#define _XOPEN_SOURCE 500
#define CRTSCTS  020000000000
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <stdint.h>
#include <sys/mman.h>
#include <stdbool.h>

int fd;
int count;
char *device;
char byte;
char buf[32];
char r_time[8];
int count = 0;
int msg;
int bpm;
int hour;
int min;
int sec;
bool readingBuffer = false;

// Open and append 10 BPMs to a file
int mmap_write(int bpms[10], int hours[10], int mins[10], int secs[10])
{
    // Our file set
    const char *filepath = "/tmp/histogram.csv";

    int fd = open(filepath, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    
    if (fd == -1)
    {
        perror("Error opening file for writing");
        return 1;
        //exit(EXIT_FAILURE);
    }

    // Stretch the file size to the size of the (mmapped) array of char

    size_t textsize = 140; // + \0 null character
    
    if (lseek(fd, textsize-1, SEEK_SET) == -1)
    {
        close(fd);
        perror("Error calling lseek() to 'stretch' the file");
        return 1;
        //exit(EXIT_FAILURE);
    }
    
    if (write(fd, "", 1) == -1)
    {
        close(fd);
        perror("Error writing last byte of the file");
        return 1;
        //exit(EXIT_FAILURE);
    }

    // Now the file is ready to be mmapped.
    char *map = mmap(0, textsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED)
    {
        close(fd);
        perror("Error mmapping the file");
        return 1;
        //exit(EXIT_FAILURE);
    }
    
    int count = 0;
    char snum[14];

    if (bpms == NULL || hours == NULL || mins == NULL || secs == NULL)
    {
        fclose(fopen(filepath, "w"));
    }
    else
        for (size_t i = 0; i < 10; i++)
        {
            //printf("%.3d,%.2d:%.2d:%.2d,", bpms[i], hours[i], mins[i], secs[i]);
            sprintf(snum, "%.3d,%.2d:%.2d:%.2d,", bpms[i], hours[i], mins[i], secs[i]);

            if (i < 9)
                snum[13] = '\n';

            //printf("\n%s", snum);

            for (size_t j = count; j < count + 14; j++)
                map[j] = snum[j % 14];

            count += 14;
        }

    // Write it now to disk
    if (msync(map, textsize, MS_SYNC) == -1)
        perror("Could not sync the file to disk");
    
    // Don't forget to free the mmapped memory
    if (munmap(map, textsize) == -1)
    {
        close(fd);
        perror("Error un-mmapping the file");
        exit(EXIT_FAILURE);
    }

    // Un-mmaping doesn't close the file, so we still need to do that.
    close(fd);

    return 0;
}

void processHist(char *map)
{
    char bpm[4];
    char time[9];
    bool selected = true;
    //bool onbpm = true; // false means on time

    if (strcmp(r_time, "-1") == 0)
        printf("No time selected");
    else
        selected = false;

    //printf("%s", map);

    for (size_t i = 0; i < strlen(map); i += 14)
    {
        bpm[0] = map[i];
        bpm[1] = map[i + 1];
        bpm[2] = map[i + 2];
        bpm[3] = '\0';

        strncpy(time, map + i + 4, 8);
        time[10] = '\0';

        if (strcmp(r_time, time) == 0)
            selected = true;

        if (selected == true)
        {
            printf("\n%s bpm | %s | ", bpm, time);

            for (size_t j = 0; j < atoi(bpm); j++)
                printf("*");
        }
    }

    printf("\n");
}

int mmap_read()
{    
    const char *filepath = "/tmp/histogram.csv";

    int fd = open(filepath, O_RDONLY, (mode_t)0600);
    
    if (fd == -1)
    {
        perror("Error opening file for writing");
        return 1;
        //exit(EXIT_FAILURE);
    }        
    
    struct stat fileInfo = {0};
    
    if (fstat(fd, &fileInfo) == -1)
    {
        perror("Error getting the file size");
        return 1;
        //exit(EXIT_FAILURE);
    }
    
    if (fileInfo.st_size == 0)
    {
        fprintf(stderr, "Error: File is empty, nothing to do\n");
        return 1;
        //exit(EXIT_FAILURE);
    }
    
    //printf("File size is %ji\n", (intmax_t)fileInfo.st_size);
    
    char *map = mmap(0, fileInfo.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED)
    {
        close(fd);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }
    
    // for (off_t i = 0; i < fileInfo.st_size; i++)
    // {
    //     printf("Found character %c at %ji\n", map[i], (intmax_t)i);
    // }

    processHist(map);
    
    // Don't forget to free the mmapped memory
    if (munmap(map, fileInfo.st_size) == -1)
    {
        close(fd);
        perror("Error un-mmapping the file");
        exit(EXIT_FAILURE);
    }

    // Un-mmaping doesn't close the file, so we still need to do that.
    close(fd);
    
    return 0;
}

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
int sendBytes(char byte)
{
  // Write this letter of the alphabet
    while (readingBuffer)
        usleep(1000 * 1000);

    readingBuffer = true;
    count = write(fd, &byte, 1);
    if (count == -1)
    {
        perror("write");
        close(fd);
        return -1;
    }
    else if (count == 0)
    {
        fprintf(stderr, "No data written\n");
        close(fd);
        return -1;
    }

    // Wait for data to transmit
    sleep(1);

    // Read the response
    count = read(fd, &buf, 32);
    if (count == -1) 
    {
        perror("read");
        close(fd);
        return -1;
    }
    else if (count == 0)
    {
        //fprintf(stderr, "No data returned\n");
        //continue;
        return 0;
    }

    // Ensure the response is null-terminated
    buf[count] = 0;
    readingBuffer = false;
    //printf("(%d): %s", count, buf);
    return 0;
}

// Sends char r to arduino to provoke actions on display
void resume()
{
    printf("you resumed!!\n");
    sendBytes('r');
}

// Sends char p to arduino to provoke actions on display
void pauseProg()
{
    printf("you paused!!\n");
    sendBytes('p');
}

// Sends char s to arduino to provoke actions on display
void show()
{
    int num;
    sendBytes('s');
    //printf("Input what integer you want to be shown: ");
    scanf("%d", &num);
    sendBytes(num);
}

// Print a star histogram using the recorded dates and times.
void printHistogram(int bpms[10], int hours[10], int mins[10], int secs[10])
{
    int i;
    int j;

    for (i = 0; i < 10; i++)
    {
        printf("\n");
        
        printf(hours[i] < 10 ? "0%d" : "%d", hours[i]);
        printf(mins[i] < 10 ? ":0%d" : ":%d", mins[i]);
        printf(secs[i] < 10 ? ":0%d" : ":%d", secs[i]);

        printf(" | ");

        for (j = 0; j < bpms[i]; j++)
            printf("*");
    }
}

// Data visualizer. Reads data from Arduino and stores into 10 character array.
// Gets sent to mmap'd file after 10 reads.
void process_rate()
{
    int i;
    int count = 0;
    int bpms[10];
    int hours[10];
    int mins[10];
    int secs[10];

    while (1)
    {
        sendBytes('c');

        i = 0;

        // Read the message count. Can be used to retransmit data from Arduino
        while (buf[i] != 'C')
            i++;
        msg = buf[i + 2];

        // Read the bpm
        while (buf[i] != 'B')
            i++;
        bpm = (int) buf[i + 2];

        // Read the hour of the BPM recorded
        while (buf[i] != 'H')
            i++;
        hour = (int) buf[i + 2] - 32;

        // Read the minute of the BPM recorded
        while (buf[i] != 'M')
            i++;
        min = (int) buf[i + 2] - 32;
        
        // Read the second of the BPM recorded
        while (buf[i] != 'S')
            i++;
        sec = (int) buf[i + 2] - 32;

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
    }
}

void flush()
{
    fflush(stdin);
    fflush(stdout);
    sendBytes('c');
    fflush(stdin);
    fflush(stdout);
    tcflush(fd, TCIFLUSH);

    usleep(1000 * 1000 * 2);

    fflush(stdin);
    fflush(stdout);
    sendBytes('c');
    fflush(stdin);
    fflush(stdout);
    tcflush(fd, TCIFLUSH);
}

/*
**ConnectArduino********************************************************************
*/

int arduinoConnect()
{
    // Read the device path from input, or default to /dev/ttyACM0   
	char *device = "/dev/ttyACM0";
    printf("Connecting to %s\n", device);

    /*
     * Need the following flags to open:
     * O_RDWR: to read from/write to the devices
     * O_NOCTTY: Do not become the process's controlling terminal
     * O_NDELAY: Open the resource in nonblocking mode
     */
    fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        perror("Error opening serial");
        return -1;
    }

    /* Configure settings on the serial port */
    if (init_tty(fd) == -1) {
        perror("init");
        close(fd);
        return -1;
    }

    /* Wait for Arduino to reboot */
    usleep(1000 * 1000);

    /* Flush whatever is remaining in the buffer */
    tcflush(fd, TCIFLUSH);

    flush();

    return 0;
}

void arduinoClose()
{
    tcflush(fd, TCIFLUSH);

    close(fd);
}

void readResponse(char* txt, int len, char* buff, int buffLen, char identifier)
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

void arduinoRate()
{
    int i = 0;

    sendBytes('c');

    // Read the bpm
    while (buf[i] != 'B')
        i++;
    bpm = (int) buf[i + 2];

    // Read the hour of the BPM recorded
    while (buf[i] != 'H')
        i++;
    hour = (int) buf[i + 2] - 32;

    // Read the minute of the BPM recorded
    while (buf[i] != 'M')
        i++;
    min = (int) buf[i + 2] - 32;
    
    // Read the second of the BPM recorded
    while (buf[i] != 'S')
        i++;
    sec = (int) buf[i + 2] - 32;

    fflush(stdin);
    fflush(stdout);
    //printf("%s", buf);
    printf("\n%.3d BPM at %.2d:%.2d:%.2d", bpm, hour, min, sec);
    printf("\n\n");
}

void arduinoEnv()
{
    sendBytes('e');

    printf("\n%.25s", buf);
    printf("\n\n");
}

void clearFile()
{
    mmap_write(NULL, NULL, NULL, NULL);
}

void sysExit()
{
    arduinoClose();

    exit(0);
}

void getHist()
{
    scanf("%s", r_time);

    mmap_read();
}

void inputCmd()
{
    char input[30];


    while(1)    //infinitely looping while
    {
        printf("processor/shell$ ");  //ask user to enter command
        scanf("%s", input); 

        if (strcmp(input, "show") == 0) //if input is show X then start show method
            show();
        else if (strcmp(input, "pause") == 0) //if input is pause then start pause method
            pauseProg();
        else if (strcmp(input, "resume") == 0)   //if input is resume then start resume method
            resume();
        else if (strcmp(input, "rate") == 0)
            arduinoRate();
        else if (strcmp(input, "env") == 0)
            arduinoEnv();
        else if (strcmp(input, "hist") == 0)
            getHist();
        else if (strcmp(input, "reset") == 0)
            clearFile();
        else if(strcmp(input, "q") == 0 || strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0 || strcmp(input, "x") == 0) //if exit or quit then exit program
            sysExit();
        else if (strcmp(input, "c") == 0 || strcmp(input, "connect") == 0)
            arduinoConnect();
        else if (strcmp(input, "close") == 0)
            arduinoClose();
        else if (strcmp(input, "flush") == 0)
            flush();
        //else
        //    process_rate();
    }
   // printf("You exited the program\n");
   
}

int main()
{
    arduinoConnect();
    pid_t pid = fork();

    if (pid == 0)
        inputCmd();
    else
    {
       usleep(1000 * 1000 * 10);
       process_rate();
    }

	return 0;
}
