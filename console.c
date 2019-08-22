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

#include "console.h"
#include "commands.h"
#include "utilities.h"
//#include "u8g2.h"
#include "oled.h"

#define LEN_CMDBUF 32

char cmdBuffer[LEN_CMDBUF];
char ackBuffer[5]; //for acknowledge command
volatile uint8_t cmdReceived = 0;  //set to true when a command 
int32_t int_val;
int16_t result_int; //
char* result_str;
char* value;
char* str_val;
char temp;
uint8_t dac_max = 241; //calibration value
uint8_t pwm_val;

/*
 * This callback is registered with the debugUart.c module.
 * When a complete command has been received in the UART buffer, uBuff, 
 * this callback function is invoked. uBuff is copied into cmdBuffer, which 
 * is processed in processCommand().
 */
void cmdCallback(char* uBuff)
{
  memcpy(cmdBuffer, uBuff, LEN_CMDBUF);
  cmdReceived = 1;
}

/*
 * Called regularly from the loop in main.c. It processes any commands in cmdBuffer.
 */
void processCommand(void)
{
  CmdType cmd;
  ParamID param;
  DataType dformat;
  ErrType err = 0;
  
  if (cmdReceived)
  {
    //UART_1_UartPutString(cmdBuffer); //debug
    cmd = cmdBuffer[0];
    param = cmdBuffer[1];
    dformat = cmdBuffer[2];
    value = &cmdBuffer[3];
      
    //Error checking
    //Parameter must be supplied if dformat > DFORMAT_NONE
    if (dformat > D_NONE && *value == '\0')
    {
      err = ERR_NOARG;
    }
    else if (dformat == D_INT32)
    {
      if (cmdBuffer[3] == '-' && cmdBuffer[4] == '\0')
        err = ERR_INT_INVALID;
      else if (is_integer(value))
        int_val = atoi(value);
      else
        err = ERR_INT_INVALID;
    }    
   
    
    if (err)
    {
      dbg_printf("Error code: %d\n", err);
    }
    else //process commands
    {
      //dbg_printf("Processing CMD\n");
      
      if (cmd == READ && param == ADC) 
      {
        dbg_printf("Reading ADC\n");
        result_int = 99;
        dbg_printf("%d\n", result_int);
      }
      else if (cmd == READ && param == ADC_DMA) //01199 - read 99 samples
      {
        dbg_printf("Reading ADC DMA\n");
      }
      else if (cmd == WRITE && param == DAC1)
      {
        dbg_printf("Write DAC1: %d\n", int_val);
      }
      else if (cmd == WRITE && param == OLED1)
      {
        dbg_printf("Write OLED: %s\n", value);
        write_oled(value);
        //u8g2_DrawStr(&u8g2, 0,20, value);
      }
      else if (cmd == WRITE && param == DACPWM)
      {
        dbg_printf("Write DACPWM: %d\n", int_val);
        //PWM_1_WriteCompare(int_val); //The PWM has double buffered capture/compare
      }
      else if (cmd == WRITE && param == IDAC)
      {
        dbg_printf("Write IDAC: %d\n", int_val);
        IDAC_1_SetValue(int_val); //The PWM has double buffered capture/compare
      }
    }//end if !err
    
    
    if (SEND_ACK) //send acknowledge bytes
    {
      //CyDelayUs(100); //Give pyserial a small delay to allow ack packet to be detected.
      ackBuffer[0] = cmd;
      ackBuffer[1] = param;
      ackBuffer[2] = dformat;
      ackBuffer[3] = '\n';
      ackBuffer[4] = 0x04; //End of transmission
      UART_1_UartPutString(ackBuffer);
    }
    cmdReceived = 0; //Indicate that the cmd has been processed.
  }
}//processCommand


//Return 1 if buf is an integer, i.e. contains all digits.
//The only non-digit permitted is a '-' sign in position 0.
//The param buff is a pointer to a non-null char array.
uint8_t is_integer(char *buf)
{
  uint8_t is_integer = 1;
  uint8_t j=0;
  
  while(j < LEN_CMDBUF - 3){
    if (*buf == '\0')
      break;
    is_integer = isdigit(*buf);
    if (j == 0 && *buf == '-')
      is_integer = 1;
    if (is_integer == 0)
      break;
    j++;
    buf++;
  }
  
  return is_integer;
}
/* [] END OF FILE */
