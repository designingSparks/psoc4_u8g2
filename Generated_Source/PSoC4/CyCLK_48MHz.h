/*******************************************************************************
* File Name: CyCLK_48MHz.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_CyCLK_48MHz_H)
#define CY_CLOCK_CyCLK_48MHz_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void CyCLK_48MHz_StartEx(uint32 alignClkDiv);
#define CyCLK_48MHz_Start() \
    CyCLK_48MHz_StartEx(CyCLK_48MHz__PA_DIV_ID)

#else

void CyCLK_48MHz_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void CyCLK_48MHz_Stop(void);

void CyCLK_48MHz_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 CyCLK_48MHz_GetDividerRegister(void);
uint8  CyCLK_48MHz_GetFractionalDividerRegister(void);

#define CyCLK_48MHz_Enable()                         CyCLK_48MHz_Start()
#define CyCLK_48MHz_Disable()                        CyCLK_48MHz_Stop()
#define CyCLK_48MHz_SetDividerRegister(clkDivider, reset)  \
    CyCLK_48MHz_SetFractionalDividerRegister((clkDivider), 0u)
#define CyCLK_48MHz_SetDivider(clkDivider)           CyCLK_48MHz_SetDividerRegister((clkDivider), 1u)
#define CyCLK_48MHz_SetDividerValue(clkDivider)      CyCLK_48MHz_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define CyCLK_48MHz_DIV_ID     CyCLK_48MHz__DIV_ID

#define CyCLK_48MHz_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define CyCLK_48MHz_CTRL_REG   (*(reg32 *)CyCLK_48MHz__CTRL_REGISTER)
#define CyCLK_48MHz_DIV_REG    (*(reg32 *)CyCLK_48MHz__DIV_REGISTER)

#define CyCLK_48MHz_CMD_DIV_SHIFT          (0u)
#define CyCLK_48MHz_CMD_PA_DIV_SHIFT       (8u)
#define CyCLK_48MHz_CMD_DISABLE_SHIFT      (30u)
#define CyCLK_48MHz_CMD_ENABLE_SHIFT       (31u)

#define CyCLK_48MHz_CMD_DISABLE_MASK       ((uint32)((uint32)1u << CyCLK_48MHz_CMD_DISABLE_SHIFT))
#define CyCLK_48MHz_CMD_ENABLE_MASK        ((uint32)((uint32)1u << CyCLK_48MHz_CMD_ENABLE_SHIFT))

#define CyCLK_48MHz_DIV_FRAC_MASK  (0x000000F8u)
#define CyCLK_48MHz_DIV_FRAC_SHIFT (3u)
#define CyCLK_48MHz_DIV_INT_MASK   (0xFFFFFF00u)
#define CyCLK_48MHz_DIV_INT_SHIFT  (8u)

#else 

#define CyCLK_48MHz_DIV_REG        (*(reg32 *)CyCLK_48MHz__REGISTER)
#define CyCLK_48MHz_ENABLE_REG     CyCLK_48MHz_DIV_REG
#define CyCLK_48MHz_DIV_FRAC_MASK  CyCLK_48MHz__FRAC_MASK
#define CyCLK_48MHz_DIV_FRAC_SHIFT (16u)
#define CyCLK_48MHz_DIV_INT_MASK   CyCLK_48MHz__DIVIDER_MASK
#define CyCLK_48MHz_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_CyCLK_48MHz_H) */

/* [] END OF FILE */
