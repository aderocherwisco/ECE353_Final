/*
 * Buzzer.c
 *
 *  Created on: Dec 13, 2021
 *      Author: sstaggs
 */
#include "Buzzer.h"

 TaskHandle_t Task_Buzzer_Handle;

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


void Task_Buzzer(void) {
    uint16_t ticks_period = (uint16_t)(SystemCoreClock) - 1;
    uint16_t old_period = ticks_period;
    init_buzzer(ticks_period);

    // track array location
    int count = 0;
    // track # of loops through main while loop
    int time = 0;

    // array of the notes, macros defined in Buzzer.h
    int tetris [137] = {NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_B4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_D5,NOTE_D5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_C5, NOTE_B4, NOTE_B4, // First Measure
                       NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, 0, NOTE_A4, NOTE_A4, NOTE_C5, NOTE_C5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_C5, NOTE_C5, // Second Measure
                       NOTE_B4, NOTE_B4, NOTE_B4, NOTE_B4, 0, NOTE_B4, 0, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, // Third Measure
                       NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, 0, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, // Fourth Measure
                       NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, 0, NOTE_D5, NOTE_D5, NOTE_F5, NOTE_F5, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_G5, NOTE_G5, NOTE_F5, NOTE_F5, // Fifth Measure
                       NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, 0, NOTE_E5, NOTE_E5, NOTE_C5, NOTE_C5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_C5, NOTE_C5, // Sixth Measure
                       NOTE_B4, NOTE_B4, NOTE_B4, NOTE_B4, 0, NOTE_B4, 0, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, // Seventh Measure
                       NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, 0, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4}; // Eighth Measure

    // Include in the main while loop to keep song playing
    while(1)
    {
        time++;
        // adjust here to speed up or slow down the music
        if((time % (SystemCoreClock/500)) == 0)
        {
            time = 0;
            if(tetris[count] == 0)
            {
                old_period = 0;
                count++;
            }
            ticks_period = (uint16_t)(SystemCoreClock/tetris[count]) - 1;
            if(old_period != ticks_period)
            {
                changeTone(ticks_period);
                old_period = ticks_period;
            }
            count++;
            if(count > 136) count = 0;
        }
        if(Buzzer_Status() == false)
        {
            Buzzer_On();
        }
    }
}
