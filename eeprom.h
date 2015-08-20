/*
 * eeprom.h
 *
 *  Created on: Aug 20, 2015
 *      Author: Anthony
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#define EE_ADDR_MOTOR1 0x0000
#define EE_ADDR_MOTOR2 0x0004
#define EE_ADDR_MOTOR3 0x0008
#define EE_ADDR_SENSOR_THRESHOLD 0x000C
#define EE_ADDR_SENSOR_TIMEOUT 0x0010

#define EE_ADDR_CALFLAG 0x0064
#define EE_ADDR_SENSOR_SETUP 0x0068

#define EE_CALFLAG_SET 0xFADE
#define EE_SENSOR_SET 0xFADE


#endif /* EEPROM_H_ */
