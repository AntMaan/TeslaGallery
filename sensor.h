/*
 * sensor.h
 *
 *  Created on: Aug 10, 2014
 *      Author: Anthony
 */

#ifndef SENSOR_H_
#define SENSOR_H_

void SensorInit(void);
void Sensor1ISR(void);
void Sensor2ISR(void);

void SensorChangeTimeout(tint_t timeout);
void SensorChangeThreshold(uint32_t theshold);

#define DEFAULT_TRIGGER_TIMEOUT 10000
#define DEFAULT_THRESHOLD 2048
#define SENSOR_HYST_WINDOW 200

#define MAX_THRESHOLD_VALUE 4096 - (SENSOR_HYST_WINDOW << 1)
#define MIN_THESHOLD_VALUE 0 + (SENSOR_HYST_WINDOW << 1)

#endif /* SENSOR_H_ */
