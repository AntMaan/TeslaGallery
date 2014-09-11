/*
 * midi.h
 *
 *  Created on: Mar 26, 2014
 *      Author: Anthony
 */

#ifndef MIDI_H_
#define MIDI_H_

#include <stdint.h>

// The channel this device is on
#define CHANNEL 0x01

#define NOTE_ON 0x90
#define NOTE_OFF 0x80

#define B2 0x2F
#define C3 0x30
#define E3 0x34
#define F3 0x35
#define B3 0x3B
#define C4 0x3C
#define C5 0x48
#define D5 0x4A
#define E5 0x4C

void ProcessMidi(char midi_char);
void MidiInit(void);
void MidiChannelUpdate(void);
void MidiSendMsg(uint8_t *buf, uint8_t length);

#endif /* MIDI_H_ */
