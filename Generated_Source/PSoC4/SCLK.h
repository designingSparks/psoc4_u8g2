/*******************************************************************************
* File Name: SCLK.h  
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

#if !defined(CY_PINS_SCLK_H) /* Pins SCLK_H */
#define CY_PINS_SCLK_H

#include "cytypes.h"
#include "cyfitter.h"
#include "SCLK_aliases.h"


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
} SCLK_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   SCLK_Read(void);
void    SCLK_Write(uint8 value);
uint8   SCLK_ReadDataReg(void);
#if defined(SCLK__PC) || (CY_PSOC4_4200L) 
    void    SCLK_SetDriveMode(uint8 mode);
#endif
void    SCLK_SetInterruptMode(uint16 position, uint16 mode);
uint8   SCLK_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void SCLK_Sleep(void); 
void SCLK_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(SCLK__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define SCLK_DRIVE_MODE_BITS        (3)
    #define SCLK_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - SCLK_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the SCLK_SetDriveMode() function.
         *  @{
         */
        #define SCLK_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define SCLK_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define SCLK_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define SCLK_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define SCLK_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define SCLK_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define SCLK_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define SCLK_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define SCLK_MASK               SCLK__MASK
#define SCLK_SHIFT              SCLK__SHIFT
#define SCLK_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in SCLK_SetInterruptMode() function.
     *  @{
     */
        #define SCLK_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define SCLK_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define SCLK_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define SCLK_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(SCLK__SIO)
    #define SCLK_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(SCLK__PC) && (CY_PSOC4_4200L)
    #define SCLK_USBIO_ENABLE               ((uint32)0x80000000u)
    #define SCLK_USBIO_DISABLE              ((uint32)(~SCLK_USBIO_ENABLE))
    #define SCLK_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define SCLK_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define SCLK_USBIO_ENTER_SLEEP          ((uint32)((1u << SCLK_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << SCLK_USBIO_SUSPEND_DEL_SHIFT)))
    #define SCLK_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << SCLK_USBIO_SUSPEND_SHIFT)))
    #define SCLK_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << SCLK_USBIO_SUSPEND_DEL_SHIFT)))
    #define SCLK_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(SCLK__PC)
    /* Port Configuration */
    #define SCLK_PC                 (* (reg32 *) SCLK__PC)
#endif
/* Pin State */
#define SCLK_PS                     (* (reg32 *) SCLK__PS)
/* Data Register */
#define SCLK_DR                     (* (reg32 *) SCLK__DR)
/* Input Buffer Disable Override */
#define SCLK_INP_DIS                (* (reg32 *) SCLK__PC2)

/* Interrupt configuration Registers */
#define SCLK_INTCFG                 (* (reg32 *) SCLK__INTCFG)
#define SCLK_INTSTAT                (* (reg32 *) SCLK__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define SCLK_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(SCLK__SIO)
    #define SCLK_SIO_REG            (* (reg32 *) SCLK__SIO)
#endif /* (SCLK__SIO_CFG) */

/* USBIO registers */
#if !defined(SCLK__PC) && (CY_PSOC4_4200L)
    #define SCLK_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define SCLK_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define SCLK_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define SCLK_DRIVE_MODE_SHIFT       (0x00u)
#define SCLK_DRIVE_MODE_MASK        (0x07u << SCLK_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins SCLK_H */


/* [] END OF FILE */
