/*******************************************************************************
* File Name: v_EEPROM.h
* Version 2.0
*
* Description:
*  This file provides constants and parameter values for the Emulated EEPROM
*  component.
*
********************************************************************************
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CY_EM_EEPROM_v_EEPROM_H
#define CY_EM_EEPROM_v_EEPROM_H

#include "cyfitter.h"

#if (CYDEV_CHIP_FAMILY_USED != CYDEV_CHIP_FAMILY_PSOC6)
    #include "CyFlash.h"
    #include "cytypes.h"
    #include "cy_em_eeprom.h"
#else
    #include "syslib/cy_syslib.h"
    #include "em_eeprom/cy_em_eeprom.h"
#endif /* (CYDEV_CHIP_FAMILY_USED != CYDEV_CHIP_FAMILY_PSOC6) */

#if defined(__cplusplus)
extern "C" {
#endif


/***************************************
* Conditional Compilation Parameters
***************************************/
#define v_EEPROM_STORAGE_EM_EEPROM          (0U != v_EEPROM_USE_EMULATED_EEEPROM)

#if !defined(CY_PSOC6)
    #define CY_PSOC6                                    (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC6)
#endif /* !defined(CY_PSOC6) */


/***************************************
*        Function Prototypes
***************************************/

cy_en_em_eeprom_status_t v_EEPROM_Init(uint32 startAddress);

/*******************************************************************************
* Function Name: v_EEPROM_Write
****************************************************************************//**
*
* Invokes the Cy_Em_EEPROM_Write() function. In case of PSoC 6 the function 
* is located in Cy_Em_EEPROM middleware library which is part of PDL. In case if
* it is utilized in the design based on non-PSOC 6 device the function is 
* located in internal design wide resource component - Em_EEPROM_Dynamic.
*
*******************************************************************************/
#define v_EEPROM_Write(addr, eepromData, size) \
                                            Cy_Em_EEPROM_Write(addr, eepromData, size, &v_EEPROM_context)


/*******************************************************************************
* Function Name: v_EEPROM_Read
****************************************************************************//**
*
* Invokes the Cy_Em_EEPROM_Read() function. In case of PSoC 6 the function 
* is located in Cy_Em_EEPROM middleware library which is part of PDL. In case if
* it is utilized in the design based on non-PSOC 6 device the function is 
* located in internal design wide resource component - Em_EEPROM_Dynamic.
*
*******************************************************************************/
#define v_EEPROM_Read(addr, eepromData, size) \
                                            Cy_Em_EEPROM_Read(addr, eepromData, size, &v_EEPROM_context)


/*******************************************************************************
* Function Name: v_EEPROM_Erase
****************************************************************************//**
*
* Invokes the Cy_Em_EEPROM_Erase() function. In case of PSoC 6 the function 
* is located in Cy_Em_EEPROM middleware library which is part of PDL. In case if
* it is utilized in the design based on non-PSOC 6 device the function is 
* located in internal design wide resource component - Em_EEPROM_Dynamic.
*
*******************************************************************************/
#define v_EEPROM_Erase()                Cy_Em_EEPROM_Erase(&v_EEPROM_context)


/*******************************************************************************
* Function Name: v_EEPROM_NumWrites
****************************************************************************//**
*
* Invokes the Cy_Em_EEPROM_NumWrites() function. In case of PSoC 6 the function 
* is located in Cy_Em_EEPROM middleware library which is part of PDL. In case if
* it is utilized in the design based on non-PSOC 6 device the function is 
* located in internal design wide resource component - Em_EEPROM_Dynamic.
*
*******************************************************************************/
#define v_EEPROM_NumWrites()            Cy_Em_EEPROM_NumWrites(&v_EEPROM_context)


/***************************************
*   Initial Parameter Constants
****************************************/

/* Defines the location of the EEPROM storage (User flash or Emulated EEPROM flash area) */
#define v_EEPROM_USE_EMULATED_EEEPROM       (1U)
/* Defines if blocking or non-blocking writes will be used for writing flash */
#define v_EEPROM_USE_BLOCKING_WRITE         (0U)
/* The EEPROM size required by the user */
#define v_EEPROM_EEPROM_SIZE                (256U)
/* The actual size of flash used to implement the EEPROM with the
* configuration entered by the user.
*/
#define v_EEPROM_ACTUAL_SIZE                (512U)
#define v_EEPROM_PHYSICAL_SIZE              (v_EEPROM_ACTUAL_SIZE)
/* Holds the value of Wear Leveling option enter by user */
#define v_EEPROM_WEAR_LEVEL_FACTOR          (1U)
/* Holds the value of Redundant Copy option enter by user */
#define v_EEPROM_REDUNDANT_COPY             (0U)


/***************************************
*       Global Variables
***************************************/
extern cy_stc_eeprom_config_t const v_EEPROM_config;
extern cy_stc_eeprom_context_t v_EEPROM_context;
extern const uint8 v_EEPROM_em_EepromStorage[v_EEPROM_ACTUAL_SIZE];

#if defined(__cplusplus)
}
#endif

#endif /* CY_EM_EEPROM_v_EEPROM_H */


/* [] END OF FILE */
