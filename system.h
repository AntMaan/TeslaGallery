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

// subsytem enumeration needed for the subsys module
enum sys_index {
    SYSTEM = 0,
    TESLA,
    MIDI,
    MOTOR,
    SENSOR,
    LIMITS,
    // ADD SUBSYSTEMS BELOW //
    // ADD SUBSYSTEMS ABOVE //
    UNKNOWN
};

#define SUBSYS_USE_UART0
#define USE_UART0
#define UART0_BAUD 115200
#define USE_UART1
#define UART1_BAUD 31250
#define UART_RX_BUFFER_LENGTH 128
#define UART_TX_BUFFER_LENGTH 256

#define LOG_BUF &tx0

void system_init();

#endif /* SYSTEM_H_ */
