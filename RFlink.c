/*
 * RFlink.c
 *
 *  Created on: Nov 3, 2020
 *  Modified:   Nov 4, 2020
 *      Author: terry
 */

#include "uart.h"
#include "RFlink.h"

void config_rx_gpio() {
    P3->DIR &= ~BIT2; // set P3.2 as input
    P3->DIR |= BIT3;  // set P3.3 as output (in case we need to transmit anything)

    // use select registers to set primary function (EUSCI_A2)
    P3->SEL0 |= BIT2 | BIT3;
    P3->SEL1 &= ~BIT2 & ~BIT3;
}

void rx_state(uint8_t rx_data, RX_UART_TypeDef * rx_uart) {
    switch(rx_uart->state) {
        case IDLE:
            if (rx_data == 0x00)
                rx_uart->state = HLF_STT;
            // otherwise it stays in idle state

        case HLF_STT: {
            if (rx_data == 0xF0)
                rx_uart->state = START;
            else
                rx_uart->state = IDLE;
        }

        case START:
            rx_uart->state = R_LEN;

        case R_LEN: {
            rx_uart->length = rx_data;
            rx_uart->state = R_DATA;
            rx_uart->i = 0;
        }

        case R_DATA: {
            if ((rx_uart->i += 8) <= rx_uart->length) {
                // read data, keep in R_DATA state
                switch(rx_uart->i) {
                case 8:
                    rx_uart->vertical += rx_data;       // Least significant 8 bits
                case 16:
                    rx_uart->vertical += rx_data * 0x20; // Most significant 8 bits
                case 24:
                    rx_uart->horizontal += rx_data;
                case 32:
                    rx_uart->horizontal += rx_data * 0x20;
                case 40:
                    rx_uart->swval += rx_data;
                case 48:
                    rx_uart->swval += rx_data;
                }
            }

            // otherwise, we have read enough bits to obtain necessary data
            else if (rx_data == 0x00)
                rx_uart->state = HLF_STP;
            else
                rx_uart->state = ERR;
        }

        case HLF_STP: {
            if (rx_data == 0xFF)
                rx_uart->state = IDLE;
            else
                rx_uart->state = ERR;
        }

        default: {
            // error state - set state back to idle
            rx_uart->state = IDLE;
        }

    }

}
