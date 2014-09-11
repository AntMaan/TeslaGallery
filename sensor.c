/*
 * sensor.c
 *
 *  Created on: Aug 10, 2014
 *      Author: Anthony
 */

#include <stdint.h>
#include <stdbool.h>

#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include <driverlib/gpio.h>
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"

#include "subsys.h"

#include "sensor.h"

version_t SENSOR_VERSION;

#define TRIGGER_TIMEOUT 1000

#define SENSOR_LOW_THRESHOLD 600
#define SENSOR_HIGH_THRESHOLD 800

tint_t lastSensor1Trigger;
tint_t lastSensor2Trigger;

void Sensor1OutOn(void);
void Sensor1OutOff(void);
void Sensor2OutOn(void);
void Sensor2OutOff(void);

void SensorInit(void){

	SENSOR_VERSION.word = 0x14081000LU;
	SubsystemInit(SENSOR, MESSAGE, "SENSOR", SENSOR_VERSION);

	//Enable register access to ADC0
	SysCtlPeripheralEnable( SYSCTL_PERIPH_ADC0 );

	//Enable register access to GPIO Port B
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	// Set the sensor outputs as outputs
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_7);

	// Select the analog ADC function for these pins.
	GPIOPinTypeADC( GPIO_PORTB_BASE, GPIO_PIN_4);	//Sensor 1 In
	GPIOPinTypeADC( GPIO_PORTB_BASE, GPIO_PIN_6);	//Sensor 2 In

	//Set up the ADC sequencer
	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_ALWAYS, 3);
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH10|ADC_CTL_CMP0);
	ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH10|ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 0);

	//Set up the comparator
	ADCComparatorConfigure(ADC0_BASE, 0, ADC_COMP_TRIG_NONE|ADC_COMP_INT_LOW_HONCE );
	ADCComparatorRegionSet(ADC0_BASE, 0, SENSOR_LOW_THRESHOLD, SENSOR_HIGH_THRESHOLD);
	ADCComparatorReset(ADC0_BASE, 0, true, true);
	ADCComparatorIntEnable(ADC0_BASE, 0);

	ADCIntRegister(ADC0_BASE, 0, Sensor1ISR);
	ADCIntEnable(ADC0_BASE, 0);

}

void Sensor1ISR(void){
	if(TimeSince(lastSensor1Trigger) > TRIGGER_TIMEOUT){
		lastSensor1Trigger = TimeNow();
		Sensor1OutOn();
		LogMsg(SENSOR, MESSAGE, "Sensor 1 Triggered");
		TaskScheduleAdd(Sensor1OutOff, TASK_MEDIUM_PRIORITY, 10, 0);
	}
	ADCComparatorIntClear(ADC0_BASE, 0x0F);
}

void Sensor2ISR(void){
	if(TimeSince(lastSensor2Trigger) > TRIGGER_TIMEOUT){
		lastSensor2Trigger = TimeNow();
		Sensor2OutOn();
		TaskScheduleAdd(Sensor2OutOff, TASK_MEDIUM_PRIORITY, 10, 0);
	}

	ADCComparatorIntClear(ADC0_BASE, 0x0F);
}

void Sensor1OutOn(void) {GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_PIN_5);}
void Sensor1OutOff(void) {GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0);}

void Sensor2OutOn(void) {GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, GPIO_PIN_7);}
void Sensor2OutOff(void) {GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0);}

