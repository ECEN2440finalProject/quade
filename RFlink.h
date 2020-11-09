/*
 * RFlink.h
 *
 *  Created on: Nov 3, 2020
 *  Modified:   Nov 4, 2020
 *      Author: terry
 */
#include "msp.h"

#ifndef RFLINK_H_
#define RFLINK_H_

// state machine defines
#define IDLE    0
#define HLF_STT 1
#define START   2
#define R_LEN   3
#define R_DATA  4
#define HLF_STP 5
#define ERR     6

typedef struct {
    uint16_t    vertical;
    uint16_t    horizontal;
    uint16_t    swval;

    uint8_t     state; // state variable
    uint8_t     length;// length of data to be received (should be 48 for our purposes)
    uint8_t     i;     // iteration variable (count number of bits received)

} RX_UART_TypeDef;

typedef struct {
    uint8_t         read;   // current read position
    uint8_t         write;  // current write position
    uint8_t         size;   // size of buffer
    RX_UART_TypeDef *data;  // array of structs for data from receiver
} RX_UART_BUF_TypeDef;

void config_rx_gpio();
uint8_t rx_buf_len(RX_UART_BUF_TypeDef *rx_buf);

// Both of the following functions return 0 if successful, 1 if erroneous
uint8_t rx_buf_write(RX_UART_BUF_TypeDef *rx_buf, RX_UART_TypeDef data);
uint8_t rx_buf_read(RX_UART_BUF_TypeDef *rx_buf, RX_UART_TypeDef *data);

void rx_state(uint8_t rx_data, RX_UART_TypeDef * rx_uart);

#endif /* RFLINK_H_ */
