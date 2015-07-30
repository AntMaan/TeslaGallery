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
#include "limits.h"

version_t TESLA_VERSION;

void main (void){

	// Set the clock to 40MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |SYSCTL_OSC_MAIN);

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
	LimitsInit();

	DelayMs(2000);

	SubsystemInit(TESLA, MESSAGE, "TESLA", TESLA_VERSION);

	LogMsg(TESLA, MESSAGE, "Tesla System Initialized!");

	RegisterReceiverUART1(ProcessMidi);

	TaskScheduleAdd(MidiChannelUpdate, TASK_LOW_PRIORITY, 100, 1000);

	while(1){
		SystemTick();
		// We want to do this as often as possible without doing it every step
		LimitsCheck();
	}
}
