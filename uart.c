/*
 * uart.c
 *
 *  Created on: Oct 26, 2020
 *    Modified: Nov 16, 2020
 *      Author: terry
 */

#include "uart.h"
#include "RFlink.h"

//------------------------------------------------------------------------
// private functions
//------------------------------------------------------------------------
void config_uart(void) {
    UART_P->CTLW0 |= EUSCI_A_CTLW0_SWRST;       // reset EUSCI register during config

    // config
    UART_P->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK; // select baud clock source as SMCLK (for now)
    UART_P->MCTLW |= EUSCI_A_MCTLW_OS16;        // enable oversampling mode
    UART_P->MCTLW |= MOD_BITS;                  // set modulator bits for clock divider
    UART_P->BRW = CLOCK_PRESCALER;              // set prescaler (clock divider) for BRCLCK

    UART_P->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;      // enable UART
}

void config_uart_interrupt(void) {
    // enable receive interrupts in IE register
    UART_P->IE |= EUSCI_A_IE_RXIE;
    NVIC_EnableIRQ(EUSCIA2_IRQn);       // enable interrupts in NVIC
}

// starts UART communication
void start_uart(void) {
    // by reading RXBUF, we reset it before receiving first bit
    uint8_t err = UART_P->RXBUF & EUSCI_A_RXBUF_RXBUF_MASK;
    // ensure that RXBUF is cleared
    assert(UART_P->RXBUF == (UART_P->RXBUF & err));
}
