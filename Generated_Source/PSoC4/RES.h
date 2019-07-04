/*******************************************************************************
* File Name: RES.h  
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

#if !defined(CY_PINS_RES_H) /* Pins RES_H */
#define CY_PINS_RES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "RES_aliases.h"


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
} RES_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   RES_Read(void);
void    RES_Write(uint8 value);
uint8   RES_ReadDataReg(void);
#if defined(RES__PC) || (CY_PSOC4_4200L) 
    void    RES_SetDriveMode(uint8 mode);
#endif
void    RES_SetInterruptMode(uint16 position, uint16 mode);
uint8   RES_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void RES_Sleep(void); 
void RES_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(RES__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define RES_DRIVE_MODE_BITS        (3)
    #define RES_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - RES_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the RES_SetDriveMode() function.
         *  @{
         */
        #define RES_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define RES_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define RES_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define RES_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define RES_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define RES_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define RES_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define RES_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define RES_MASK               RES__MASK
#define RES_SHIFT              RES__SHIFT
#define RES_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in RES_SetInterruptMode() function.
     *  @{
     */
        #define RES_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define RES_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define RES_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define RES_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(RES__SIO)
    #define RES_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(RES__PC) && (CY_PSOC4_4200L)
    #define RES_USBIO_ENABLE               ((uint32)0x80000000u)
    #define RES_USBIO_DISABLE              ((uint32)(~RES_USBIO_ENABLE))
    #define RES_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define RES_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define RES_USBIO_ENTER_SLEEP          ((uint32)((1u << RES_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << RES_USBIO_SUSPEND_DEL_SHIFT)))
    #define RES_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << RES_USBIO_SUSPEND_SHIFT)))
    #define RES_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << RES_USBIO_SUSPEND_DEL_SHIFT)))
    #define RES_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(RES__PC)
    /* Port Configuration */
    #define RES_PC                 (* (reg32 *) RES__PC)
#endif
/* Pin State */
#define RES_PS                     (* (reg32 *) RES__PS)
/* Data Register */
#define RES_DR                     (* (reg32 *) RES__DR)
/* Input Buffer Disable Override */
#define RES_INP_DIS                (* (reg32 *) RES__PC2)

/* Interrupt configuration Registers */
#define RES_INTCFG                 (* (reg32 *) RES__INTCFG)
#define RES_INTSTAT                (* (reg32 *) RES__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define RES_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(RES__SIO)
    #define RES_SIO_REG            (* (reg32 *) RES__SIO)
#endif /* (RES__SIO_CFG) */

/* USBIO registers */
#if !defined(RES__PC) && (CY_PSOC4_4200L)
    #define RES_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define RES_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define RES_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define RES_DRIVE_MODE_SHIFT       (0x00u)
#define RES_DRIVE_MODE_MASK        (0x07u << RES_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins RES_H */


/* [] END OF FILE */
