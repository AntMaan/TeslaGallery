/*
 * midi.c
 *
 *  Created on: Mar 26, 2014
 *      Author: Anthony
 */

#include "midi.h"
#include "motor.h"
#include "subsys.h"

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "int_def.h"

#define slope -59.5238
#define y_int 9000

static inline calc_period(char velocity) {return (uint32_t)velocity*slope + y_int;}

typedef struct {
	char note;
	char velocity;
} noteEvent_t;

enum {
	IDLE = 0,
	CONTROL_BYTE_RECEIVED,
	NOTE_RECEIVED
} midi_msg_states;

version_t MIDI_VERSION;

union8_t channel;

void MidiInit(void){
	MIDI_VERSION.word = 0x14081000LU;
	SubsystemInit(MIDI, MESSAGE, "MIDI", MIDI_VERSION);

	//Allow register access to port's
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_4);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
}

void MidiChannelUpdate(void){
	channel.bits.b0 = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2) >> 2;
	channel.bits.b1 = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_3) >> 3;
	channel.bits.b2 = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) >> 4;
	channel.bits.b3 = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_5) >> 5;
}


void MidiSendMsg(uint8_t *buf, uint8_t length){
	uint8_t i;
	for(i = 0; i < length; i++){
		Push(&tx1, buf[i]);
	}
}

void ProcessMidi(char midi_char) {
	static noteEvent_t noteEvent;
	static uint8_t messageType;

//	LogMsg(MIDI, MESSAGE, "Character Received: 0x%x", midi_char);

	switch (midi_msg_states){
		case IDLE:
			// Check to make sure we just received a control byte for this device's channel
			if( (midi_char & 0x80) && ((midi_char & 0x0F) == channel.b) ){
				messageType = midi_char & 0xF0;
				midi_msg_states = CONTROL_BYTE_RECEIVED;
			}
			break;
		case CONTROL_BYTE_RECEIVED:
			noteEvent.note = midi_char;
			midi_msg_states = NOTE_RECEIVED;
			break;
		case NOTE_RECEIVED:
			noteEvent.velocity = midi_char;
			midi_msg_states = IDLE;
			switch(messageType){
				case NOTE_ON:
					switch(noteEvent.note){
						case C3:
							if(noteEvent.velocity == 0){
								motors[MOTOR1].period = noteEvent.velocity;
							}
							else{
								motors[MOTOR1].period = calc_period(noteEvent.velocity);
							}
							motors[MOTOR1].direction = UP;
							break;
						case B2:
							if(noteEvent.velocity == 0){
								motors[MOTOR1].period = noteEvent.velocity;
							}
							else{
								motors[MOTOR1].period = calc_period(noteEvent.velocity);
							}
							motors[MOTOR1].direction = DOWN;
							break;
						case F3:
							if(noteEvent.velocity == 0){
								motors[MOTOR2].period = noteEvent.velocity;
							}
							else{
								motors[MOTOR2].period = calc_period(noteEvent.velocity);
							}
							motors[MOTOR2].direction = UP;
							break;
						case E3:
							if(noteEvent.velocity == 0){
								motors[MOTOR2].period = noteEvent.velocity;
							}
							else{
								motors[MOTOR2].period = calc_period(noteEvent.velocity);
							}
							motors[MOTOR2].direction = DOWN;
							break;
						case C4:
							if(noteEvent.velocity == 0){
								motors[MOTOR3].period = noteEvent.velocity;
							}
							else{
								motors[MOTOR3].period = calc_period(noteEvent.velocity);
							}
							motors[MOTOR3].direction = UP;
							break;
						case B3:
							if(noteEvent.velocity == 0){
								motors[MOTOR3].period = noteEvent.velocity;
							}
							else{
								motors[MOTOR3].period = calc_period(noteEvent.velocity);
							}
							motors[MOTOR3].direction = DOWN;
							break;
						case C5:
							MotorsKillAll();
							break;
						case D5:

							break;

						case E5:

							break;
						default:
							midi_msg_states = IDLE;
							break;
					}
					break;
			case NOTE_OFF:
				switch(noteEvent.note){
					case C3:
						motors[MOTOR1].period = 0;
						motors[MOTOR1].direction = UP;
						break;
					case B2:
						motors[MOTOR1].period = 0;
						motors[MOTOR1].direction = DOWN;
						break;
					case F3:
						motors[MOTOR2].period = 0;
						motors[MOTOR2].direction = UP;
						break;
					case E3:
						motors[MOTOR2].period = 0;
						motors[MOTOR2].direction = DOWN;
						break;
					case C4:
						motors[MOTOR3].period = 0;
						motors[MOTOR3].direction = UP;
						break;
					case B3:
						motors[MOTOR3].period = 0;
						motors[MOTOR3].direction = DOWN;
						break;
					default:
						midi_msg_states = IDLE;
						break;
				}
				break;
			}
	}
}

