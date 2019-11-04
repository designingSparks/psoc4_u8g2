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
#include "buttons.h"

//Prototypes
//void initialize();
void Lpcomp_DDFT_Out(void);
void taskHandler(void);

int main(void)
{
    
    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    IDAC_1_Start();
    //PWM_1_Start();
    
    //PWM_Buzzer_Start();
    PWM_TON_Start();
	  PWM_T2_Start();
    
    SPIMaster_Start();
    uartInit(cmdCallback); //Register callback function console.c
    init_Oled();
    
    Counter_Start();
    
    LPComp_0_Start();    
    LPComp_0_SetSpeed(LPComp_0_HIGHSPEED);
	  Lpcomp_DDFT_Out(); //Wire comparator output to P2.3
    
    //Apply start pulse to Timer and low side switch.
  	startPWM_Write(1); //reacts to rising edge
    CyDelay(1);
  	startPWM_Write(0); 
  
    PWM_TON_WritePeriod(9600);
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
    
    CySysTickStart();
    CySysTickSetCallback(0, taskHandler);
    CyGlobalIntEnable; /* Enable global interrupts. */
    
   /* for(i = 0;; i++)
    {
        // Place your application code here.
      //SPIMaster_WriteTxData(i);
      //UART_1_UartPutString("Hello world\n");
      //processCommand();
      debounceButtons();
      updateOutputs();
      CyDelay(5);
      
    }*/
}

//Crude task handler based on 1ms sysTick interrupt
void taskHandler(void)
{
  static uint8_t i;
  uint32_t temp;  
  
  if (i < 5)
    i++;
  else
  {
    i = 0;
  
    debounceButtons();
    updateOutputs();
    
    //Test
    //temp = PWM_Buzzer_ReadPeriod();
    //temp = temp + 100;
    //if (temp >= 24000)
     // temp = 6000;
    //PWM_Buzzer_WritePeriod(temp); //Instant update works but has glitches.
    //PWM_Buzzer_WriteCompare(temp/2);
    //PWM_Buzzer_WritePeriodBuf(temp); //Buffered update has no glitches but has beat frequencies
    //PWM_Buzzer_WriteCompareBuf(temp/2);
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
