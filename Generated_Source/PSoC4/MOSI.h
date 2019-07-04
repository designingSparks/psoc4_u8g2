/*******************************************************************************
* File Name: MOSI.h  
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

#if !defined(CY_PINS_MOSI_H) /* Pins MOSI_H */
#define CY_PINS_MOSI_H

#include "cytypes.h"
#include "cyfitter.h"
#include "MOSI_aliases.h"


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
} MOSI_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   MOSI_Read(void);
void    MOSI_Write(uint8 value);
uint8   MOSI_ReadDataReg(void);
#if defined(MOSI__PC) || (CY_PSOC4_4200L) 
    void    MOSI_SetDriveMode(uint8 mode);
#endif
void    MOSI_SetInterruptMode(uint16 position, uint16 mode);
uint8   MOSI_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void MOSI_Sleep(void); 
void MOSI_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(MOSI__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define MOSI_DRIVE_MODE_BITS        (3)
    #define MOSI_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - MOSI_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the MOSI_SetDriveMode() function.
         *  @{
         */
        #define MOSI_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define MOSI_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define MOSI_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define MOSI_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define MOSI_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define MOSI_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define MOSI_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define MOSI_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define MOSI_MASK               MOSI__MASK
#define MOSI_SHIFT              MOSI__SHIFT
#define MOSI_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in MOSI_SetInterruptMode() function.
     *  @{
     */
        #define MOSI_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define MOSI_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define MOSI_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define MOSI_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(MOSI__SIO)
    #define MOSI_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(MOSI__PC) && (CY_PSOC4_4200L)
    #define MOSI_USBIO_ENABLE               ((uint32)0x80000000u)
    #define MOSI_USBIO_DISABLE              ((uint32)(~MOSI_USBIO_ENABLE))
    #define MOSI_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define MOSI_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define MOSI_USBIO_ENTER_SLEEP          ((uint32)((1u << MOSI_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << MOSI_USBIO_SUSPEND_DEL_SHIFT)))
    #define MOSI_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << MOSI_USBIO_SUSPEND_SHIFT)))
    #define MOSI_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << MOSI_USBIO_SUSPEND_DEL_SHIFT)))
    #define MOSI_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(MOSI__PC)
    /* Port Configuration */
    #define MOSI_PC                 (* (reg32 *) MOSI__PC)
#endif
/* Pin State */
#define MOSI_PS                     (* (reg32 *) MOSI__PS)
/* Data Register */
#define MOSI_DR                     (* (reg32 *) MOSI__DR)
/* Input Buffer Disable Override */
#define MOSI_INP_DIS                (* (reg32 *) MOSI__PC2)

/* Interrupt configuration Registers */
#define MOSI_INTCFG                 (* (reg32 *) MOSI__INTCFG)
#define MOSI_INTSTAT                (* (reg32 *) MOSI__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define MOSI_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(MOSI__SIO)
    #define MOSI_SIO_REG            (* (reg32 *) MOSI__SIO)
#endif /* (MOSI__SIO_CFG) */

/* USBIO registers */
#if !defined(MOSI__PC) && (CY_PSOC4_4200L)
    #define MOSI_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define MOSI_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define MOSI_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define MOSI_DRIVE_MODE_SHIFT       (0x00u)
#define MOSI_DRIVE_MODE_MASK        (0x07u << MOSI_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins MOSI_H */


/* [] END OF FILE */
