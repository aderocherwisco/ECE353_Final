/*
 * Accelerometer.c
 *
 *  Created on: Dec 12, 2021
 *      Author: Sean
 */
#include "Accelerometer.h"

void init_accel(void)
{
    // configure X axis (J3.23) -> (P6.1 SEL 1)
    P6->SEL0 |= BIT1;
    P6->SEL1 |= BIT1;
    // configure Y axis (J3.24) -> (P4.0 SEL 1)
    P4->SEL0 |= BIT0;
    P4->SEL1 |= BIT0;
    // Configure CTL0 to sample 16-times in pulsed sample mode.
    ADC14->CTL0 = ADC14_CTL0_SHP | ADC14_CTL0_SHT02 | ADC14_CTL0_ENC;
    ADC14->CTL0 = ADC14_CTL0_CONSEQ_1;
    // Configure ADC to return 12-bit values
    ADC14->CTL1 = ADC14_CTL1_RES_2;
    // Associate the X direction analog signal with MEM[0]
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_14;
    // Associate the Y direction analog signal with MEM[1]
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_13;
    // Indicate End-of-Sequence
    ADC14->MCTL[2] = ADC14_MCTLN_EOS;
    // Enable interrupts in the ADC AFTER a value is written into MEM[1].
    ADC14->IER0 = ADC14_IER0_IE1;
    // Enable ADC Interrupt in the NVIC
    NVIC_EnableIRQ(ADC14_IRQn);
    // Turn ADC ON
    ADC14->CTL0 |= ADC14_CTL0_ON;


    /*
    // configure Z axis (J3.25) -> (P4.2 SEL 3)
    P4->SEL0 |= BIT2;
    P4->SEL3 |= BIT2;
    // Associate the Y direction analog signal with MEM[2]
    ADC14->MCTL[2] = ADC14_MCTLN_INCH_11;
    */
}
