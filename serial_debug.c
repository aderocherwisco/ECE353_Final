/*
 * serial_debug.c
 *
 *  Created on: Aug 25, 2020
 *      Author: Joe Krachey
 */

#include "serial_debug.h"


Circular_Buffer *Tx_Buffer;

volatile char charArr[80];
volatile uint16_t count = 0;
volatile bool newString = false;

//****************************************************************************
// Helper function used to configure the eUSCI
//****************************************************************************
static void serial_debug_init_uart(void)
{

    P1->SEL0 |= BIT2 | BIT3;
    P1->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;
    EUSCI_A0-> CTLW0 = EUSCI_A_CTLW0_SWRST | EUSCI_B_CTLW0_SSEL__SMCLK;


    EUSCI_A0->BRW = 13;

    EUSCI_A0->MCTLW = (0<<EUSCI_A_MCTLW_BRF_OFS | EUSCI_A_MCTLW_OS16);

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;

    EUSCI_A0->IFG &= ~(EUSCI_A_IFG_RXIFG | EUSCI_A_IFG_TXIFG);

    EUSCI_A0-> IE |= EUSCI_A_IE_RXIE;

    NVIC_EnableIRQ(EUSCIA0_IRQn);
    NVIC_SetPriority(EUSCIA0_IRQn, 3);

    // Prime the pump -- ADD CODE
    EUSCI_A0->TXBUF = 0;

}

//****************************************************************************
// Initializes the EUSCI_A0 peripheral to be a UART with a baud rate of 115200
//
// NOTE: This function assumes that the SMCLK has been configured to run at
// 24MHz.  Please change __SYSTEM_CLOCK in system_msp432p401r.c to the
// correct value of 24000000
// ****************************************************************************/
void serial_debug_init(void)
{
    // Initialize the UART
    serial_debug_init_uart();
    Tx_Buffer = circular_buffer_init(80);
}

//****************************************************************************
// Prints a string to the serial debug UART
// ****************************************************************************/
void serial_debug_put_string(char * s)
{
    while(*s != 0 ){

        while(EUSCI_A0->STATW & EUSCI_A_STATW_BUSY){};

        EUSCI_A0->TXBUF = *s;
        s++;
    }
}

//****************************************************************************
// This function is called from MicroLIB's stdio library.  By implementing
// this function, MicroLIB's putchar(), puts(), printf(), etc will now work.
// ****************************************************************************/
int fputc(int c, FILE* stream)
{
    // Busy wait while the circular buffer is full -- ADD CODE
    while(circular_buffer_full(Tx_Buffer)) {
    }


    // globally disable interrupts
    __disable_irq();

    // add the character to the circular buffer  -- ADD CODE
    circular_buffer_add(Tx_Buffer, c);

    // globally disable interrupts
      __enable_irq();

    // Enable Tx Empty Interrupts  -- ADD CODE
    EUSCI_A0 -> IE |= BIT1;

    return 0;
}

//****************************************************************************
// UART interrupt service routine
// ****************************************************************************/
void EUSCIA0_IRQHandler(void)
{
    if(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG){
        char input = EUSCI_A0->RXBUF;
        charArr[count] = input;
        count ++;

        if(input == 0x0A || input == 0x0D){
            newString = true;
        }
    }


    // Check for Tx Interrupts
   if (EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)
   {
       //Check to see if the Tx circular buffer is empty
       if(circular_buffer_empty(Tx_Buffer))
       {
           // Disable Tx Interrupts -- ADD CODE
           EUSCI_A0 -> IE &= ~BIT1;
       }
       else
       {
           // Get the next char from the circular buffer -- ADD CODE
           char c = circular_buffer_remove(Tx_Buffer);
           // Transmit the character -- ADD CODE
           EUSCI_A0->TXBUF = c;
       }
   }
    //EUSCI_A0->TXBUF = EUSCI_A0->RXBUF;


}

