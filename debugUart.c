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

#define BUFFERSIZE	32 //last byte is '\r' to indicate EOC
#define ERRORMSG "Barrrm!\r"

//Add an extra byte to allow uartBuffer to be null terminated
//Otherwise UART_1_UartPutString doesn't work properly
char uartBuffer[BUFFERSIZE + 1]; 
uint32_t bufferPos = 0;
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
 
  ch = UART_1_UartGetChar();
  
  if (ch == EOT) //reset input buffer
  {
    bufferPos = 0;
    memset(uartBuffer, 0, BUFFERSIZE);
  }
  else if (ch == EOC && overrun)
  {
    bufferPos = 0;
    overrun = 0;
    memset(uartBuffer, 0, BUFFERSIZE);
    UART_1_UartPutString("Overrun\n");
    //TODO: Send an error code back.
  }
  else if (ch == EOC)
  {
    bufferPos = 0;
    //UART_1_UartPutString(uartBuffer); //debug
    fn_cmdReceived(uartBuffer); 
    memset(uartBuffer, 0, BUFFERSIZE); //Clean the buffer!
  }
  else if (bufferPos < BUFFERSIZE)
  {
    uartBuffer[bufferPos++] = ch;
  }
  else if (bufferPos >= BUFFERSIZE)
  {
    overrun = 1;
  }
  //Clear the interrupt. This is necessary for the PSOC4
  source = UART_1_GetRxInterruptSourceMasked();
  UART_1_ClearRxInterruptSource(source);
}
/* [] END OF FILE */
