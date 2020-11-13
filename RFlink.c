/*
 * RFlink.c
 *
 *  Created on: Nov 3, 2020
 *  Modified:   Nov 9, 2020
 *      Author: terry
 */

#include "uart.h"
#include "RFlink.h"

// GPIO config for UART
void config_rx_gpio() {
    P3->DIR &= ~BIT2; // set P3.2 as input
    P3->DIR |= BIT3;  // set P3.3 as output (in case we need to transmit anything)

    // use select registers to set primary function (EUSCI_A2)
    P3->SEL0 |= BIT2 | BIT3;
    P3->SEL1 &= ~BIT2 & ~BIT3;
}

// Returns length of buffer
uint8_t rx_buf_len(RX_UART_BUF_TypeDef *rx_buf) {
    int16_t length = rx_buf->write - rx_buf->read;
    return length & (rx_buf->size - 1); // works b/c 2s complement
}

//------------------------------------------------------------------------
// Both rx_buf_write and rx_buf_read return 0 if successful
//------------------------------------------------------------------------

// Write Rx data to buffer
uint8_t rx_buf_write(RX_UART_BUF_TypeDef *rx_buf, RX_UART_TypeDef data) {
    // check input first--buffer full?
    if (rx_buf_len(rx_buf) == rx_buf->size - 1)
        return 1;

    rx_buf->data[rx_buf->write] = data;                     // write data
    rx_buf->write = (rx_buf->write+1) & (rx_buf->size-1);   // advance write variable
    return 0;
}

// Read Rx data from buffer (before converting to PWM for servos)
uint8_t rx_buf_read(RX_UART_BUF_TypeDef *rx_buf, RX_UART_TypeDef *data) {
    // check input first--buffer empty?
    if (rx_buf_len(rx_buf) == 0)
        return 1;

    *data = rx_buf->data[rx_buf->read];                 // read and store data
    rx_buf->read = (rx_buf->read+1) & (rx_buf->size-1); // advance read variable
    return 0;
}

// Next State Logic for state machine
void rx_state(uint8_t rx_data, RX_UART_TypeDef * rx_uart) {
    switch(rx_uart->state) {
        case IDLE:
            if (rx_data == 0x00)
                rx_uart->state = HLF_STT;
            break;
            // otherwise it stays in idle state

        case HLF_STT:
            if (rx_data == 0xF0)
                rx_uart->state = START;
            else
                rx_uart->state = IDLE;
            break;

        case START:
            rx_uart->state = R_LEN;
            break;

        case R_LEN:
            rx_uart->length = rx_data;
            rx_uart->state = R_DATA;
            rx_uart->i = 0;
            break;

        case R_DATA:
            if ((rx_uart->i += 8) <= rx_uart->length) {
                // read data, keep in R_DATA state
                switch(rx_uart->i) {
                case 8:
                    rx_uart->vertical += rx_data;       // Least significant 8 bits
                    break;
                case 16:
                    rx_uart->vertical += rx_data * 0x100;// Most significant 8 bits
                    break;
                case 24:
                    rx_uart->horizontal += rx_data;
                    break;
                case 32:
                    rx_uart->horizontal += rx_data * 0x100;
                    break;
                case 40:
                    rx_uart->swval += rx_data;
                    break;
                case 48:
                    rx_uart->swval += rx_data * 0x100;
                    break;
                }
            }

            // otherwise, we have read enough bits to obtain necessary data
            else if (rx_data == 0x00)
                rx_uart->state = HLF_STP;
            else
                rx_uart->state = ERR;
        break;

        case HLF_STP:
            if (rx_data == 0xFF)
                rx_uart->state = IDLE;
            else
                rx_uart->state = ERR;
        break;

        default:
            // error state - set state back to idle
            rx_uart->state = IDLE;
    }

}
