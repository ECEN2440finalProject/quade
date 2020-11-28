/*
 * thermistor.c
 *
 *  Created on: Nov 27, 2020
 *      Author: Ethan C
 * Modified by: Terry Vis
 */

#include "thermistor.h"

volatile uint16_t adcResult;

// Configure ADC14
void adc_config(void) {
    //![Simple ADC14 Configure]
    /* Initializing ADC (MCLK/1/1) */
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);

    /* Configuring ADC Memory (ADC_MEM0 A0/A1) in repeat mode
     * with use of external references */
    ADC14_configureSingleSampleMode(ADC_MEM0, true);
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_EXTPOS_VREFNEG_EXTNEG,
            ADC_INPUT_A0, false);
}

// Configure all pins (LEDs as output, GPIO as input)
void adc_pin_config(void) {
    // set P1.0, P2.0, P2.1, P2.2 as outputs
    P1->DIR |= BIT0;
    P2->DIR |= BIT0 | BIT1 | BIT2;

    // initialize all outputs to low
    P1->OUT &= ~BIT0;
    P2->OUT &= ~BIT0 & ~BIT1 & ~BIT2;

    /* Setting up GPIO pins as analog inputs (and references) */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,
    GPIO_PIN7 | GPIO_PIN6 | GPIO_PIN5 | GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);
}

// Enable interrupts for ADC14
void adc_enable_interrupt(void) {
    /* Enabling sample timer in auto iteration mode and interrupts*/
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
    ADC14_enableInterrupt(ADC_INT0);

    /* Enabling Interrupts */
    Interrupt_enableInterrupt(INT_ADC14);
    Interrupt_enableMaster();
}

void start_adc(void) {
    /* Triggering the start of the sample */
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();
    //![Simple ADC14 Configure]
}

void ADC14_IRQHandler(void)
{
    uint64_t status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);


    if(status & ADC_INT0)
    {
        adcResult = ADC14_getResult(ADC_MEM0);
        P1->OUT &= ~BIT0;
        P2->OUT &= ~BIT0;
        P2->OUT &= ~BIT1;
        P2->OUT &= ~BIT2;
    }

    if (adcResult >= 0x29C1)                            // >40degrees: Red
        P2->OUT |= BIT0;
    else if (adcResult >= 0x26B5 && adcResult < 0x29C1) //35C-40C: Red
        P2->OUT |= BIT0;
    else if (adcResult >= 0x236E && adcResult < 0x26B5) //30C-35C: Yellow
        P2->OUT |= BIT0 | BIT1;
    else if (adcResult >= 0x2000 && adcResult < 0x236E) //25C-30C: Yellow
        P2->OUT |= BIT0 | BIT1;
    else if (adcResult >= 0x1C73 && adcResult < 0x2000) //20C-25C: Green
        P2->OUT |= BIT1;
    else if (adcResult >= 0x18E6 && adcResult < 0x1C73) //15C-20C: Blue-Green
        P2->OUT |= BIT1 | BIT2;
    else if (adcResult >= 0x156E && adcResult < 0x18E6) //10C-15C: Blue-Green
        P2->OUT |= BIT1 | BIT2;
    else if (adcResult >= 0x121D && adcResult < 0x156E) //5C-10C: Blue
        P2->OUT |= BIT2;
    else if (adcResult >= 0x156E && adcResult < 0x18E6) //0C-5C: Blue
        P2->OUT |= BIT2;
    else if (adcResult <= 0x18E6) //0C<: White
        P2->OUT |= BIT0 | BIT1 | BIT2;
    else
        P1->OUT &= ~BIT0;                     // P1.0 = 0
}
