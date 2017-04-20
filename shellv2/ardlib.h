#ifndef _ARDLIB_H
#define _ARDLIB_H

void sys_exit();

void arduino_env();

void arduino_heartrate();

void arduino_close();

int arduino_connect(char **args);

void process_rate();

void print_histogram(int bpms[10], int hours[10], int mins[10], int secs[10]);

void arduino_show();

void arduino_pause();

void arduino_resume();

void arduino_clock_sync();

int send_bytes(char byte);

int init_tty(int fd);

void fork_heartrate();

#endif