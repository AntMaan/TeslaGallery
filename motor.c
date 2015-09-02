/*
 * motor.c
 *
 *  Created on: Mar 26, 2014
 *      Author: Anthony
 */

#include <stdint.h>
#include <stdbool.h>

#include <inc/hw_memmap.h>

#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>
#include <driverlib/pwm.h>
#include <driverlib/sysctl.h>
#include <driverlib/interrupt.h>
#include <driverlib/eeprom.h>

#include <inc/hw_gpio.h>
#include <inc/hw_types.h>

#include "motor.h"
#include "timing.h"
#include "subsys.h"
#include "limits.h"
#include "eeprom.h"

version_t MOTOR_VERSION;

motor_control_t motors[NUM_MOTORS];
uint8_t blockReset = 0;

motorState_t motorState;

#define CAL_RATE 2500
#define RESET_RATE 2500

void MotorsInit(uint8_t type){

	uint32_t calFlag;

	MOTOR_VERSION.word = 0x14081000LU;

	blockReset = type;

	motorState = NORMAL;

	// Allow access to peripheral registers
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);

	// Setup our step pins as outputs
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_7);
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);

	// Configure the direction pins as outputs
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6);
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_1);

	// Configure the timers for one-shot
	TimerConfigure(TIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_ONE_SHOT);
	TimerConfigure(TIMER3_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_ONE_SHOT);
	TimerConfigure(TIMER4_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_ONE_SHOT);

	// Set the prescaler for the timers
	TimerPrescaleSet(TIMER2_BASE, TIMER_A, 1);
	TimerPrescaleSet(TIMER3_BASE, TIMER_A, 1);
	TimerPrescaleSet(TIMER4_BASE, TIMER_A, 1);

	// Make sure the interrupts are clear
	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
	TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);

	TimerIntRegister(TIMER2_BASE, TIMER_A, Motor1Step);
	TimerIntRegister(TIMER3_BASE, TIMER_A, Motor2Step);
	TimerIntRegister(TIMER4_BASE, TIMER_A, Motor3Step);

	//Enable the timeout interrupt on each timer
	TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
	TimerIntEnable(TIMER4_BASE, TIMER_TIMA_TIMEOUT);

	uint8_t i;
    for(i=0; i < NUM_MOTORS; i++){
        motors[i].deccelRate = DEFAULT_RATE;
        motors[i].accelRate = DEFAULT_RATE;
        motors[i].direction = UP;
        motors[i].timerVal = 65536;
        motors[i].steps = 0;
        // Add logic to read from EEPROM on the condition that the calibrated flag is set in EEPROM
        EEPROMRead(&calFlag, EE_ADDR_CALFLAG, 4);
        if(calFlag == EE_CALFLAG_SET){
        	EEPROMRead((uint32_t *) &motors[i].cal_steps, i * 4, 4);
        } else {
        	motors[i].cal_steps = -1;
        }
    }

	// Register a subsystem for logging LIMITS messages
	SubsystemInit(MOTOR, MESSAGE, "MOTOR", MOTOR_VERSION);
}


/**
 * 	@brief 		Routine to find the calibration values for all of the motors
 * 				The calibration values are the number of steps for each motor
 * 				that the actuator can travel before hitting the limit switch.
 * 				This routine assumes that the motor is in the desired "lowest
 * 				point" when the routine is called.
 *
 * 				This routine moves all motors "UP" until the limit switch for
 * 				that actuator is pressed, counting the number of steps to
 * 				travel to the limit switch.  These values are recorded and
 * 				written to memory to be used by the reset sequence to eliminate
 * 				drift that may accumulate over time.
 *
 * 				Note:  As this routine requires manual setup, it should only
 * 				be called when the designated Calibrate MIDI message has been
 * 				received
 *
 * 	@param[in]	None
 * 	@return		None
 */
void MotorsCalibrate(void){

	uint8_t motor_index;
	uint8_t limit_state;
	uint32_t calFlag;

	if(blockReset){
		return;
	}

	motorState = CAL;

	LogMsg(MOTOR, MESSAGE, "Starting Calibration!");

	// Make sure that all motors are off
	for(motor_index = 0; motor_index < NUM_MOTORS; motor_index++){
		if(motors[motor_index].timerVal != 65536){
			LogMsg(MOTOR, MESSAGE, "Can't calibrate, motors are active!");
			motorState = NORMAL;
			return;
		}
	}

	// Check to make sure none of the limits are already hit
	limit_state = LimitsCheck();
	if(limit_state){
		LogMsg(MOTOR, MESSAGE, "Can't calibrate, one of the limit switches is already pressed!");
		motorState = NORMAL;
		return;
	}

	// Reset all motor step counts and set the direction and rate
	for(motor_index = 0; motor_index < NUM_MOTORS; motor_index++){
		motors[motor_index].steps = 0;
		motors[motor_index].cal_steps = -1;
		if(motor_index == MOTOR1){
			motors[motor_index].direction = DOWN;
		} else {
			motors[motor_index].direction = UP;
		}
		motors[motor_index].timerVal = CAL_RATE;
	}

	// Make sure all of the direction pins represent the motor state
	GPIOPinWrite(MOTOR1_DIR_PORT, MOTOR1_DIR_PIN, motors[MOTOR1].direction);
	GPIOPinWrite(MOTOR2_DIR_PORT, MOTOR2_DIR_PIN, motors[MOTOR2].direction);
	GPIOPinWrite(MOTOR3_DIR_PORT, MOTOR3_DIR_PIN, motors[MOTOR3].direction);

	//Start moving all motors up
	MotorStart(MOTOR1);
	MotorStart(MOTOR2);
	MotorStart(MOTOR3);

	// Wait until all limits are pressed
	while(limit_state != (MOTOR1_LIMIT | MOTOR2_LIMIT | MOTOR3_LIMIT)){
		limit_state |= LimitsCheck();
		SystemTick();
		if(motorState == DISABLED){
			LogMsg(MOTOR, MESSAGE, "Calibration terminated!");
			// Maybe put a EEPROM read here to fix local cal_steps
			uint8_t i;
			for(i=0; i < NUM_MOTORS; i++){
				EEPROMRead(&calFlag, EE_ADDR_CALFLAG, sizeof(EE_CALFLAG_SET));
				if(calFlag == EE_CALFLAG_SET){
					EEPROMRead((uint32_t *) &motors[i].cal_steps, i * 4, 4);
				}
			}
			return;
		}
	}

	for(motor_index = 0; motor_index < NUM_MOTORS; motor_index++){
		SystemTick();
		motors[motor_index].cal_steps = motors[motor_index].steps;
		// Add EEPROM write here to save cal_steps values
		EEPROMProgram( (uint32_t *)&motors[motor_index].cal_steps, motor_index * 4, 4);
		LogMsg(MOTOR, MESSAGE, "Calibration value recorded- Motor %d Steps: %d", motor_index + 1, motors[motor_index].cal_steps);
	}
	calFlag = EE_CALFLAG_SET;
	EEPROMProgram(&calFlag, EE_ADDR_CALFLAG, sizeof(calFlag));

	// Do reset routine so that the motors go back to resting state
	MotorsReset();

	motorState = NORMAL;
}

enum reset_state {
	MOVING_TO_LIMITS = 0,
	MOVING_TO_ZERO
} reset_state;

/**
 * 	@brief		Routine to reset the motors back to "resting state"
 * 				using the calibration values.  This routine will simply
 * 				return if no calibration values are loaded.  The routine
 * 				works by bringing all motors up to the limit switch and
 * 				then moving down the number of steps in the calibration
 * 				value.  When the limit switch is pressed, the current step
 * 				count is set to the cal_step value.  Then, the motors will
 * 				move down until the step count is 0
 *
 * 	@param[in]	None
 * 	@return		None
 */
void MotorsReset(void){

	uint8_t motor_index;
	uint8_t limit_state;

	if(blockReset){
		return;
	}

	motorState = RESET;
	reset_state = MOVING_TO_LIMITS;

	// Make sure that all motors are off and we have calibration values to use
	for(motor_index = 0; motor_index < NUM_MOTORS; motor_index++){
		if(motors[motor_index].timerVal != 65536){
			LogMsg(MOTOR, MESSAGE, "Can't reset, motors are active!");
			motorState = NORMAL;
			return;
		}
		if(motors[motor_index].cal_steps < 0){
			LogMsg(MOTOR, MESSAGE, "Can't reset, no calibration values!");
			motorState = NORMAL;
			return;
		}
	}

	// Set the direction and rate
	for(motor_index = 0; motor_index < NUM_MOTORS; motor_index++){
		if(motor_index == MOTOR1){
			motors[motor_index].direction = DOWN;
		} else {
			motors[motor_index].direction = UP;
		}
		motors[motor_index].timerVal = RESET_RATE;
	}

	// Make sure all of the direction pins represent the motor state
	GPIOPinWrite(MOTOR1_DIR_PORT, MOTOR1_DIR_PIN, motors[MOTOR1].direction);
	GPIOPinWrite(MOTOR2_DIR_PORT, MOTOR2_DIR_PIN, motors[MOTOR2].direction);
	GPIOPinWrite(MOTOR3_DIR_PORT, MOTOR3_DIR_PIN, motors[MOTOR3].direction);

	// Check the current limit state
	limit_state = LimitsCheck();

	if(!(limit_state & MOTOR1_LIMIT)){
		MotorStart(MOTOR1);
	}

	if(!(limit_state & MOTOR2_LIMIT)){
		MotorStart(MOTOR2);
	}

	if(!(limit_state & MOTOR3_LIMIT)){
		MotorStart(MOTOR3);
	}

	// Wait until all limits are pressed
	while(limit_state != (MOTOR1_LIMIT | MOTOR2_LIMIT | MOTOR3_LIMIT)){
		limit_state |= LimitsCheck();
		SystemTick();
		if(motorState == DISABLED){
			LogMsg(MOTOR, MESSAGE, "Reset terminated!");
			return;
		}
	}

	reset_state = MOVING_TO_ZERO;

	// Now that all the limits are pressed, reset the current step count
	for(motor_index = 0; motor_index < NUM_MOTORS; motor_index++){
		motors[motor_index].steps = motors[motor_index].cal_steps;
		if(motor_index == MOTOR1){
			motors[motor_index].direction = UP;
		} else {
			motors[motor_index].direction = DOWN;
		}
		motors[motor_index].timerVal = RESET_RATE;
	}

	// Make sure all of the direction pins represent the motor state
	GPIOPinWrite(MOTOR1_DIR_PORT, MOTOR1_DIR_PIN, motors[MOTOR1].direction);
	GPIOPinWrite(MOTOR2_DIR_PORT, MOTOR2_DIR_PIN, motors[MOTOR2].direction);
	GPIOPinWrite(MOTOR3_DIR_PORT, MOTOR3_DIR_PIN, motors[MOTOR3].direction);

	//Start moving all motors down
	MotorStart(MOTOR1);
	MotorStart(MOTOR2);
	MotorStart(MOTOR3);

	/* Wait for the steps to equal 0, since we are in RESET state, the MotorStep
	 * functions will take care of turning the motors off */
	while(motors[MOTOR1].steps || motors[MOTOR2].steps || motors[MOTOR3].steps){
		SystemTick();
		if(motorState == DISABLED){
			LogMsg(MOTOR, MESSAGE, "Reset terminated!");
			return;
		}
	}

	LogMsg(MOTOR, MESSAGE, "Reset Finished!");

	motorState = NORMAL;
}


void MotorStart(uint8_t motor_index){

	switch (motor_index){
		case MOTOR1:
			TimerLoadSet(TIMER2_BASE, TIMER_A, MIN_PERIOD);
			TimerEnable(TIMER2_BASE, TIMER_A);
			break;
		case MOTOR2:
			TimerLoadSet(TIMER3_BASE, TIMER_A, MIN_PERIOD);
			TimerEnable(TIMER3_BASE, TIMER_A);
			break;
		case MOTOR3:
			TimerLoadSet(TIMER4_BASE, TIMER_A, MIN_PERIOD);
			TimerEnable(TIMER4_BASE, TIMER_A);
			break;
	}
}

void MotorStop(uint8_t motor_index){

	if(motor_index < NUM_MOTORS){
		switch (motor_index){
			case MOTOR1:
				TimerDisable(TIMER2_BASE, TIMER_A);
				TimerLoadSet(TIMER2_BASE, TIMER_A, MIN_PERIOD);
				break;
			case MOTOR2:
				TimerDisable(TIMER3_BASE, TIMER_A);
				TimerLoadSet(TIMER3_BASE, TIMER_A, MIN_PERIOD);
				break;
			case MOTOR3:
				TimerDisable(TIMER4_BASE, TIMER_A);
				TimerLoadSet(TIMER4_BASE, TIMER_A, MIN_PERIOD);
				break;
		}
		motors[motor_index].timerVal = 65536;
	}
}

void MotorsDisable(void){
	uint8_t i;

	for(i = 0; i < NUM_MOTORS; i++){
		MotorStop(i);
	}
	motorState = DISABLED;
	LogMsg(MOTOR, MESSAGE, "Motors Disabled");
}

void MotorsEnable(void){
	motorState = NORMAL;
	LogMsg(MOTOR, MESSAGE, "Motors Enabled");
}

void Motor1Step(void){
	uint32_t currentTimerVal;
	uint32_t newTimerVal;
	// Clear the interrupt
	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	// Get the current state of the step pin
	uint8_t pin_state = GPIOPinRead(MOTOR1_STEP_PORT, MOTOR1_STEP_PIN);
	// Toggle the pin
	GPIOPinWrite(MOTOR1_STEP_PORT, MOTOR1_STEP_PIN, ~(pin_state & MOTOR1_STEP_PIN));
	/* If the pin was off before, then we just turned it on, which
	 * means we have just stepped, so update the count */
	if(pin_state == 0x00) {
		if(!motors[MOTOR1].direction){
			motors[MOTOR1].steps++;
		} else {
			motors[MOTOR1].steps--;
		}
	}

	if( (motorState == RESET) && (motors[MOTOR1].steps == 0) && (reset_state == MOVING_TO_ZERO) ){
		MotorStop(MOTOR1);
		return;
	}

	if(pin_state == 0x00) {
		// Get the previous timer value
		currentTimerVal = TimerLoadGet(TIMER2_BASE, TIMER_A);
		// If the current timer value is greater than the desired timerVal, decrease the timer load
		if(currentTimerVal > motors[MOTOR1].timerVal){
			newTimerVal = currentTimerVal - motors[MOTOR1].accelRate*currentTimerVal;
		} else if (currentTimerVal < motors[MOTOR1].timerVal){
			newTimerVal = currentTimerVal + motors[MOTOR1].deccelRate*currentTimerVal;
		}
		else{
			newTimerVal = currentTimerVal;
		}

		// As long as the newTimerVal is less than MIN_PERIOD, re-enable the timer with the updated value
		if(newTimerVal <= MIN_PERIOD){
			TimerLoadSet(TIMER2_BASE, TIMER_A, newTimerVal);
			TimerEnable(TIMER2_BASE, TIMER_A);
		}
	} else {
		TimerEnable(TIMER2_BASE, TIMER_A);
	}
}

void Motor2Step(void){
	uint32_t currentTimerVal;
	uint32_t newTimerVal;
	// Clear the interrupt
	TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
	// Get the current state of the step pin
	uint8_t pin_state = GPIOPinRead(MOTOR2_STEP_PORT, MOTOR2_STEP_PIN);
	// Toggle the pin
	GPIOPinWrite(MOTOR2_STEP_PORT, MOTOR2_STEP_PIN, ~(pin_state & MOTOR2_STEP_PIN));
	/* If the pin was off before, then we just turned it on, which
	 * means we have just stepped, so update the count */
	if(pin_state == 0x00) {
		if(motors[MOTOR2].direction){
			motors[MOTOR2].steps++;
		} else {
			motors[MOTOR2].steps--;
		}
	}

	if( (motorState == RESET) && (motors[MOTOR2].steps == 0) && (reset_state == MOVING_TO_ZERO) ){
		MotorStop(MOTOR2);
		return;
	}

	if(pin_state == 0x00) {
		// Get the previous timer value
		currentTimerVal = TimerLoadGet(TIMER3_BASE, TIMER_A);
		// If the current timer value is greater than the desired timerVal, decrease the timer load
		if(currentTimerVal > motors[MOTOR2].timerVal){
			newTimerVal = currentTimerVal - motors[MOTOR2].accelRate*currentTimerVal;
		} else if (currentTimerVal < motors[MOTOR2].timerVal){
			newTimerVal = currentTimerVal + motors[MOTOR2].deccelRate*currentTimerVal;
		}
		else{
			newTimerVal = currentTimerVal;
		}

		// As long as the newTimerVal is less than MIN_PERIOD, re-enable the timer with the updated value
		if(newTimerVal <= MIN_PERIOD){
			TimerLoadSet(TIMER3_BASE, TIMER_A, newTimerVal);
			TimerEnable(TIMER3_BASE, TIMER_A);
		}
	} else {
		TimerEnable(TIMER3_BASE, TIMER_A);
	}
}

void Motor3Step(void){
	uint32_t currentTimerVal;
	uint32_t newTimerVal;
	// Clear the interrupt
	TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
	// Get the current state of the step pin
	uint8_t pin_state = GPIOPinRead(MOTOR3_STEP_PORT, MOTOR3_STEP_PIN);
	// Toggle the pin
	GPIOPinWrite(MOTOR3_STEP_PORT, MOTOR3_STEP_PIN, ~(pin_state & MOTOR3_STEP_PIN));
	/* If the pin was off before, then we just turned it on, which
	 * means we have just stepped, so update the count */
	if(pin_state == 0x00) {
		if(motors[MOTOR3].direction){
			motors[MOTOR3].steps++;
		} else {
			motors[MOTOR3].steps--;
		}
	}

	if( (motorState == RESET) && (motors[MOTOR3].steps == 0) && (reset_state == MOVING_TO_ZERO) ){
		MotorStop(MOTOR3);
		return;
	}

	if(pin_state == 0x00) {
		// Get the previous timer value
		currentTimerVal = TimerLoadGet(TIMER4_BASE, TIMER_A);
		// If the current timer value is greater than the desired timerVal, decrease the timer load
		if(currentTimerVal > motors[MOTOR3].timerVal){
			newTimerVal = currentTimerVal - motors[MOTOR3].accelRate*currentTimerVal;
		} else if (currentTimerVal < motors[MOTOR3].timerVal){
			newTimerVal = currentTimerVal + motors[MOTOR3].deccelRate*currentTimerVal;
		}
		else{
			newTimerVal = currentTimerVal;
		}

		// As long as the newTimerVal is less than MIN_PERIOD, re-enable the timer with the updated value
		if(newTimerVal <= MIN_PERIOD){
			TimerLoadSet(TIMER4_BASE, TIMER_A, newTimerVal);
			TimerEnable(TIMER4_BASE, TIMER_A);
		}
	} else {
		TimerEnable(TIMER4_BASE, TIMER_A);
	}
}
