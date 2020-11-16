/*
 * uart.h
 *
 *  Created on: Oct 26, 2020
 *    Modified: Nov 16, 2020
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

//--------------------------------------------
//go to:
//http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
//to find these numbers
//currently at 3MHz clock with 1200bpsbaud rate

#define CLOCK_PRESCALER 156
//4 bits only!!
#define FIRST_MOD 0x4
//8 bits only!!
#define SECOND_MOD 0x00

//MCTLW REGISTER BITS:
// [2ND MOD x 8            ] [1ST MOD x 4] [RES X 3 ][OVR SMPL x 1]
// [15 14 13 12 11 10 09 08] [07 06 05 04] [03 02 01][00]
#define MOD_BITS ((SECOND_MOD << 8) | (FIRST_MOD << 4))
//--------------------------------------------


#define UART_P EUSCI_A2

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
