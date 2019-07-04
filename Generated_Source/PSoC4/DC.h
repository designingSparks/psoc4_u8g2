/*******************************************************************************
* File Name: DC.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_DC_H) /* Pins DC_H */
#define CY_PINS_DC_H

#include "cytypes.h"
#include "cyfitter.h"
#include "DC_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} DC_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   DC_Read(void);
void    DC_Write(uint8 value);
uint8   DC_ReadDataReg(void);
#if defined(DC__PC) || (CY_PSOC4_4200L) 
    void    DC_SetDriveMode(uint8 mode);
#endif
void    DC_SetInterruptMode(uint16 position, uint16 mode);
uint8   DC_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void DC_Sleep(void); 
void DC_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(DC__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define DC_DRIVE_MODE_BITS        (3)
    #define DC_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - DC_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the DC_SetDriveMode() function.
         *  @{
         */
        #define DC_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define DC_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define DC_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define DC_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define DC_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define DC_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define DC_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define DC_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define DC_MASK               DC__MASK
#define DC_SHIFT              DC__SHIFT
#define DC_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in DC_SetInterruptMode() function.
     *  @{
     */
        #define DC_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define DC_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define DC_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define DC_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(DC__SIO)
    #define DC_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(DC__PC) && (CY_PSOC4_4200L)
    #define DC_USBIO_ENABLE               ((uint32)0x80000000u)
    #define DC_USBIO_DISABLE              ((uint32)(~DC_USBIO_ENABLE))
    #define DC_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define DC_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define DC_USBIO_ENTER_SLEEP          ((uint32)((1u << DC_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << DC_USBIO_SUSPEND_DEL_SHIFT)))
    #define DC_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << DC_USBIO_SUSPEND_SHIFT)))
    #define DC_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << DC_USBIO_SUSPEND_DEL_SHIFT)))
    #define DC_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(DC__PC)
    /* Port Configuration */
    #define DC_PC                 (* (reg32 *) DC__PC)
#endif
/* Pin State */
#define DC_PS                     (* (reg32 *) DC__PS)
/* Data Register */
#define DC_DR                     (* (reg32 *) DC__DR)
/* Input Buffer Disable Override */
#define DC_INP_DIS                (* (reg32 *) DC__PC2)

/* Interrupt configuration Registers */
#define DC_INTCFG                 (* (reg32 *) DC__INTCFG)
#define DC_INTSTAT                (* (reg32 *) DC__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define DC_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(DC__SIO)
    #define DC_SIO_REG            (* (reg32 *) DC__SIO)
#endif /* (DC__SIO_CFG) */

/* USBIO registers */
#if !defined(DC__PC) && (CY_PSOC4_4200L)
    #define DC_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define DC_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define DC_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define DC_DRIVE_MODE_SHIFT       (0x00u)
#define DC_DRIVE_MODE_MASK        (0x07u << DC_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins DC_H */


/* [] END OF FILE */
