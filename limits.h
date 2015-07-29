/*
 * limits.h
 *
 *  Created on: Sep 19, 2014
 *      Author: Anthony
 */

#ifndef LIMITS_H_
#define LIMITS_H_

#define MOTOR1_LIMIT GPIO_PIN_7
#define MOTOR2_LIMIT GPIO_PIN_5
#define MOTOR3_LIMIT GPIO_PIN_3

#define LOWER_LIMIT_OVERSHOOT -444	// how far past step count past zero motors are allowed to go before reset occurs.
#define UPPER_LIMIT_OVERSHOOT 444

void LimitsInit(void);
uint8_t LimitsCheck(void);

#endif /* LIMITS_H_ */
