/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include "debugUart.h"

CY_ISR_PROTO(uartRxISR);

#define BUFFERSIZE	15 //last byte is '\r' to indicate EOC
#define ERRORMSG "Barrrm!\r"

//Add an extra byte to allow uartBuffer to be null terminated
//Otherwise UART_1_UartPutString doesn't work properly
char uartBuffer[BUFFERSIZE + 1]; 
uint32_t bufferPos = 0;
uint32_t dataReady = 0;
cmdReceived_callback fn_cmdReceived; //callback function


/*************************************************************************************************/
/*! \brief  Initializes UART and sets ISR
            
\param   -
\return  -
\callgraph -
 **************************************************************************************************/
void uartInit(cmdReceived_callback cb)
{
  fn_cmdReceived = cb;
  UART_1_Start();
  UART_RX_ISR_StartEx(uartRxISR); //Use my custom ISR handler
}


/*
 * ISR handler for when a character is received.
 * Important! The interrupt must be cleared otherwise this ISR will be repeatedly called.
*/
CY_ISR(uartRxISR)
{
  uint8_t ch;
  uint32 source = 0;
  static uint8_t overrun = 0;
  
  if (overrun) //receive characters until EOC received
  {
    ch = UART_1_UartGetChar();
    if (ch == EOC)
    {
      overrun = 0;
      memset(uartBuffer, 0, BUFFERSIZE); //Clean the buffer!
		  bufferPos = 0;
    }
  }
  else if (bufferPos < BUFFERSIZE)
  {
    ch = UART_1_UartGetChar();
    if (ch == EOC)
    {
      
      //UART_1_UartPutString(uartBuffer);  //Debug
      //cmdCallback(&uartBuffer[0]); //below works the same
      fn_cmdReceived(uartBuffer); 
      dataReady = 1;
      memset(uartBuffer, 0, BUFFERSIZE); //Clean the buffer!
		  bufferPos = 0;
    }
    else
      uartBuffer[bufferPos++] = ch;
  }
  else //buffer overrun
  {
    UART_1_UartPutString(ERRORMSG);
    overrun = 1;
  }
  //Clear the interrupt
  source = UART_1_GetRxInterruptSourceMasked();
  UART_1_ClearRxInterruptSource(source);
}


/* [] END OF FILE */
