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
void MotorsKillAll(void);
void Motor1Step(void);
void Motor2Step(void);
void Motor3Step(void);

// CPU/MOTOR Configurations
#define CLK_FREQ 80000000
#define PWM_CLK_DIV 8

#define PWM_TIMER_FREQ (CLK_FREQ/PWM_CLK_DIV) // Hz

#define DEFAULT_RATE 1000
#define MIN_PERIOD 65535

typedef enum  {
	MOTOR1 = 0,
	MOTOR2,
	MOTOR3,
	NUM_MOTORS
} motor_index_t;

#define UP 0
#define DOWN (~0)

typedef struct {
	uint32_t pwm_base_module;
	uint32_t pwm_generator;
	uint32_t pwm_genFault;
	uint32_t pwm_interrupt;
	uint32_t pwm_pin;
	uint32_t dir_port;
	uint8_t dir_pin;
} motor_config_t;

typedef struct {
	uint32_t period;
	uint8_t direction;
	uint16_t accelRate;
	uint16_t deccelRate;
	int16_t steps;
} motor_control_t;

// Define config information for the motors
static motor_config_t motor_config[NUM_MOTORS] = {
	{// Motor 1
		PWM0_BASE,
		PWM_GEN_3,
		PWM_INT_GEN_3,
		INT_PWM0_3,
		PWM_OUT_6,
		MOTOR1_DIR_PORT,
		MOTOR1_DIR_PIN
	},
	{// Motor 2
		PWM1_BASE,
		PWM_GEN_0,
		PWM_INT_GEN_0,
		INT_PWM0_0,
		PWM_OUT_0,
		MOTOR2_DIR_PORT,
		MOTOR2_DIR_PIN
	},
	{// Motor 3
		PWM1_BASE,
		PWM_GEN_1,
		PWM_INT_GEN_1,
		INT_PWM1_1,
		PWM_OUT_3,
		MOTOR3_DIR_PORT,
		MOTOR3_DIR_PIN
	}
};

extern motor_control_t motors[NUM_MOTORS];

#endif /* MOTOR_H_ */
