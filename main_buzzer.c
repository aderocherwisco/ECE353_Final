#include "msp.h"
#include <stdint.h>
#include <stdbool.h>

#include "Buzzer.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

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
