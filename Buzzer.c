/*
 * Buzzer.c
 *
 *  Created on: Dec 13, 2021
 *      Author: sstaggs
 */
#include "Buzzer.h"

void init_buzzer(uint16_t ticks_period)
{
    // Set pin mode (J4.40) -> (P2.7 SEL0)
    P2->DIR |= BIT7;
    P2->SEL0 |= BIT7;
    P2->SEL1 &= ~BIT7;
    // Turn off the timer peripheral
    TIMER_A0->CTL = 0;
    // Set the period for TimerA
    TIMER_A0->CCR[0] = ticks_period - 1;
    // Configure duty cycle
    TIMER_A0->CCR[4] = (ticks_period / 256) - 1;
    // Sets the SET/RESET mode
    TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;
    // Turn the first TimerA peripheral.  Use SMCLK as the clock source
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK;
}

void Buzzer_On(void)
{
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;

    TIMER_A0->CTL |= TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;
}

void Buzzer_Off(void)
{
    // Turn off the timer
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;
}

void changeTone(uint16_t ticks_period)
{
    // Turn off the timer peripheral
    TIMER_A0->CTL = 0;
    Buzzer_Off();
    // Set the period for TimerA
    TIMER_A0->CCR[0] = ticks_period - 1;
    // Configure duty cycle
    TIMER_A0->CCR[4] = (ticks_period / 256) - 1;
    // Turn the first TimerA peripheral.  Use SMCLK as the clock source
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK;
}

bool Buzzer_Status(void)
{
    if( (TIMER_A0->CTL & TIMER_A_CTL_MC_MASK) == TIMER_A_CTL_MC__STOP)
    {
        return false;
    }
    else
    {
        return true;
    }
}
