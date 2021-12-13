/*
 * Copyright (c) 2016-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== main_freertos.c ========
 */


#include <main.h>


/*
 *  ======== main ========
 */
int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;    // stop watchdog timer



    i2c_init();
    opt3001_init();
    serial_debug_init();
    ece353_ADC14_PS2_XY();
    __enable_irq();




    printf("\n\r");
    printf("*********************************************\n\r");
    printf("* Final Proj\n\r");
    printf("*********************************************\n\r");
    printf("\n\r");

    Queue_Game = xQueueCreate(10,sizeof(uint32_t));

    xTaskCreate
    (   Task_Game_Timer,
        "Task_Game_Timer",
        configMINIMAL_STACK_SIZE,
        NULL,
        1,
        &Task_Game_Timer_Handle
    );

    xTaskCreate
    (   Task_Game,
        "Task_Game",
        configMINIMAL_STACK_SIZE,
        NULL,
        2,
        &Task_Game_Handle
    );

    xTaskCreate
    (   Task_Joystick_Timer,
        "Task_Joystick_Timer",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        &Task_Joystick_Timer_Handle
    );

    xTaskCreate
    (   Task_Joystick_Bottom_Half,
        "Task_Joystick",
        configMINIMAL_STACK_SIZE,
        NULL,
        4,
        &Task_Joystick_Bottom_Half_Handle
    );


        /* Start the FreeRTOS scheduler */
        vTaskStartScheduler();
    while(1){
        /* unsigned long int lux = opt3001_read_lux();
         char buffer[50];
         ltoa(lux,buffer, 10);
         printf("Light Level: ");
         printf(buffer);
         printf("\n\r");*/
    };
    return (0);
}


//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
    /* Handle Memory Allocation Errors */
    while(1)
    {
    }
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    while(1)
    {
    }
}
