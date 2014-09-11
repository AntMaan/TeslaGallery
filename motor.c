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

#include <inc/hw_gpio.h>
#include <inc/hw_types.h>

#include "motor.h"
#include "timing.h"

#define NEW_PERIOD_INCREASING (currentPeriod + motors[i].deccelRate)

#define NEW_PERIOD_DECREASING (currentPeriod - motors[i].accelRate)

#define PIN_MASK (1 << (0x0000000F & motor_config[i].pwm_pin))


motor_control_t motors[NUM_MOTORS];

void MotorsInit(void){

	// Allow access to both PWM module registers
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

	// Configure the outputs for PWM output
	GPIOPinConfigure(GPIO_PC4_M0PWM6);
	GPIOPinConfigure(GPIO_PD0_M1PWM0);
	GPIOPinConfigure(GPIO_PA7_M1PWM3);
	GPIOPinConfigure(GPIO_PB6_M0PWM0);

	GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_7);
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
	GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4);
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);

	/* PF0 requires unlocking before configuration */
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
	GPIOPinConfigure(GPIO_PF0_M1PWM4);
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_0);
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_M;

	// Configure the direction pins as outputs
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_1);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6);

	SysCtlPWMClockSet(SYSCTL_PWMDIV_8);

	uint8_t i;
	for(i=0; i < NUM_MOTORS; i++){
		motors[i].deccelRate = DEFAULT_RATE;
		motors[i].accelRate = DEFAULT_RATE;
		motors[i].direction = UP;
		motors[i].period = 1;
		motors[i].steps = 0;
		PWMGenConfigure(motor_config[i].pwm_base_module, motor_config[i].pwm_generator, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_DB_SYNC_LOCAL | PWM_GEN_MODE_DBG_RUN);
		PWMGenPeriodSet(motor_config[i].pwm_base_module, motor_config[i].pwm_generator, motors[i].period);
		PWMPulseWidthSet(motor_config[i].pwm_base_module, motor_config[i].pwm_pin, motors[i].period/2);
		PWMGenIntClear(motor_config[0].pwm_base_module, motor_config[0].pwm_generator, PWM_INT_CNT_ZERO);
		PWMGenIntTrigEnable(motor_config[i].pwm_base_module, motor_config[i].pwm_generator, PWM_INT_CNT_ZERO);
		PWMIntEnable(motor_config[i].pwm_base_module, motor_config[i].pwm_genFault);
		IntEnable(motor_config[i].pwm_interrupt);
		PWMGenEnable(motor_config[i].pwm_base_module, motor_config[i].pwm_generator);
		PWMOutputState(motor_config[i].pwm_base_module, PIN_MASK, 0);
	}
}

void MotorsKillAll(void){
	uint8_t i;
	for(i = 0; i < NUM_MOTORS; i++){
		motors[i].period = 0;
	}
}

void MotorsUpdate(void){
	uint8_t i;

	for(i=0; i<NUM_MOTORS; i++){
		uint32_t currentPeriod = PWMGenPeriodGet(motor_config[i].pwm_base_module, motor_config[i].pwm_generator);
		if (currentPeriod < motors[i].period)
		{
			if(currentPeriod == 1)
			{
				//Update the period and make the duty cycle ~50%
				PWMGenPeriodSet(motor_config[i].pwm_base_module, motor_config[i].pwm_generator, MIN_PERIOD);
				PWMPulseWidthSet(motor_config[i].pwm_base_module, motor_config[i].pwm_pin, MIN_PERIOD/2);
				GPIOPinWrite(motor_config[i].dir_port, motor_config[i].dir_pin, motors[i].direction);
				// Make sure the output is on
				PWMOutputState(motor_config[i].pwm_base_module, PIN_MASK, 1);
			}
		}
		else if(currentPeriod > motors[i].period){
			if (motors[i].period == 0){
				if(NEW_PERIOD_DECREASING > MIN_PERIOD)
				{
					PWMGenPeriodSet(motor_config[i].pwm_base_module, motor_config[i].pwm_generator, 1);
					PWMPulseWidthSet(motor_config[i].pwm_base_module, motor_config[i].pwm_pin, 0);
					GPIOPinWrite(motor_config[i].dir_port, motor_config[i].dir_pin, motors[i].direction);
					// Make sure the output is off
					PWMOutputState(motor_config[i].pwm_base_module, PIN_MASK, 1);
				}
				else{
					PWMGenPeriodSet(motor_config[i].pwm_base_module, motor_config[i].pwm_generator, NEW_PERIOD_INCREASING);
					PWMPulseWidthSet(motor_config[i].pwm_base_module, motor_config[i].pwm_pin, NEW_PERIOD_INCREASING/2);
					GPIOPinWrite(motor_config[i].dir_port, motor_config[i].dir_pin, motors[i].direction);
					// Make sure the output is on
					PWMOutputState(motor_config[i].pwm_base_module, PIN_MASK, 1);
				}
			}
			else if(NEW_PERIOD_DECREASING <= motors[i].period){
				PWMGenPeriodSet(motor_config[i].pwm_base_module, motor_config[i].pwm_generator, motors[i].period);
				PWMPulseWidthSet(motor_config[i].pwm_base_module, motor_config[i].pwm_pin, motors[i].period/2);
				// Set the direction
				GPIOPinWrite(motor_config[i].dir_port, motor_config[i].dir_pin, motors[i].direction);
				// Make sure the output is on
				PWMOutputState(motor_config[i].pwm_base_module, PIN_MASK, 1);
			}
			else{
				PWMGenPeriodSet(motor_config[i].pwm_base_module, motor_config[i].pwm_generator, NEW_PERIOD_DECREASING);
				PWMPulseWidthSet(motor_config[i].pwm_base_module, motor_config[i].pwm_pin, NEW_PERIOD_DECREASING/2);
				// Set the direction
				GPIOPinWrite(motor_config[i].dir_port, motor_config[i].dir_pin, motors[i].direction);
				// Make sure the output is on
				PWMOutputState(motor_config[i].pwm_base_module, PIN_MASK, 1);

			}
		}
	}
}

void Motor1Step(void){
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, ~(GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_7) & GPIO_PIN_7));
	PWMGenIntClear(motor_config[0].pwm_base_module, motor_config[0].pwm_generator, PWM_INT_CNT_ZERO);
	if(GPIOPinRead(motor_config[0].dir_port, motor_config[0].dir_pin) == UP){
		motors[0].steps++;
	} else {
		motors[0].steps--;
	}
}

void Motor2Step(void){
	PWMGenIntClear(motor_config[1].pwm_base_module, motor_config[1].pwm_generator, PWM_INT_CNT_ZERO);
	if(GPIOPinRead(motor_config[1].dir_port, motor_config[1].dir_pin) == UP){
		motors[1].steps++;
	} else {
		motors[1].steps--;
	}
/*	int i = 5000;
	while(i--);*/
}
void Motor3Step(void){
	PWMGenIntClear(motor_config[2].pwm_base_module, motor_config[2].pwm_generator, PWM_INT_CNT_ZERO);
	if(GPIOPinRead(motor_config[2].dir_port, motor_config[2].dir_pin) == UP){
		motors[2].steps++;
	} else {
		motors[2].steps--;
	}
/*	int i = 5000;
	while(i--);*/
}
