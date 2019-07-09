//https://fresh2refresh.com/c-programming/c-typedef/
/*typedef struct student 
{
  int id;
  char name[20];
  float percentage;
} status;
*/
#ifndef _COMMANDS_H
#define _COMMANDS_H

#define BASE 48 //'0'
  
//Set the initial value to 48 for debugging.
typedef enum  {
    READ=BASE,
    WRITE,
    CALIBRATE
}CmdType;

typedef enum {
    ADC=BASE,
    OLED1,
    OLED2,
    OLED3,
    DACPWM,
    IDAC,
    ADC_DMA,
    DAC1,
    DAC2,
    DAC1_MSD,
    DAC1_LSD,
    PWM_DAC,
    DAC2_MSD
}ParamID;

typedef enum {
    D_NONE=BASE, // '0'
    D_INT32, 
    D_STR
}DataType;

typedef enum {
    ERR_NONE=0,
    ERR_NOARG,
    ERR_INT_INVALID
}ErrType;

#endif

/* [] END OF FILE */
