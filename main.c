#include <stdbool.h>
#include <stdint.h>

#include "timing.h"
#include "task.h"
#include "uart.h"
#include "buffer.h"
#include "subsys.h"
#include "system.h"

#include "sensor.h"
#include "midi.h"
#include "motor.h"
#include "major.h"

version_t TESLA_VERSION;

void MotorStepTest(void);

void main (void){

	SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_INT | SYSCTL_OSC_MAIN);

	TESLA_VERSION.word = 0x14081000LU;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	TimerInit();
	TaskInit();
	SystemInit();
	MotorsInit();
	MidiInit();
	SensorInit();
	MajorInit();

	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_7);

	SubsystemInit(TESLA, MESSAGE, "TESLA", TESLA_VERSION);

	LogMsg(TESLA, MESSAGE, "Tesla System Initialized!");

	RegisterReceiverUART1(ProcessMidi);

	TaskScheduleAdd(MotorsUpdate, TASK_MEDIUM_PRIORITY, 100, 10);
	TaskScheduleAdd(MidiChannelUpdate, TASK_LOW_PRIORITY, 100, 1000);

	MotorStepTest();

	while(1){
		SystemTick();
	}
}

void MotorStepTest(void){

	motors[MOTOR1].period = 10000;
	motors[MOTOR1].direction = UP;

	WaitMs(250);
	motors[MOTOR1].period = 0;

	WaitMs(1000);

	LogMsg(TESLA, MESSAGE, "Motor 1 Steps %d", motors[MOTOR1].steps);

	motors[MOTOR1].period = 10000;
	motors[MOTOR1].direction = DOWN;

	WaitMs(150);
	motors[MOTOR1].period = 0;

	WaitMs(1000);

	LogMsg(TESLA, MESSAGE, "Motor 1 Steps %d", motors[MOTOR1].steps);

}

