/*
 * major.c
 *
 *  Created on: Aug 11, 2014
 *      Author: Anthony
 */


#include "major.h"

#include <stdint.h>
#include <stdbool.h>

#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "midi.h"

void ExternalSensorISR_PortD(void);
void ExternalSensorISR_PortE(void);
void ExternalSensorISR_PortF(void);

void MajorInit(void){

	// Allow register access to all ports used for external sensor inputs to the captain
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	// Set all external sensor inputs as inputs
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_7);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0  | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);

	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_RISING_EDGE);
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0);
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_M;

	//Setup the pins to interrupt on a rising edge only
	GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_7, GPIO_RISING_EDGE);
	GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_0  | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_RISING_EDGE);
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4, GPIO_RISING_EDGE);

	//Register the Port Interrupt Handlers
	GPIOIntRegister(GPIO_PORTD_BASE, ExternalSensorISR_PortD);
	GPIOIntRegister(GPIO_PORTE_BASE, ExternalSensorISR_PortE);
	GPIOIntRegister(GPIO_PORTF_BASE, ExternalSensorISR_PortF);

	//Enable the interrupts on each port for the external sensor pins
	GPIOIntEnable(GPIO_PORTD_BASE, GPIO_PIN_7);
	GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0  | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);
}

/*
 * This ISR services rising edges on PIN7 of PORTD
 * This is the only interrupt enabled on this port
 *
 * External Sensor In 10
 *
 */
void ExternalSensorISR_PortD(void){

	uint8_t midi_msg[3];

	GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_7);

	midi_msg[0] = NOTE_ON | CHANNEL_9;
	midi_msg[1] = C1;
	midi_msg[2] = 0x00;

	MidiSendMsg(&midi_msg[0], 3);

}

/*
 * This ISR services rising edges on Pins 0-3 of PORTE
 * This is the only interrupt enabled on this port
 *
 * External Sensor In 1-4
 *
 */
void ExternalSensorISR_PortE(void){

	uint8_t midi_msg[3];

	if(GPIOIntStatus(GPIO_PORTE_BASE, true) & GPIO_PIN_0){
		GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_0);
		midi_msg[0] = NOTE_ON | CHANNEL_0;
		midi_msg[1] = C1;
		midi_msg[2] = 0x0F;
		MidiSendMsg(&midi_msg[0], 3);
	}

	if(GPIOIntStatus(GPIO_PORTE_BASE, true) & GPIO_PIN_1){
		GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_1);
		midi_msg[0] = NOTE_ON | CHANNEL_1;
		midi_msg[1] = C1;
		midi_msg[2] = 0x0F;
		MidiSendMsg(&midi_msg[0], 3);
	}

	if(GPIOIntStatus(GPIO_PORTE_BASE, true) & GPIO_PIN_2){
		GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_2);
		midi_msg[0] = NOTE_ON | CHANNEL_2;
		midi_msg[1] = C1;
		midi_msg[2] = 0x0F;
		MidiSendMsg(&midi_msg[0], 3);
	}

	if(GPIOIntStatus(GPIO_PORTE_BASE, true) & GPIO_PIN_3){
		GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_3);
		midi_msg[0] = NOTE_ON | CHANNEL_3;
		midi_msg[1] = C1;
		midi_msg[2] = 0x0F;
		MidiSendMsg(&midi_msg[0], 3);
	}
}

/*
 * This ISR services rising edges on Pins 0-4 of PORTF
 * This is the only interrupt enabled on this port
 *
 * External Sensor In 5-9
 *
 */
void ExternalSensorISR_PortF(void){

	uint8_t midi_msg[3];

	if(GPIOIntStatus(GPIO_PORTF_BASE, true) & GPIO_PIN_0){
		GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);
		midi_msg[0] = NOTE_ON | CHANNEL_4;
		midi_msg[1] = C1;
		midi_msg[2] = 0x0F;
		MidiSendMsg(&midi_msg[0], 3);
	}

	if(GPIOIntStatus(GPIO_PORTF_BASE, true) & GPIO_PIN_1){
		GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_1);
		midi_msg[0] = NOTE_ON | CHANNEL_5;
		midi_msg[1] = C1;
		midi_msg[2] = 0x0F;
		MidiSendMsg(&midi_msg[0], 3);
	}

	if(GPIOIntStatus(GPIO_PORTF_BASE, true) & GPIO_PIN_2){
		GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_2);
		midi_msg[0] = NOTE_ON | CHANNEL_6;
		midi_msg[1] = C1;
		midi_msg[2] = 0x0F;
		MidiSendMsg(&midi_msg[0], 3);
	}

	if(GPIOIntStatus(GPIO_PORTF_BASE, true) & GPIO_PIN_3){
		GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_3);
		midi_msg[0] = NOTE_ON | CHANNEL_7;
		midi_msg[1] = C1;
		midi_msg[2] = 0x0F;
		MidiSendMsg(&midi_msg[0], 3);
	}

	if(GPIOIntStatus(GPIO_PORTF_BASE, true) & GPIO_PIN_4){
		GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);
		midi_msg[0] = NOTE_ON | CHANNEL_8;
		midi_msg[1] = C1;
		midi_msg[2] = 0x0F;
		MidiSendMsg(&midi_msg[0], 3);
	}
}



