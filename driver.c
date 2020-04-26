/**
* ATTiny13 Pins:
*                                 -----
* (PCINT5/_RESET/ADC0/dW) PB5   -|1   8|- VCC
* (PCINT3/CLKI/ADC3)      PC3   -|2   7|- PB2 (SCK/ADC1/T0/PCINT2)
* (PCINT4/ADC2)           PB4   -|3   6|- PB1 (MISO/AIN1/OC0B/INT0/PCINT1)
*                         GND   -|4   5|- PB0 (MOSI/AIN0/OC0A/PCINT0)
*                                 -----

*  USE FUSE BITS: LOW 0x2A, HIGH: 0xFF : CLK Src Internal 9.6MhZ / 8, Preserve EEPROM through CHIP erase,
*/
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "driver.h"
#include "main.h"

#define MOTOR_LEFT_PIN PB0
#define MOTOR_RIGHT_PIN PB1
#define UI_PIN PB2
#define UI_PIN_INT PCINT2
#define UI_PIN_ADC MUX0
#define STATUS_LED_PIN PB3
#define END_SWITCH_PIN PB4

void go_to_deep_sleep() {
	adcOn(false);
	MCUCR |= (1 << SE);
	MCUCR &= ~(1 << SM0);	
	MCUCR |= (1 << SM1);
	MCUCR |= (3<<5); // set both BODS and BODSE
	MCUCR = (MCUCR & ~(1 << 5)) |(1<<6); // then set  BODS bit and clear BODSE bit
	__asm__ __volatile__("sleep");
}

void wakeUpRoutine() {
	adcOn(true);
	enableINT0(false);
}

void setupDriver() {
/*	Setup input / output ports:
	-> Output: Motor left:      PB0 -> INITIAL OUTPUT -> LOW
	-> Output: Motor right:     PB1 -> INITIAL OUTPUT -> LOW
	-> Output: Status LED       PB3 -> INITIAL OUTPUT -> HIGH
	-> Input: Switch ADC input: PB2 -> NO INTERNAL PULL UP!
	-> Input: End-Schalter:     PB4 -> NO INTERNAL PULL UP! */
	/* DISABLE Pull ups */
	MCUCR |= (1 << PUD);
	DDRB  |=   (1 << MOTOR_LEFT_PIN) | (1 << MOTOR_RIGHT_PIN) | (1 << STATUS_LED_PIN);
	DDRB  &= ~((1 << UI_PIN) | (1 << END_SWITCH_PIN));
	PORTB &= ~((0 << MOTOR_LEFT_PIN) | (0<<MOTOR_RIGHT_PIN));
	PORTB |= (1 << STATUS_LED_PIN);
	sleep(5000);
	sei();
	enableINT0(true);
	setupWatchdog(9);
	go_to_deep_sleep();
	wakeUpRoutine();
	main();
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
		ADMUX  |= (1<<REFS0);
		ADMUX  |= (1<<MUX0);
		ADCSRA |= (1<<ADEN);
	} else {
		/* Turn off adc */
		ADMUX &= ~(1<<UI_PIN_ADC);
		ADCSRA &= ~(1<<ADEN);
	}
}

void statusLED(bool on) {
	PORTB &= ~(1 << STATUS_LED_PIN);
	if (on) {
		PORTB |= (1 << STATUS_LED_PIN);
	}
}

void burstLED() {
	for(int i=0; i<20; i++) {
		// LED ON
		sleep(BURST_DELAY);
		// LED OFF
		sleep(BURST_DELAY);
	}
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
	PORTB &= ~((1 << MOTOR_RIGHT_PIN) | (1 << MOTOR_LEFT_PIN));
	if (isOn) {
		PORTB |= (1 << MOTOR_LEFT_PIN);
	}
}

void motorRight(bool isOn) {
	PORTB &= ~((1 << MOTOR_RIGHT_PIN) | (1 << MOTOR_LEFT_PIN));
	if (isOn) {
		PORTB |= (1 << MOTOR_RIGHT_PIN);
	}
}


void enableINT0(bool enable) {
	if (enable) {
		MCUCR |= (1 << PCIE);
		PCMSK |= (1 << UI_PIN_INT);
	} else {
		MCUCR &= ~(1 << PCIE);
		PCMSK &= ~(1 << UI_PIN_INT);
	}
}

void sleep(int ms){
	_delay_ms(ms);
}

ISR(INT0_vect) {
	wakeUpRoutine();
	uiTriggered();
}

ISR(WDT_vect) {
	wakeUpRoutine();
	watchDogInterruptCallback();
}