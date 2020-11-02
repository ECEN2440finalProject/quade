/*
 * uart.c
 *
 *  Created on: Oct 26, 2020
 *    Modified: Nov 2, 2020
 *      Author: terry
 */

#include "uart.h"

//private variables
static UART_PAYLOAD_TypeDef uart_payload_struct;

//------------------------------------------------------------------------
// private functions
//------------------------------------------------------------------------
void config_uart(void) {
    // put EUSCI register in dormant state during config to save power
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;       // reset EUSCI register during config
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_DORM;        // dormant state

    // config
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK; // select baud clock source as SMCLK (for now)
    EUSCI_A0->CTLW0 &= EUSCI_A_CTLW0_MODE_0;      // UART mode (without auto baud rate)
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SEVENBIT;   // 8 bit word length
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_MSB;        // LSB sent first
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SPB;        // one stop bit
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_PEN;        // parity disable
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SYNC;       // synchronous mode disabled

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_BRKIE;       // enable interrupts (UCRXIFG) for break chars
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_RXEIE;      // reject all erroneous chars

    EUSCI_A0->MCTLW |= EUSCI_A_MCTLW_OS16;        // enable oversampling mode

    EUSCI_A0->BRW = BAUD_RATE;                    // set baud rate
    EUSCI_A0->CTLW1 &= EUSCI_A_CTLW1_GLIT_0;      // minimum deglitch time (~5 ns)
    EUSCI_A0->ABCTL &= ~EUSCI_A_ABCTL_ABDEN;      // disable auto baud rate detection
    EUSCI_A0->ABCTL |= EUSCI_A_ABCTL_DELIM_1;     // set break/synch delimiter length to 2

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;      // enable UART
}

void config_uart_interrupt(void) {
    // enable receive interrupts in IE register
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;

    NVIC_EnableIRQ(EUSCIA0_IRQn);       // enable interrupts in NVIC
}

void EUSCIA0_IRQHandler(void) {
    // check each flag, then perform necessary function
    if (EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG) {

        EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_TXBRK;

        // store RXBUF bits, will empty buffer
        uint8_t rx = EUSCI_A0->RXBUF & EUSCI_A_RXBUF_RXBUF_MASK;
        *uart_payload_struct.data = rx;

        EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG; // clear flag

    } else if (!(EUSCI_A0->RXBUF) && !(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)) {
        // if RXBUF is empty and RX interrupt flag is cleared, start transmission of
        // next word
        EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_TXBRK;
    }
}

// starts UART communication
void start_uart(void) {
    // ensure that any previous transmission has stopped
    while(EUSCI_A0->STATW & EUSCI_A_STATW_BUSY);

    // by reading RXBUF, we reset it before receiving first bit
    uint8_t err = EUSCI_A0->RXBUF & EUSCI_A_RXBUF_RXBUF_MASK;
    // ensure that RXBUF is cleared
    assert(EUSCI_A0->RXBUF == (EUSCI_A0->RXBUF & err));

    // bring EUSCI out of dormant state (if currently dormant)
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_DORM;

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_TXBRK;


}
