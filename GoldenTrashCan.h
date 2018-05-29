// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _GoldenTrashCan_H_
#define _GoldenTrashCan_H_
#include "Arduino.h"
#include "Motor.h"
#include "LedWrapper.h"

//#include "TimerOne.h"
#include "Bounce2.h"
#include "FlexiTimer2.h"
#include "Fastled.h"

#define MOTORA 		9
#define MOTORB 		10
#define SW1			7
#define SW2			11
#define MOTOR_SPEED	A0
#define MOTOR_CTRL	A1

enum LedMode {None=0, Chase=1, Fade=2, Random=3};



Motor motor;

uint8_t isCommandAvailable();
void setCommandAvailable(boolean flag);
uint8_t commandDelay(uint32_t time);



//Do not add code below this line
#endif /* _GoldenTrashCan_H_ */
