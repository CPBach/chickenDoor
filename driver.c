/**
* ATTiny13 Pins:
*                                 -----
* (PCINT5/_RESET/ADC0/dW) PB5   -|1   8|- VCC
* (PCINT3/CLKI/ADC3)      PC3   -|2   7|- PB2 (SCK/ADC1/T0/PCINT2)
* (PCINT4/ADC2)           PB4   -|3   6|- PB1 (MISO/AIN1/OC0B/INT0/PCINT1)
*                         GND   -|4   5|- PB0 (MOSI/AIN0/OC0A/PCINT0)
*                                 -----
*/
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "driver.h"
#include "main.h"

void go_to_deep_sleep() {
	adcOn(false);
	MCUCR = 0; // TODO: set bit 5 to 1, 	Set Bit 4 to 1, Bit 3 to 0 -> SM1=1;SM0=0 and also Sleep enable
	MCUCR |= (3<<5); // set both BODS and BODSE
	MCUCR = (MCUCR & ~(1 << 5)) |(1<<6); // then set  BODS bit and clear BODSE bit
	__asm__ __volatile__("sleep");
}

void wakeUpRoutine() {
	adcOn(true);
}

// Set it to 9 for 8s interval.
void setupWatchdog(int ii) {
	unsigned char bb;
	int ww;
	
	if (ii>9) ii = 9;
	bb = ii & 7;
	if (ii>7) bb|= (1<<5);
	bb|= (1<<WDCE);
	ww=bb;
	
	MCUCR &= (1<<WDRF);
	WDTCR |= (1<<WDCE) | (1<<WDE);
	WDTCR = bb;
	WDTCR |= _BV(WDTIE);
}

void adcOn(bool adcOn) {
	if(adcOn) {
		/* Turn on adc  */
	} else {
		/* Turn off adc */
		ADCSRA &= ~(1<<ADEN);
	}
}

void statusLED(bool on) {
	
}

void burstLED() {
	for(int i=0; i<20; i++) {
		// LED ON
		_delay_ms(BURST_DELAY);
		// LED OFF
		_delay_ms(BURST_DELAY);
	}
}

void sleep(int ms){
	_delay_ms(ms);
}

void printSingleDigit(int number) {
	burstLED();
	sleep(1000);
	for (int i=0; i<number; i++) {
		statusLED(true);
		sleep(500);
		statusLED(false);
		sleep(500);
	}
	sleep(1000);
}

void printHexNumber(uint16_t number) {
	int firstHex = number >> 12;
	int secondHex = (number & 0x0fff) >> 8;
	int thirdHex = (number & 0x00f0) >> 4;
	int fourthHex = number & 0x000f;
	printSingleDigit(firstHex);
	printSingleDigit(secondHex);
	printSingleDigit(thirdHex);
	printSingleDigit(fourthHex);
}

void motorLeft(bool isOn) {
	
}

void motorRight(bool isOn) {
	
}

ISR(WDT_vect) {
	wakeUpRoutine();
	watchDogInterruptCallback();
}

void enableINT0(bool enable) {
	if (enable) {
		// enable interrupt 0
	} else {
		// disable interrupt 0
	}
}

ISR(INT0_vect) {
	wakeUpRoutine();
	// Turn off INT0
	enableINT0(false);
	uiTriggered();
}