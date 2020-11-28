/*
 * thermistor.h
 *
 *  Created on: Nov 27, 2020
 *      Author: terry
 */

#ifndef THERMISTOR_H_
#define THERMISTOR_H_

// Includes--from driverlib (TI open source)
#include "msp.h"
#include "adc14.h"
#include "gpio.h"
#include "interrupt.h"
#include "debug.h"
#include "cpu.h"
#include "sysctl.h"
#include "sysctl_a.h"



void adc_config(void);
void adc_pin_config(void);
void adc_enable_interrupt(void);
void start_adc(void);


#endif /* THERMISTOR_H_ */
