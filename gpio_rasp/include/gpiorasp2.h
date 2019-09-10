/**
 * Library with basic operations for handle raspberry pi 2 gpio pins.
 * Header file
 * 
 * Instituto Tecnológico de Costa Rica
 * Computer Engineering
 * CE5303: Embedded Systems Introduction
 * 
 * Author: Jeremy Rodriguez Solorzano
 * Date: 08/09/2019
 */

#ifndef _GPIORASP2_H
#define _GPIORASP2_H

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define IN 0
#define OUT 1

#define LOW 0

/**
 * Export the GPIO pin number to be used
 */
int GPIOExport(int);

/**
 * Unexport the GPIO pin number
 */
int GPIOUnexport(int );

/** 
 * Set the pin mode (IN/OUT)
 */
int pinMode(int, int );

/**
 * Read the pin value
 */
int digitalRead(int);

/**
 * Write a value in the given pin
 */
int digitalWrite(int, int);

/**
 * Generate a blink in a given pin, with a given duration and frequency
 * duration: number of blinks
 * freq: frequency in seconds
 */
int blink(int, int, int);

#endif