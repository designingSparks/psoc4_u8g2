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
#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <project.h>
  
#define MAX_FORMAT_BUFFER_SIZE 16
static uint8_t s_formatBuffer[MAX_FORMAT_BUFFER_SIZE];

//Also works.
//#define asprintf(...) sprintf((char *)s_formatBuffer,__VA_ARGS__); UART_1_UartPutString((const char *)s_formatBuffer)


//Function prototype  
void dbg_printf(char const * format, ...);

#endif
  
/* [] END OF FILE */
