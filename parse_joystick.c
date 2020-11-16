/*
 * parse_joystick.c
 *
 *  Created on: Nov 16, 2020
 *      Author: terry
 */

#include "parse_joystick.h"

// returns void, but accepts pointer to int8_t array for positions
// position 0 is vertical, 1 is horizontal, 2 is switch
void joystick_pos(RX_UART_TypeDef * rx_data, int8_t * pos) {
    // Vertical
    if (rx_data->vertical < 1024/4)
        pos[0] = JS_DOWN;
    else if (rx_data->vertical > 1024*3/4)
        pos[0] = JS_UP;
    else pos[0] = JS_NEUTRAL;

    // Horizontal
    if (rx_data->horizontal < 1024/4)
        pos[1] = JS_RIGHT;
    else if (rx_data->vertical > 1024*3/4)
        pos[1] = JS_LEFT;
    else pos[1] = JS_NEUTRAL;

    // Switch (button)
    pos[2] = rx_data->swval; // outputs digital value (0 or 1) so assign that directly to pos[2]
}


