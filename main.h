#ifndef MAIN__H
#define MAIN__H
/**
* 
*/

#define CALIBRATED_ONE_HOUR 450
#define OPENING_TIME_MS 10000

void leftButtonPressed(void);
void leftButtonReleased(void);
void middleButtonPressed(void);
void middleButtonReleased(void);
void rightButtonPressed(void);
void rightButtonReleased(void);
void openCage(void);
void uiTriggered(void);
void watchDogInterruptCallback(void);
#endif