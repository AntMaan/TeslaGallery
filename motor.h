/*
 * motor.h
 *
 *  Created on: Mar 26, 2014
 *      Author: Anthony
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/pwm.h>

#include "system.h"

/*
 * Initialize all pins and register with system using GPIO libraries.
 * NOTE: If a new motor is added to the pwm_motors list, please make
 * 		 it is initialized in this function.
 */
void MotorsInit(void);
void MotorsUpdate(void);
void MotorsDisable(void);
void MotorsEnable(void);
void MotorStart(uint8_t motor_index);
void MotorStop(uint8_t motor_index);
void MotorsCalibrate(void);
void MotorsReset(void);
void Motor1Step(void);
void Motor2Step(void);
void Motor3Step(void);

#define DEFAULT_RATE .01
#define MIN_PERIOD 65535

#define MOTOR1_DIR_PORT GPIO_PORTC_BASE
#define MOTOR1_DIR_PIN GPIO_PIN_5
#define MOTOR1_STEP_PORT GPIO_PORTC_BASE
#define MOTOR1_STEP_PIN GPIO_PIN_4

#define MOTOR2_DIR_PORT GPIO_PORTD_BASE
#define MOTOR2_DIR_PIN GPIO_PIN_1
#define MOTOR2_STEP_PORT GPIO_PORTD_BASE
#define MOTOR2_STEP_PIN GPIO_PIN_0

#define MOTOR3_DIR_PORT GPIO_PORTA_BASE
#define MOTOR3_DIR_PIN GPIO_PIN_6
#define MOTOR3_STEP_PORT GPIO_PORTA_BASE
#define MOTOR3_STEP_PIN GPIO_PIN_7

typedef enum  {
	MOTOR1 = 0,
	MOTOR2,
	MOTOR3,
	NUM_MOTORS
} motor_index_t;

#define UP (~0)
#define DOWN 0

typedef struct {
	uint32_t timerVal;
	uint8_t direction;
	float accelRate;
	float deccelRate;
	int32_t steps;
	int32_t cal_steps;
} motor_control_t;

typedef enum {
	NORMAL = 0,
	CAL,
	RESET,
	DISABLED
} motorState_t;

extern motor_control_t motors[NUM_MOTORS];
extern motorState_t motorState;

#endif /* MOTOR_H_ */
