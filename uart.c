/*
 * uart.c
 *
 *  Created on: Oct 26, 2020
 *    Modified: Nov 4, 2020
 *      Author: terry
 */

#include "uart.h"
#include "RFlink.h"

//------------------------------------------------------------------------
// private functions
//------------------------------------------------------------------------
void config_uart(void) {
    // put EUSCI register in dormant state during config to save power
    UART_P->CTLW0 |= EUSCI_A_CTLW0_SWRST;       // reset EUSCI register during config
    UART_P->CTLW0 |= EUSCI_A_CTLW0_DORM;        // dormant state

    // config
    UART_P->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK; // select baud clock source as SMCLK (for now)
    UART_P->CTLW0 &= EUSCI_A_CTLW0_MODE_0;      // UART mode (without auto baud rate)
    UART_P->CTLW0 &= ~EUSCI_A_CTLW0_SEVENBIT;   // 8 bit word length
    UART_P->CTLW0 &= ~EUSCI_A_CTLW0_MSB;        // LSB sent first
    UART_P->CTLW0 &= ~EUSCI_A_CTLW0_SPB;        // one stop bit
    UART_P->CTLW0 &= ~EUSCI_A_CTLW0_PEN;        // parity disable
    UART_P->CTLW0 &= ~EUSCI_A_CTLW0_SYNC;       // synchronous mode disabled

    UART_P->CTLW0 |= EUSCI_A_CTLW0_BRKIE;       // enable interrupts (UCRXIFG) for break chars
    UART_P->CTLW0 &= ~EUSCI_A_CTLW0_RXEIE;      // reject all erroneous chars

    UART_P->MCTLW |= EUSCI_A_MCTLW_OS16;        // enable oversampling mode

    UART_P->BRW = BAUD_RATE;                    // set baud rate
    UART_P->CTLW1 &= EUSCI_A_CTLW1_GLIT_0;      // minimum deglitch time (~5 ns)
    UART_P->ABCTL &= ~EUSCI_A_ABCTL_ABDEN;      // disable auto baud rate detection
    UART_P->ABCTL |= EUSCI_A_ABCTL_DELIM_1;     // set break/synch delimiter length to 2

    UART_P->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;      // enable UART
}

void config_uart_interrupt(void) {
    // enable receive interrupts in IE register
    UART_P->IE |= EUSCI_A_IE_RXIE;

    NVIC_EnableIRQ(EUSCIA2_IRQn);       // enable interrupts in NVIC
}

// starts UART communication
void start_uart(void) {
    // ensure that any previous transmission has stopped
    while(UART_P->STATW & EUSCI_A_STATW_BUSY);

    // by reading RXBUF, we reset it before receiving first bit
    uint8_t err = UART_P->RXBUF & EUSCI_A_RXBUF_RXBUF_MASK;
    // ensure that RXBUF is cleared
    assert(UART_P->RXBUF == (UART_P->RXBUF & err));

    // bring EUSCI out of dormant state (if currently dormant)
    UART_P->CTLW0 &= ~EUSCI_A_CTLW0_DORM;

    UART_P->CTLW0 &= ~EUSCI_A_CTLW0_TXBRK;


}
