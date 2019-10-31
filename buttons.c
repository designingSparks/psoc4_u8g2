/* ========================================
 *
 * Inspired from http://www.kennethkuhn.com/electronics/debounce.c
 *
 * ========================================
*/
#include "buttons.h"

#define SAMPLE_TIME 5 //ms
#define DEBOUNCE_TIME	50 //ms
#define MAX_COUNT	DEBOUNCE_TIME/SAMPLE_TIME

#define SW1_STATE (uint8)((SW1_PS & SW1_MASK) >> SW1_SHIFT)
#define SW2_STATE (uint8)((SW2_PS & SW2_MASK) >> SW2_SHIFT)
#define SW3_STATE (uint8)((SW3_PS & SW3_MASK) >> SW3_SHIFT)

//unsigned int input;       /* 0 or 1 depending on the input signal */
uint8_t count1, count2, count3;  /* Will range from 0 to the specified MAXIMUM */
uint8_t out1, out2, out3;      /* Cleaned-up version of the input signal */


//Polled every SAMPLE_TIME ms from main()
void debounceButtons(void)
{
  //Debounce SW1
  if (SW1_STATE == 1) //sw inactive
  {
    if (count1 > 0) 
      count1--;
  }
  else if (count1 < MAX_COUNT)
    count1++;
  
  if (count1 == 0)
    out1 = 0;
  else if (count1 >= MAX_COUNT)
  {
    out1 = 1;
    count1 = MAX_COUNT;  //theoretically not needed
  }
  
  //Debounce SW2
  if (SW2_STATE == 1) //sw inactive
  {
    if (count2 > 0) 
      count2--;
  }
  else if (count2 < MAX_COUNT)
    count2++;
  
  if (count2 == 0)
    out2 = 0;
  else if (count2 >= MAX_COUNT)
  {
    out2 = 1;
    count2 = MAX_COUNT;  //theoretically not needed
  }
  
  //Debounce SW3
  if (SW3_STATE == 1) //sw inactive
  {
    if (count3 > 0) 
      count3--;
  }
  else if (count3 < MAX_COUNT)
    count3++;
  
  if (count3 == 0)
    out3 = 0;
  else if (count3 >= MAX_COUNT)
  {
    out3 = 1;
    count3 = MAX_COUNT;  //theoretically not needed
  }
  
} //processButtons  


void updateOutputs(void)
{
  
  uint32_t temp;
  
  //Update based on output
  /*if (out1 == 1)
    PWM_Buzzer_Start();
  else
    PWM_Buzzer_Stop();
  */
    
  if (out2) {
    temp = PWM_Buzzer_ReadPeriod();
    temp = temp - 100;
    if (temp < 6000)
      temp = 6000;
    
    
    /*
    PWM_Buzzer_Stop();
    PWM_Buzzer_WritePeriod(temp);
    PWM_Buzzer_WriteCompare(temp/2);
    PWM_Buzzer_Start();
    */
    
    PWM_Buzzer_WritePeriodBuf(temp);
    PWM_Buzzer_WriteCompareBuf(temp/2);
    
  }
    
  
  if (out3) {
    temp = PWM_Buzzer_ReadPeriod();
    temp = temp + 100;
    if (temp > 24000)
      temp = 24000;
    PWM_Buzzer_WritePeriodBuf(temp);
    PWM_Buzzer_WriteCompareBuf(temp/2);
  }
  
}


/* [] END OF FILE */
