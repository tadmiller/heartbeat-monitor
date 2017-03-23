#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <stdint.h>

int init_tty(int fd);

/**
 * Run this program and pass the location of the serial file descriptor as an argument
 * On linux, this is likely /dev/ttyACM0
 * On OSX, this may be similar to /dev/cu.usbmodem1D1131
 */

int
main(int argc, char **argv) {
    int fd;
    int count;
    char *device;
    char byte;
    char buf[32];

    /*
     * Read the device path from input,
     * or default to /dev/ttyACM0
     */
    if (argc == 2) {
        device = argv[1];
    } else {
        device = "/dev/ttyACM0";
    }
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
    usleep(1000*1000);

    /* Flush whatever is remaining in the buffer */
    tcflush(fd, TCIFLUSH);

    for (byte = 'a'; byte <= 'z'; byte++) {

        // Write this letter of the alphabet
        count = write(fd, &byte, 1);
        if (count == -1) {
            perror("write");
            close(fd);
            return -1;
        } else if (count == 0) {
            fprintf(stderr, "No data written\n");
            close(fd);
            return -1;
        }

        // Wait for data to transmit
        sleep(1);

        // Read the response
        count = read(fd, &buf, 32);
        if (count == -1) {
            perror("read");
            close(fd);
            return -1;
        } else if (count == 0) {
            fprintf(stderr, "No data returned\n");
            continue;
        }

        // Ensure the response is null-terminated
        buf[count] = 0;
        printf("(%d): %s", count, buf);

    }

    close(fd);
    return 0;
}

int
init_tty(int fd) {
    struct termios tty;
    /*
     * Configure the serial port.
     * First, get a reference to options for the tty
     * Then, set the baud rate to 9600 (same as on Arduino)
     */
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(fd, &tty) == -1) {
        perror("tcgetattr");
        return -1;
    }

    if (cfsetospeed(&tty, (speed_t)B9600) == -1) {
        perror("ctsetospeed");
        return -1;
    }
    if (cfsetispeed(&tty, (speed_t)B9600) == -1) {
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
    if (tcsetattr(fd, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}
