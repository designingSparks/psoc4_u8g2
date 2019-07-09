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
#include "project.h"
#include "oled.h"
#include "debugUart.h"
#include "console.h"


int main(void)
{
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    IDAC_1_Start();
    PWM_1_Start();
    SPIMaster_Start();
    uartInit(cmdCallback); //Register callback function console.c
    init_Oled();
    
    uint8_t i;
    //Variables for emulated EEPROM example
    static const uint8 CYCODE eepromArray[]=
                                { 0x46, 0x69, 0x72, 0x73, 0x74, 0x20, 0x72, 0x75, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x00 };
                                             /* F     i     r     s     t           r     u     n     */
    uint8 array[]              ={ 0x50, 0x6F, 0x77, 0x65, 0x72, 0x43, 0x79, 0x63, 0x6C, 0x65, 0x23, 0x20, 0x30, 0x00 };
                                /* P     o     w     e     r     C     y     c     l     e     #           0*/
    
    

    
    
    //UART testing
    UART_1_UartPutString("UART Initialised\n");
    UART_1_UartPutString("Hello world\n");
    UART_1_UartPutString("John Schon\n");
    
    for(i = 0;; i++)
    {
        /* Place your application code here. */
      //SPIMaster_WriteTxData(i);
      //UART_1_UartPutString("Hello world\n");
      processCommand();
      CyDelay(200);
    }
}


/* [] END OF FILE */
