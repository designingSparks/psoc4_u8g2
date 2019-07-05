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
#ifndef _DEBUG_UART_H
#define _DEBUG_UART_H
#define EOC '\n' //end of command character
#define EOT 0x4  //used to flush buffer
  
typedef void (*cmdReceived_callback) (char *);

//Function prototypes
void uartInit(cmdReceived_callback);
  
  
#endif

/* [] END OF FILE */
