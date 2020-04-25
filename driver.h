/**
* ATTiny13 Pins:
*                                 -----
* (PCINT5/_RESET/ADC0/dW) PB5   -|1   8|- VCC
* (PCINT3/CLKI/ADC3)      PC3   -|2   7|- PB2 (SCK/ADC1/T0/PCINT2)
* (PCINT4/ADC2)           PB4   -|3   6|- PB1 (MISO/AIN1/OC0B/INT0/PCINT1)
*                         GND   -|4   5|- PB0 (MOSI/AIN0/OC0A/PCINT0)
*                                 -----
*/
#ifndef DRIVER__H
#define DRIVER__H
#include <stdint.h>
#include <stdbool.h>
#define BURST_DELAY 50

void go_to_deep_sleep(void);

void wakeUpRoutine(void);
void motorLeft(bool);
void motorRight(bool);

void setupWatchdog(int);


void adcOn(bool);

void statusLED(bool);

void burstLED(void);

void sleep(int);

void printSingleDigit(int);

void printHexNumber(uint16_t);

void enableINT0(bool);

#endif