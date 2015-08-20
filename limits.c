/*
 * limits.c
 *
 *  Created on: Sep 19, 2014
 *      Author: Anthony
 */

#include <stdint.h>

#include "inc/hw_memmap.h"

#include "subsys.h"

#include "motor.h"
#include "limits.h"

version_t LIMITS_VERSION;

/**
 * 	@brief		Configure the pins used for inputs of the limit switches.
 *
 * 	@param[in]	None
 * 	@return		None
 */
void LimitsInit(void){

	LIMITS_VERSION.word = 0x14081000LU;

	// Set the pin type to input for the pins
	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_3 | GPIO_PIN_5);
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_7);

	// Set the pins to have a pull-up resistor
	GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_3 | GPIO_PIN_5, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
	GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);

	// Register a subsystem for logging LIMITS messages
	SubsystemInit(LIMITS, MESSAGE, "LIMITS", LIMITS_VERSION);

#ifdef LIMITS_VERBOSE
		LogMsg(LIMITS, MESSAGE, "System initialized");
#endif
}

/**
 *  @brief		Checks the hard (limit switches) and soft (step count) limits for
 *  			each motor to make sure that it has not gone outside of the normal
 *  			operating region.
 *
 *	@param[in]	None
 *	@return		Returns the bit-packed representation of the limit switches in position
 *				of the GPIO_PIN_X they are on.
 *
 */
uint8_t LimitsCheck(void){

	uint8_t switch_state;
	uint8_t motor_index;

	switch_state = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_3 | GPIO_PIN_5);
	switch_state += GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_7);
	switch_state ^= MOTOR1_LIMIT | MOTOR2_LIMIT | MOTOR3_LIMIT;

	// Check the limit switches
	if(switch_state){
		if(motorState == NORMAL) {
			MotorsDisable();
			MotorsReset();
		} else {
			if(switch_state & MOTOR1_LIMIT){
				MotorStop(MOTOR1);
			}
			if(switch_state & MOTOR2_LIMIT){
				MotorStop(MOTOR2);
			}
			if(switch_state & MOTOR3_LIMIT){
				MotorStop(MOTOR3);
			}
		}
		return switch_state;
	}

	if(motorState != RESET){

		// Don't check the soft limits if PC6 is low.  This means that it is a small doll and has no limits
		if(GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6)){
			// Check the soft limits
			for(motor_index = 0; motor_index < NUM_MOTORS; motor_index++){
				// We can only check soft limits if we have previously calibrated
				if(motors[motor_index].cal_steps > 0){
					// Check the soft upper limits
					if(motors[motor_index].steps >= motors[motor_index].cal_steps + UPPER_LIMIT_OVERSHOOT ){
						MotorsDisable();
						DelayMs(100);

						#ifdef LIMITS_VERBOSE
							LogMsg(LIMITS, MESSAGE, "Soft Upper Limit Reached: Motor %d", motor_index + 1);
						#endif

						/* We decided that if you hit the soft upper limit, the limit switch is probably not working
						   Therefore, don't reset and basically leave the system locked out */
						// MotorsReset();

						return switch_state;
					}

					// Check the soft lower limits
					if(motors[motor_index].steps <= LOWER_LIMIT_OVERSHOOT){
						MotorsDisable();
						DelayMs(100);

						#ifdef LIMITS_VERBOSE
							LogMsg(LIMITS, MESSAGE, "Soft Lower Limit Reached: Motor %d", motor_index + 1);
						#endif

						MotorsReset();
						return switch_state;
					}
				}
			}
		}
	}
	return switch_state;
}
