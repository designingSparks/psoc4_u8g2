/*******************************************************************************
* File Name: DC.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_DC_ALIASES_H) /* Pins DC_ALIASES_H */
#define CY_PINS_DC_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define DC_0			(DC__0__PC)
#define DC_0_PS		(DC__0__PS)
#define DC_0_PC		(DC__0__PC)
#define DC_0_DR		(DC__0__DR)
#define DC_0_SHIFT	(DC__0__SHIFT)
#define DC_0_INTR	((uint16)((uint16)0x0003u << (DC__0__SHIFT*2u)))

#define DC_INTR_ALL	 ((uint16)(DC_0_INTR))


#endif /* End Pins DC_ALIASES_H */


/* [] END OF FILE */
