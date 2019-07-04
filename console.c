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
#define LEN_CMDBUF 16

char cmdBuffer[LEN_CMDBUF];
//char intBuffer[14];
volatile uint8_t cmdReceived = 0;  //set to true when a command 
int32_t int_val;
char* param;
char* str_val;

/*
 * This callback is registered with the debugUart.c module.
 * When a complete command has been received in the UART buffer, uBuff, 
 * this callback function is invoked. uBuff is copied into cmdBuffer, which 
 * is processed in processCommand().
 */
void cmdCallback(char* uBuff)
{
  memcpy(cmdBuffer, uBuff, 9);
  //memcpy(intBuffer, &uBuff[2], 7);
  cmdReceived = 1;
}

/*
 * Called regularly from the loop in main.c. It processes any commands in cmdBuffer.
 */
void processCommand(void)
{
  CmdType cmd;
  ParamID fn;
  DataType dformat;
  ErrType err = 0;
  
  if (cmdReceived)
  {
    cmd = cmdBuffer[0];
    fn = cmdBuffer[1];
    dformat = cmdBuffer[2];
    param = &cmdBuffer[3];
    
    //Error checking
    //Parameter must be supplied if dformat > DFORMAT_NONE
    if (dformat > DFORMAT_NONE && *param == '\0')
    {
      err = ERR_NOARG;
    }
    else if (dformat == DFORMAT_INT32)
    {
      if (cmdBuffer[3] == '-' && cmdBuffer[4] == '\0')
        err = ERR_INT_INVALID;
      else if (is_integer(param))
        int_val = atoi(param);
      else
        err = ERR_INT_INVALID;
    }    
    else if (dformat == DFORMAT_STR)
    {
      str_val = param;
    }
    
    if (err)
    {
      dbg_printf("Error code: %d\n", err);
    }
    else //process commands
    {
      dbg_printf("Processing CMD\n");
      if (cmd == CMD_READ && fn == PARAM_YK)
      {
        dbg_printf("Read YK:\n");
      }
      else if (cmd == CMD_WRITE && fn == PARAM_YK) 
      {
        dbg_printf("Write YK:\n");
      }
      else if (cmd == CMD_TEST) //'2'
      {
        if (dformat == DFORMAT_INT32)
          dbg_printf("Int: %d\n", int_val);
        else if (dformat == DFORMAT_STR)
          dbg_printf("Str: %s\n", str_val);
        //dbg_printf("Int=%d  String=%s\n",1,"asdf");
        //dbg_printf("%02d:%02d:%02d", 10, 10, 5);
      }
    }//end if !err
    
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
