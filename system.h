/*
 * system.h
 *
 *  Created on: Mar 26, 2014
 *      Author: Anthony
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdint.h>
#include <stdbool.h>

#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_ints.h>
#include <inc/hw_gpio.h>

#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>

#define PERIPHERAL_CLOCK SysCtlClockGet()
#define SYSTEM_CLOCK SysCtlClockGet()

#define MOTOR1_DIR_PORT GPIO_PORTC_BASE
#define MOTOR1_DIR_PIN GPIO_PIN_5

#define MOTOR2_DIR_PORT GPIO_PORTD_BASE
#define MOTOR2_DIR_PIN GPIO_PIN_1

#define MOTOR3_DIR_PORT GPIO_PORTA_BASE
#define MOTOR3_DIR_PIN GPIO_PIN_6

// subsytem enumeration needed for the subsys module
enum sys_index {
    SYSTEM = 0,
    TESLA,
    MIDI,
    SENSOR,
    // ADD SUBSYSTEMS BELOW //
    // ADD SUBSYSTEMS ABOVE //
    UNKNOWN
};

#define SUBSYS_USE_UART0
#define USE_UART0
#define UART0_BAUD 115200
#define USE_UART1
#define UART1_BAUD 31250

#define LOG_BUF &tx0

void system_init();

#endif /* SYSTEM_H_ */
