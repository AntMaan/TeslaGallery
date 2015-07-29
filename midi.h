/*
 * midi.h
 *
 *  Created on: Mar 26, 2014
 *      Author: Anthony
 */

#ifndef MIDI_H_
#define MIDI_H_

#include <stdint.h>
#include "int_def.h"

#define NOTE_ON 0x90
#define NOTE_OFF 0x80

#define C1 0x0C
#define B2 0x2F
#define C3 0x30
#define E3 0x34
#define F3 0x35
#define B3 0x3B
#define C4 0x3C
#define C5 0x48
#define D5 0x4A
#define E5 0x4C
#define F5 0x4D

#define CHANNEL_0 0x00
#define CHANNEL_1 0x01
#define CHANNEL_2 0x02
#define CHANNEL_3 0x03
#define CHANNEL_4 0x04
#define CHANNEL_5 0x05
#define CHANNEL_6 0x06
#define CHANNEL_7 0x07
#define CHANNEL_8 0x08
#define CHANNEL_9 0x09
#define CHANNEL_10 0x0A
#define CHANNEL_11 0x0B
#define CHANNEL_12 0x0C
#define CHANNEL_13 0x0D
#define CHANNEL_14 0x0E
#define CHANNEL_15 0x0F

extern union8_t channel;

void ProcessMidi(char midi_char);
void MidiInit(void);
void MidiChannelUpdate(void);
void MidiSendMsg(uint8_t *buf, uint8_t length);

#endif /* MIDI_H_ */
