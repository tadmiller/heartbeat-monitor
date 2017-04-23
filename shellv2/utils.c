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
#include <math.h>
#include "utils.h"

#define GROUP_SIZE 96

// Open and append 10 BPMs to a file
int mmap_write(char *data, char *file, char mode)
{
	// Our file set
	const char *filepath = file != NULL ? file : "/tmp/histogram.csv";
	const int fd = open(filepath, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
	size_t textsize;
	
	if (fd == -1)
	{
		perror("Error opening file for writing");
		return 1;
	}

	// Stretch the file size to the size of the (mmapped) array of char

	textsize = data != NULL ? strlen(data) : 0; // + \0 null character
	
	if (lseek(fd, textsize - 1, SEEK_SET) == -1)
	{
		close(fd);
		perror("Error calling lseek() to 'stretch' the file");
		return 1;
	}
	
	if (write(fd, "", 1) == -1)
	{
		close(fd);
		perror("Error writing last byte of the file");
		return 1;
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
	
	//int count = 0;

	if (data == NULL)
	{
		close(fd);
		fclose(fopen(filepath, "w"));
		return 0;
	}
	// else
	// 	for (size_t i = 0; i < 10; i++)
	// 	{
	// 		//printf("%.3d,%.2d:%.2d:%.2d,", bpms[i], hours[i], mins[i], secs[i]);
	// 		sprintf(snum, "%.3d,%.2d:%.2d:%.2d,", bpms[i], hours[i], mins[i], secs[i]);

	// 		if (i < 9)
	// 			snum[13] = '\n';

	// 		//printf("\n%s", snum);

	// 		for (size_t j = count; j < count + 14; j++)
	// 			map[j] = snum[j % 14];

	// 		count += 14;
	// 	}

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
	char r_time[8];
	char bpm[4];
	char time[11];
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

char *mmap_read(char *file)
{    
	const char *filepath = file != NULL ? file : "/tmp/histogram.csv";
	const int fd = open(filepath, O_RDONLY, (mode_t) 0600);

	struct stat fileInfo = {0};
	char *map;

	if (fd == -1)
	{
		perror("Error opening file for writing");
		return NULL;
	}
	
	if (fstat(fd, &fileInfo) == -1)
	{
		perror("Error getting the file size");
		return NULL;
	}
	
	if (fileInfo.st_size == 0)
	{
		fprintf(stderr, "Error: File is empty, nothing to do\n");
		return NULL;
	}  //printf("File size is %ji\n", (intmax_t)fileInfo.st_size);

	map = mmap(0, fileInfo.st_size, PROT_READ, MAP_SHARED, fd, 0);

	if (map == MAP_FAILED)
	{
		close(fd);
		perror("Error mmapping the file");
		return NULL;
	}

	// Un-mmaping doesn't close the file, so we still need to do that.
	close(fd);

	return map;
}

void print_stars(int len)
{
	for (size_t i = 0; i < len; i++)
		printf("*");
}

int get_mean(int *group)
{
	int sum = 0;

	for (size_t i = 1; i < group[0]; i++)
		sum += group[i];

	return sum / (group[0] - 1);
}

int get_std_dev(int *group)
{
	return 0;
}

// Print a star histogram using the recorded dates and times.
void print_hist(int **groups, int len)
{
	for (size_t i = 0; i < len; i++)
	{
		if (groups[i])
		{
			printf("\n%.2d:%.2d | μ %.2d |\t", inverse_bucket_h(i), inverse_bucket_m(i), get_mean(groups[i]));
			print_stars(groups[i][1]);
			printf("\n      | σ %.2d |\t", 0);
			print_stars(groups[i][1]);
			printf("\n");
		}
	}

	printf("\n");
}

// str = map, len = strlen(map)
// num = bpms/hours/mins/secs
// i = itr for num
// j = itr for num - ptr so we can pass it around multiple times
int parse_commas(char *str, int len, int *numArr, size_t numItr, size_t *j)
{
	size_t start = *j;
	int numSize;
	char *num;

	while (*j < len && str[*j] != ',')
	{	// Apparently CR is two characters
		if (str[*j] == 13)
			start += 2;

		*j += 1;
	}
	if (*j >= len)
		return 1;

	numSize = *j - start;
	num = malloc(sizeof(char) * (numSize + 1));

	for (size_t i = 0; i < numSize; i++)
		num[i] = str[start + i];

	num[numSize + 1] = '\0';
	numArr[numItr] = atoi(num);

	*j += 1;
	free(num);

	return 0;
}

void parse_hist_csv(char *map, int len, int *bpms, int *hours, int *mins, int *secs, int arrLen)
{
	size_t j = 0;

	for (size_t i = 0; i < arrLen; i++)
	{
		parse_commas(map, len, bpms, i, &j);
		parse_commas(map, len, hours, i, &j);
		parse_commas(map, len, mins, i, &j);
		parse_commas(map, len, secs, i, &j);
	}
}

int inverse_bucket_h(int bucket)
{
	return bucket / 4;
}

int inverse_bucket_m(int bucket)
{
	return (bucket % 4) * 15;
}

int get_bucket(int hour, int min)
{
	if (hour > 23)
		hour = 23;
	if (min > 59)
		min = 59;

	return (hour * 4) + (min / 15);
}

void process_groups(int *bpms, int *hours, int *mins, int *secs, int arrLen, int **groups)
{
	int bucket = 0;

	for (size_t i = 0; i < arrLen; i++)
	{
		bucket = get_bucket(hours[i], mins[i]);

		if (groups[bucket] == NULL)
		{	// arrLen is max elems
			groups[bucket] = malloc(sizeof(int) * arrLen + 1);
			**(groups + bucket) = 0;
		}

		groups[bucket][0] += 1;
		groups[bucket][groups[bucket][0]] = bpms[i];
	}
}

void process_hist_v2(char **args)
{
	char *map = mmap_read(*(args + 1));
	int len;

	int *bpms;
	int *hours;
	int *mins;
	int *secs;
	int arrLen;

	// We'll use size 96 as a static size. This is because we have 4 15 minute intervals, times 24 hours. 4 * 24 = 96.
	int **groups;

	if (map == NULL)
		return;

	len = strlen(map);
	if (len < 13)
		return;

	arrLen = (len / 15) + 1;
	bpms = malloc(sizeof(int) * arrLen);
	hours = malloc(sizeof(int) * arrLen);
	mins = malloc(sizeof(int) * arrLen);
	secs = malloc(sizeof(int) * arrLen);

	groups = malloc(sizeof(int *) * GROUP_SIZE);
	for (size_t i = 0; i < GROUP_SIZE; i++)
		groups[i] = NULL;

	parse_hist_csv(map, len, bpms, hours, mins, secs, arrLen);
	process_groups(bpms, hours, mins, secs, arrLen, groups);
	print_hist(groups, GROUP_SIZE);

	munmap(map, strlen(map));
	free(bpms);
	free(hours);
	free(mins);
	free(secs);

	for (size_t i = 0; i < 96; i++)
		if (*(groups + i))
			free(groups[i]);

	free(groups);
}

void clear_mmap()
{
	//mmap_write(NULL, NULL, NULL, NULL);
}