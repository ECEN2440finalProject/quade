/*
 * uart.h
 *
 *  Created on: Oct 26, 2020
 *      Author: terry
 */

#ifndef UART_H_
#define UART_H_

#include "msp.h"
#include "stdbool.h"
#include "stdint.h"
#include "assert.h"

void config_uart(void);
void config_uart_interrupt(void);
void EUSCIA0_IRQHandler(void);
void start_uart(void);

// defines
#define READ        true
#define WRITE       false

#define BAUD_RATE 1500

// structs
typedef struct {
    uint8_t                 SA;         //Peripheral Receiver address to communicate with
    bool                    RW;         //read or write bit
    uint8_t                 *data;      //pointer to an array of data to be sent/received
    uint8_t                 dataBytes;  //how many bytes of data to transfer
    volatile uint8_t        byteCount;  //variable to keep track of how many bytes have TXed/RXed
    EUSCI_A_Type            *uart;      //Device Struct for accessing UART registers
    uint8_t                 deviceReg;  //register from which to read from

} UART_PAYLOAD_TypeDef;

#endif /* UART_H_ */
