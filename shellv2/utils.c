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

#include "utils.h"

char r_time[8];

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

void process_hist(char *map)
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

    process_hist(map);
    
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


// Print a star histogram using the recorded dates and times.
void print_histogram(int bpms[10], int hours[10], int mins[10], int secs[10])
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

void clear_mmap()
{
    mmap_write(NULL, NULL, NULL, NULL);
}