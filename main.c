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

//Prototypes
//void initialize();
void Lpcomp_DDFT_Out(void);

int main(void)
{
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    IDAC_1_Start();
    PWM_1_Start();
    SPIMaster_Start();
    uartInit(cmdCallback); //Register callback function console.c
    init_Oled();
    
    PWM_TON_Start();
	  PWM_T2_Start();
    Counter_Start();
    
    LPComp_0_Start();    
    LPComp_0_SetSpeed(LPComp_0_HIGHSPEED);
	  Lpcomp_DDFT_Out(); //Wire comparator output to P2.3
    
    //Apply start pulse to Timer and low side switch.
  	startPWM_Write(1); //reacts to rising edge
  	startPWM_Write(0); 
  
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

//Connect comparator outputs to physical pins
void Lpcomp_DDFT_Out(void)
{
	/* clear bit[5:0] for DDFT0 */
    (*(reg32 *)CYREG_TST_DDFT_CTRL) &= ~(0x3F);
    /* select LPCOMP_0 as DDFT0 source, DDFT0 output connect to P2[3] directly in the internal */
    (*(reg32 *)CYREG_TST_DDFT_CTRL) |= (11u);
    
    /* clear bit[13:8] for DDFT1 */
//    (*(reg32 *)CYREG_TST_DDFT_CTRL) &= ~(0x3F << 8);
    /* select LPCOMP_1 as DDFT1 source, DDFT1 output connect to P1[2] directly in the internal */
//    (*(reg32 *)CYREG_TST_DDFT_CTRL) |= (12u) << 8;
    
    /* enable DDFT function */
    (*(reg32 *)CYREG_TST_DDFT_CTRL) |= (1u) << 31;
}


/* [] END OF FILE */
