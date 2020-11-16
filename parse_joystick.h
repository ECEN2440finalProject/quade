/*
 * parse_joystick.h
 *
 *  Created on: Nov 16, 2020
 *      Author: terry
 */

#ifndef PARSE_JOYSTICK_H_
#define PARSE_JOYSTICK_H_

#include "RFlink.h"

// define macros for joystick position
#define JS_LEFT   1
#define JS_RIGHT -1
#define JS_UP     1
#define JS_DOWN  -1
#define JS_NEUTRAL 0

void joystick_pos(RX_UART_TypeDef * rx_data, int8_t * pos);

#endif /* PARSE_JOYSTICK_H_ */
