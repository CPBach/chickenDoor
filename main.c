#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "driver.h"
#include "main.h"

/* The watchdog interrupt increments this on every call. */
volatile uint16_t internalWatchdogCounter = 0;

/* If not set by buttons then the default cage opening will happen in 12 hours. */
volatile uint16_t startInXHours = 12;
volatile uint16_t hoursPassed = 0;
volatile bool hasBeenClosed = false;

void leftButtonPressed() {
	motorLeft(true);
}

void leftButtonReleased() {
	motorLeft(false);
}

void middleButtonPressed() {
	startInXHours++;
	statusLED(true);
}

void middleButtonReleased() {
	/* Status LED off*/
	statusLED(false);
}

void rightButtonPressed() {
	hasBeenClosed = true;
	/* Motor PIN High */
	motorRight(true);
}

void rightButtonReleased() {
	motorRight(false);
}

void openCage(void) {
	/* Set PIN as OUTPUT. */
	/* Set PIN to high.*/
	_delay_ms(OPENING_TIME_MS);
	/* Set PIN to low. */ 
}

void uiTriggered() {
	
}

void watchDogInterruptCallback() {
	if (++internalWatchdogCounter % CALIBRATED_ONE_HOUR == 0) {
		hoursPassed++;
		if (hoursPassed == startInXHours) {
			startInXHours = 24;
			hoursPassed = 0;
			internalWatchdogCounter = 0;
			cli();
			openCage();
			sei();
		}
	}
}

/**
* The main routine
*/
int main(void) {
	/* Control when to go back into deep sleep. */
	while(true) {
		// Do nothing
	}
}
