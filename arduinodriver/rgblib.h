/*
 * @name RGB_Drivers.ino 
 * @vers 1.01
 * @auth Theodore Miller, Danny Nsouli
 * @desc This ino drivers the RGB 8x8 Matrix display.
 *       Methods can be invoked to display certain numbers on the screen,
 *       given an x and y coordinate. 
 * 
 * 
 ******************/

/**************************/
/* RGB LED DISPLAY COLORS */
/**************************/

#ifndef _RGBLIB_H
#define _RGBLIB_H

#include <Arduino.h>
#include <Wire.h>

#define BLACK 0
#define BLUE 2
#define ORANGE 100
#define RED 32
#define GREEN 8
#define WHITE 122
#define YELLOW 40

/**************************/
/*    RGB DISPLAY PINS    */
/**************************/

#define PIN_RGB_SCLK 3  // Pin SCK del display
#define PIN_RGB_CS 4    // Pin PIN_RGB_CS del display
#define PIN_RGB_MOSI 5  // Pin DI del display

/**************************/
/*    RGB DISPLAY VARS    */
/**************************/
const int bits[8] = {128, 64, 32, 16, 8, 4, 2, 1};
const byte clr = BLUE; // What color the display is by default
const byte txt = WHITE;

//byte nums[10][4][8] = {{4, 2, 2, 2, 2, 2, 2, 4}, {2, 2, 2, 2, 2, 2, 2, 2}, {txt, txt, txt, txt, txt, clr, clr, clr}, {2, 2, 2, 2, 2, 2, 2, 2}};
/**************************/
void initMatrix();

void matrixWrite(int num);

void drawChar(char c, int x, int y);

void placeDots(int x, int y, int num, ...);

void updateDisplay(byte frame[8][8]);

void drawFrame(byte frame[8][8]);

void writeByte(byte myByte);

#endif