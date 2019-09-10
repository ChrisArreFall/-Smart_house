/**
 * Library with basic operations for handle raspberry pi 2 gpio pins.
 * Source file
 * 
 * Instituto Tecnológico de Costa Rica
 * Computer Engineering
 * CE5303: Embedded Systems Introduction
 * 
 * Author: Jeremy Rodriguez Solorzano
 * Date: 08/09/2019
 */

#include "gpiorasp2.h"

/**
 * Export the GPIO pin number to be used
 */
int GPIOExport(int pin) {
    #define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export file\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

/**
 * Unexport the GPIO pin number
 */
int GPIOUnexport(int pin) {
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport file\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

/** 
 * Set the pin mode (IN/OUT)
 */
int pinMode(int pin, int mode) {
	static const char s_directions_str[]  = "in\0out";

    #define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction file\n");
		return(-1);
	}

	if (-1 == write(fd, &s_directions_str[IN == mode ? 0 : 3], IN == mode ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction\n");
		return(-1);
	}

	close(fd);
	return(0);
}

/**
 * Read the pin value
 */
int digitalRead(int pin) {
    #define VALUE_MAX 30
	char path[VALUE_MAX];
	char value_str[3];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for reading\n");
		return(-1);
	}

	if (-1 == read(fd, value_str, 3)) {
		fprintf(stderr, "Failed to read value\n");
		return(-1);
	}

	close(fd);

	return(atoi(value_str));
}

/**
 * Write a value in the given pin
 */
int digitalWrite(int pin, int value) {
	static const char s_values_str[] = "01";

	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing\n");
		return(-1);
	}

	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value\n");
		return(-1);
	}

	close(fd);
	return(0);
}

/**
 * Generate a blink in a given pin, with a given duration and frequency
 * duration: number of blinks
 * freq: frequency in seconds
 */
int blink(int pin, int freq, int duration) {
    int repeat = duration;
    do {
		/*
		 * Write GPIO value
		 */
		if (-1 == digitalWrite(pin, repeat % 2))
			return(3);

		sleep(freq);
	}
	while (repeat--);

    return(0);
}