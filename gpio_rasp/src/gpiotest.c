/**
 * Application to test gpiorasp2 lib.
 * 
 * Instituto Tecnológico de Costa Rica
 * Computer Engineering
 * CE5303: Embedded Systems Introduction
 * 
 * Author: Jeremy Rodriguez Solorzano
 * Date: 08/09/2019
 */

#include "gpiorasp2.h"

#define PIN 27
#define POUT1 25
#define POUT2 26

#define FREQ 1
#define DUR 5

int main() {
	// Enable GPIO pins
	if (-1 == GPIOExport(POUT1) || -1 == GPIOExport(POUT2) || -1 == GPIOExport(PIN))
	  return(1);

	// Set GPIO directions
	if (-1 == pinMode(POUT1, OUT) || -1 == pinMode(POUT2, OUT) || -1 == pinMode(PIN, IN))
	  return(2);

  // Push Button

  while (1) {
    int value_read = digitalRead(PIN);
    if (value_read != 0) {
      if (digitalRead(POUT2) == 0) {
        digitalWrite(POUT1, 1);
        digitalWrite(POUT2, 1);
      } else {
        digitalWrite(POUT1, 0);
        digitalWrite(POUT2, 0);
      }
    }

    sleep(0.75);
  }

  //   // Blink (GPIO26)
  //   printf("Blinking...\n");
  //   blink(POUT2, FREQ, DUR);

  //   // Write 0 (GPIO25)
  //   printf("Writing 1...\n");
  //   digitalWrite(POUT1, 1);
  //   sleep(3);

  //   // Write 1 (GPIO25)
  //   printf("Writing 0...\n");
  //   digitalWrite(POUT1, 0);
  //   sleep(3);

  //   // Read (GPIO27)
  //   printf("Reading...\n");
  //   int value_read = digitalRead(PIN);
  //   printf("Value = %d\n", value_read);

  /*
	 Disable GPIO pins
	*/
	if (-1 == GPIOUnexport(POUT1) || -1 == GPIOUnexport(POUT2) || -1 == GPIOUnexport(PIN))
	  return(4);

	return(0);
}
