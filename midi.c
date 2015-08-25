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

#define slope -70
//#define slope -100
//#define y_int 12000
#define y_int 9250

static inline calc_period(char velocity) {return (uint32_t)(velocity*slope + y_int);}

typedef struct {
	char note;
	char velocity;
} noteEvent_t;

enum {
	IDLE = 0,
	WAITING_FOR_NOTE,
	WAITING_FOR_VELOCITY
} midi_msg_state;

version_t MIDI_VERSION;

union8_t channel;

void MidiInit(void){
	MIDI_VERSION.word = 0x14081000LU;
	SubsystemInit(MIDI, MESSAGE, "MIDI", MIDI_VERSION);

	//Allow register access to ports
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3);
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

	if(midi_char == 0xFE) return;

	// If the MSB is a 1, this is the start of a MIDI message and it's a control byte
	if(midi_char & 0x80){
		// If the message is for this channel
		if((midi_char & 0x0F) == channel.b){
			messageType = midi_char & 0xF0;
			midi_msg_state = WAITING_FOR_NOTE;
		} else {
			midi_msg_state = IDLE;
		}
		return;
	}

	switch (midi_msg_state){
		case WAITING_FOR_NOTE:
			noteEvent.note = midi_char;
			midi_msg_state = WAITING_FOR_VELOCITY;
			break;
		case WAITING_FOR_VELOCITY:
			noteEvent.velocity = midi_char;
			// Set the state back to WAITING_FOR_NOTE in case the message has more than one note control
			// If this was the end of the message, the info will automatically be fixed if a message with
			// MSB 1 is received
			midi_msg_state = WAITING_FOR_NOTE;
			switch(messageType){
				case NOTE_ON:
					LogMsg(MIDI, MESSAGE, "Note On Received. Note: %d, Velocity %d", noteEvent.note, noteEvent.velocity);
					switch(noteEvent.note){
						case C3:
							if(motorState == NORMAL){
								if(noteEvent.velocity == 0){
									motors[MOTOR1].timerVal = 65536;
								}
								else{
									motors[MOTOR1].timerVal = calc_period(noteEvent.velocity);
								}
								motors[MOTOR1].direction = UP;
								GPIOPinWrite(MOTOR1_DIR_PORT, MOTOR1_DIR_PIN, motors[MOTOR1].direction);
								MotorStart(MOTOR1);
							}
							break;
						case B2:
							if(motorState == NORMAL){
								if(noteEvent.velocity == 0){
									motors[MOTOR1].timerVal = 65536;
								}
								else{
									motors[MOTOR1].timerVal = calc_period(noteEvent.velocity);
								}
								motors[MOTOR1].direction = DOWN;
								GPIOPinWrite(MOTOR1_DIR_PORT, MOTOR1_DIR_PIN, motors[MOTOR1].direction);
								MotorStart(MOTOR1);
							}
							break;
						case F3:
							if(motorState == NORMAL){
								if(noteEvent.velocity == 0){
									motors[MOTOR2].timerVal = 65536;
								}
								else{
									motors[MOTOR2].timerVal = calc_period(noteEvent.velocity);
								}
								motors[MOTOR2].direction = UP;
								GPIOPinWrite(MOTOR2_DIR_PORT, MOTOR2_DIR_PIN, motors[MOTOR2].direction);
								MotorStart(MOTOR2);
							}
							break;
						case E3:
							if(motorState == NORMAL){
								if(noteEvent.velocity == 0){
									motors[MOTOR2].timerVal = 65536;
								}
								else{
									motors[MOTOR2].timerVal = calc_period(noteEvent.velocity);
								}
								motors[MOTOR2].direction = DOWN;
								GPIOPinWrite(MOTOR2_DIR_PORT, MOTOR2_DIR_PIN, motors[MOTOR2].direction);
								MotorStart(MOTOR2);
							}
							break;
						case C4:
							if(motorState == NORMAL){
								if(noteEvent.velocity == 0){
									motors[MOTOR3].timerVal = 65536;
								}
								else{
									motors[MOTOR3].timerVal = calc_period(noteEvent.velocity);
								}
								motors[MOTOR3].direction = UP;
								GPIOPinWrite(MOTOR3_DIR_PORT, MOTOR3_DIR_PIN, motors[MOTOR3].direction);
								MotorStart(MOTOR3);
							}
							break;
						case B3:
							if(motorState == NORMAL){
								if(noteEvent.velocity == 0){
									motors[MOTOR3].timerVal = 65536;
								}
								else{
									motors[MOTOR3].timerVal = calc_period(noteEvent.velocity);
								}
								motors[MOTOR3].direction = DOWN;
								GPIOPinWrite(MOTOR3_DIR_PORT, MOTOR3_DIR_PIN, motors[MOTOR3].direction);
								MotorStart(MOTOR3);
							}
							break;
						case C5:
							MotorsDisable();
							break;
						case D5:
							if(motorState == DISABLED){
								MotorsEnable();
							}
							break;
						case E5:
							if(motorState == NORMAL){
								MotorsCalibrate();
							}
							break;
						case F5:
							if(motorState == NORMAL){
								MotorsReset();
							}
						default:
							break;
					}
					break;
			case NOTE_OFF:
				LogMsg(MIDI, MESSAGE, "Note Off Received. Note: %d, Velocity %d", noteEvent.note, noteEvent.velocity);
				if(motorState == NORMAL){
					switch(noteEvent.note){
						case C3:
							motors[MOTOR1].timerVal = 65536;
							motors[MOTOR1].direction = UP;
							GPIOPinWrite(MOTOR1_DIR_PORT, MOTOR1_DIR_PIN, motors[MOTOR1].direction);
							break;
						case B2:
							motors[MOTOR1].timerVal = 65536;
							motors[MOTOR1].direction = DOWN;
							GPIOPinWrite(MOTOR1_DIR_PORT, MOTOR1_DIR_PIN, motors[MOTOR1].direction);
							break;
						case F3:
							motors[MOTOR2].timerVal = 65536;
							motors[MOTOR2].direction = UP;
							GPIOPinWrite(MOTOR2_DIR_PORT, MOTOR2_DIR_PIN, motors[MOTOR2].direction);
							break;
						case E3:
							motors[MOTOR2].timerVal = 65536;
							motors[MOTOR2].direction = DOWN;
							GPIOPinWrite(MOTOR2_DIR_PORT, MOTOR2_DIR_PIN, motors[MOTOR2].direction);
							break;
						case C4:
							motors[MOTOR3].timerVal = 65536;
							motors[MOTOR3].direction = UP;
							GPIOPinWrite(MOTOR3_DIR_PORT, MOTOR3_DIR_PIN, motors[MOTOR3].direction);
							break;
						case B3:
							motors[MOTOR3].timerVal = 65536;
							motors[MOTOR3].direction = DOWN;
							GPIOPinWrite(MOTOR3_DIR_PORT, MOTOR3_DIR_PIN, motors[MOTOR3].direction);
							break;
						default:
							break;
					}
				}
				break;
			}
	}
}

