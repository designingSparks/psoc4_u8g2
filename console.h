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
#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <project.h>
#include <stdint.h>
#include <stdlib.h>
#include "utilities.h"
  
  
//Function prototypes
void cmdCallback(char* rawVal);
void processCommand(void);
uint8_t is_integer(char *buf);

//https://fresh2refresh.com/c-programming/c-typedef/
/*typedef struct student 
{
  int id;
  char name[20];
  float percentage;
} status;
*/

//Set the initial value to 48 for debugging.
typedef enum  {
    CMD_READ=48, // '0'
    CMD_WRITE,
    CMD_TEST
}CmdType;

typedef enum {
    PARAM_YK=48, // '0'
    PARAM_OUT, 
    PARAM_X
}ParamID;

typedef enum {
    DFORMAT_NONE=48, // '0'
    DFORMAT_INT32, 
    DFORMAT_STR
}DataType;

typedef enum {
    ERR_NONE=0,
    ERR_NOARG,
    ERR_INT_INVALID
}ErrType;

#endif

/* [] END OF FILE */
