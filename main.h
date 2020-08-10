#ifndef _MAIN_H
#define _MAIN_H
    
#include "project.h"
volatile uint16_t* getCount(void);    
void enablePrintCount(uint8_t flag, uint32_t maxCount);
  
#endif