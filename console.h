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
#define SEND_ACK 1 //acknowledge bytes sent back for all commands
  
//Function prototypes
void cmdCallback(char* rawVal);
void processCommand(void);
uint8_t is_integer(char *buf);


#endif

/* [] END OF FILE */
