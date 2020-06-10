/*******************************************************************************
* File Name: SPIMaster.h
* Version 2.50
*
* Description:
*  Contains the function prototypes, constants and register definition
*  of the SPI Master Component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SPIM_SPIMaster_H)
#define CY_SPIM_SPIMaster_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define SPIMaster_INTERNAL_CLOCK             (1u)

#if(0u != SPIMaster_INTERNAL_CLOCK)
    #include "SPIMaster_IntClock.h"
#endif /* (0u != SPIMaster_INTERNAL_CLOCK) */

#define SPIMaster_MODE                       (1u)
#define SPIMaster_DATA_WIDTH                 (8u)
#define SPIMaster_MODE_USE_ZERO              (1u)
#define SPIMaster_BIDIRECTIONAL_MODE         (0u)

/* Internal interrupt handling */
#define SPIMaster_TX_BUFFER_SIZE             (4u)
#define SPIMaster_RX_BUFFER_SIZE             (4u)
#define SPIMaster_INTERNAL_TX_INT_ENABLED    (0u)
#define SPIMaster_INTERNAL_RX_INT_ENABLED    (0u)

#define SPIMaster_SINGLE_REG_SIZE            (8u)
#define SPIMaster_USE_SECOND_DATAPATH        (SPIMaster_DATA_WIDTH > SPIMaster_SINGLE_REG_SIZE)

#define SPIMaster_FIFO_SIZE                  (4u)
#define SPIMaster_TX_SOFTWARE_BUF_ENABLED    ((0u != SPIMaster_INTERNAL_TX_INT_ENABLED) && \
                                                     (SPIMaster_TX_BUFFER_SIZE > SPIMaster_FIFO_SIZE))

#define SPIMaster_RX_SOFTWARE_BUF_ENABLED    ((0u != SPIMaster_INTERNAL_RX_INT_ENABLED) && \
                                                     (SPIMaster_RX_BUFFER_SIZE > SPIMaster_FIFO_SIZE))


/***************************************
*        Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
    uint8 cntrPeriod;
} SPIMaster_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  SPIMaster_Init(void)                           ;
void  SPIMaster_Enable(void)                         ;
void  SPIMaster_Start(void)                          ;
void  SPIMaster_Stop(void)                           ;

void  SPIMaster_EnableTxInt(void)                    ;
void  SPIMaster_EnableRxInt(void)                    ;
void  SPIMaster_DisableTxInt(void)                   ;
void  SPIMaster_DisableRxInt(void)                   ;

void  SPIMaster_Sleep(void)                          ;
void  SPIMaster_Wakeup(void)                         ;
void  SPIMaster_SaveConfig(void)                     ;
void  SPIMaster_RestoreConfig(void)                  ;

void  SPIMaster_SetTxInterruptMode(uint8 intSrc)     ;
void  SPIMaster_SetRxInterruptMode(uint8 intSrc)     ;
uint8 SPIMaster_ReadTxStatus(void)                   ;
uint8 SPIMaster_ReadRxStatus(void)                   ;
void  SPIMaster_WriteTxData(uint8 txData)  \
                                                            ;
uint8 SPIMaster_ReadRxData(void) \
                                                            ;
uint8 SPIMaster_GetRxBufferSize(void)                ;
uint8 SPIMaster_GetTxBufferSize(void)                ;
void  SPIMaster_ClearRxBuffer(void)                  ;
void  SPIMaster_ClearTxBuffer(void)                  ;
void  SPIMaster_ClearFIFO(void)                              ;
void  SPIMaster_PutArray(const uint8 buffer[], uint8 byteCount) \
                                                            ;

#if(0u != SPIMaster_BIDIRECTIONAL_MODE)
    void  SPIMaster_TxEnable(void)                   ;
    void  SPIMaster_TxDisable(void)                  ;
#endif /* (0u != SPIMaster_BIDIRECTIONAL_MODE) */

CY_ISR_PROTO(SPIMaster_TX_ISR);
CY_ISR_PROTO(SPIMaster_RX_ISR);


/***************************************
*   Variable with external linkage
***************************************/

extern uint8 SPIMaster_initVar;


/***************************************
*           API Constants
***************************************/

#define SPIMaster_TX_ISR_NUMBER     ((uint8) (SPIMaster_TxInternalInterrupt__INTC_NUMBER))
#define SPIMaster_RX_ISR_NUMBER     ((uint8) (SPIMaster_RxInternalInterrupt__INTC_NUMBER))

#define SPIMaster_TX_ISR_PRIORITY   ((uint8) (SPIMaster_TxInternalInterrupt__INTC_PRIOR_NUM))
#define SPIMaster_RX_ISR_PRIORITY   ((uint8) (SPIMaster_RxInternalInterrupt__INTC_PRIOR_NUM))


/***************************************
*    Initial Parameter Constants
***************************************/

#define SPIMaster_INT_ON_SPI_DONE    ((uint8) (0u   << SPIMaster_STS_SPI_DONE_SHIFT))
#define SPIMaster_INT_ON_TX_EMPTY    ((uint8) (0u   << SPIMaster_STS_TX_FIFO_EMPTY_SHIFT))
#define SPIMaster_INT_ON_TX_NOT_FULL ((uint8) (0u << \
                                                                           SPIMaster_STS_TX_FIFO_NOT_FULL_SHIFT))
#define SPIMaster_INT_ON_BYTE_COMP   ((uint8) (0u  << SPIMaster_STS_BYTE_COMPLETE_SHIFT))
#define SPIMaster_INT_ON_SPI_IDLE    ((uint8) (0u   << SPIMaster_STS_SPI_IDLE_SHIFT))

/* Disable TX_NOT_FULL if software buffer is used */
#define SPIMaster_INT_ON_TX_NOT_FULL_DEF ((SPIMaster_TX_SOFTWARE_BUF_ENABLED) ? \
                                                                        (0u) : (SPIMaster_INT_ON_TX_NOT_FULL))

/* TX interrupt mask */
#define SPIMaster_TX_INIT_INTERRUPTS_MASK    (SPIMaster_INT_ON_SPI_DONE  | \
                                                     SPIMaster_INT_ON_TX_EMPTY  | \
                                                     SPIMaster_INT_ON_TX_NOT_FULL_DEF | \
                                                     SPIMaster_INT_ON_BYTE_COMP | \
                                                     SPIMaster_INT_ON_SPI_IDLE)

#define SPIMaster_INT_ON_RX_FULL         ((uint8) (0u << \
                                                                          SPIMaster_STS_RX_FIFO_FULL_SHIFT))
#define SPIMaster_INT_ON_RX_NOT_EMPTY    ((uint8) (0u << \
                                                                          SPIMaster_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define SPIMaster_INT_ON_RX_OVER         ((uint8) (0u << \
                                                                          SPIMaster_STS_RX_FIFO_OVERRUN_SHIFT))

/* RX interrupt mask */
#define SPIMaster_RX_INIT_INTERRUPTS_MASK    (SPIMaster_INT_ON_RX_FULL      | \
                                                     SPIMaster_INT_ON_RX_NOT_EMPTY | \
                                                     SPIMaster_INT_ON_RX_OVER)
/* Nubmer of bits to receive/transmit */
#define SPIMaster_BITCTR_INIT            (((uint8) (SPIMaster_DATA_WIDTH << 1u)) - 1u)


/***************************************
*             Registers
***************************************/
#if(CY_PSOC3 || CY_PSOC5)
    #define SPIMaster_TXDATA_REG (* (reg8 *) \
                                                SPIMaster_BSPIM_sR8_Dp_u0__F0_REG)
    #define SPIMaster_TXDATA_PTR (  (reg8 *) \
                                                SPIMaster_BSPIM_sR8_Dp_u0__F0_REG)
    #define SPIMaster_RXDATA_REG (* (reg8 *) \
                                                SPIMaster_BSPIM_sR8_Dp_u0__F1_REG)
    #define SPIMaster_RXDATA_PTR (  (reg8 *) \
                                                SPIMaster_BSPIM_sR8_Dp_u0__F1_REG)
#else   /* PSOC4 */
    #if(SPIMaster_USE_SECOND_DATAPATH)
        #define SPIMaster_TXDATA_REG (* (reg16 *) \
                                          SPIMaster_BSPIM_sR8_Dp_u0__16BIT_F0_REG)
        #define SPIMaster_TXDATA_PTR (  (reg16 *) \
                                          SPIMaster_BSPIM_sR8_Dp_u0__16BIT_F0_REG)
        #define SPIMaster_RXDATA_REG (* (reg16 *) \
                                          SPIMaster_BSPIM_sR8_Dp_u0__16BIT_F1_REG)
        #define SPIMaster_RXDATA_PTR (  (reg16 *) \
                                          SPIMaster_BSPIM_sR8_Dp_u0__16BIT_F1_REG)
    #else
        #define SPIMaster_TXDATA_REG (* (reg8 *) \
                                                SPIMaster_BSPIM_sR8_Dp_u0__F0_REG)
        #define SPIMaster_TXDATA_PTR (  (reg8 *) \
                                                SPIMaster_BSPIM_sR8_Dp_u0__F0_REG)
        #define SPIMaster_RXDATA_REG (* (reg8 *) \
                                                SPIMaster_BSPIM_sR8_Dp_u0__F1_REG)
        #define SPIMaster_RXDATA_PTR (  (reg8 *) \
                                                SPIMaster_BSPIM_sR8_Dp_u0__F1_REG)
    #endif /* (SPIMaster_USE_SECOND_DATAPATH) */
#endif     /* (CY_PSOC3 || CY_PSOC5) */

#define SPIMaster_AUX_CONTROL_DP0_REG (* (reg8 *) \
                                        SPIMaster_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG)
#define SPIMaster_AUX_CONTROL_DP0_PTR (  (reg8 *) \
                                        SPIMaster_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG)

#if(SPIMaster_USE_SECOND_DATAPATH)
    #define SPIMaster_AUX_CONTROL_DP1_REG  (* (reg8 *) \
                                        SPIMaster_BSPIM_sR8_Dp_u1__DP_AUX_CTL_REG)
    #define SPIMaster_AUX_CONTROL_DP1_PTR  (  (reg8 *) \
                                        SPIMaster_BSPIM_sR8_Dp_u1__DP_AUX_CTL_REG)
#endif /* (SPIMaster_USE_SECOND_DATAPATH) */

#define SPIMaster_COUNTER_PERIOD_REG     (* (reg8 *) SPIMaster_BSPIM_BitCounter__PERIOD_REG)
#define SPIMaster_COUNTER_PERIOD_PTR     (  (reg8 *) SPIMaster_BSPIM_BitCounter__PERIOD_REG)
#define SPIMaster_COUNTER_CONTROL_REG    (* (reg8 *) SPIMaster_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)
#define SPIMaster_COUNTER_CONTROL_PTR    (  (reg8 *) SPIMaster_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)

#define SPIMaster_TX_STATUS_REG          (* (reg8 *) SPIMaster_BSPIM_TxStsReg__STATUS_REG)
#define SPIMaster_TX_STATUS_PTR          (  (reg8 *) SPIMaster_BSPIM_TxStsReg__STATUS_REG)
#define SPIMaster_RX_STATUS_REG          (* (reg8 *) SPIMaster_BSPIM_RxStsReg__STATUS_REG)
#define SPIMaster_RX_STATUS_PTR          (  (reg8 *) SPIMaster_BSPIM_RxStsReg__STATUS_REG)

#define SPIMaster_CONTROL_REG            (* (reg8 *) \
                                      SPIMaster_BSPIM_BidirMode_CtrlReg__CONTROL_REG)
#define SPIMaster_CONTROL_PTR            (  (reg8 *) \
                                      SPIMaster_BSPIM_BidirMode_CtrlReg__CONTROL_REG)

#define SPIMaster_TX_STATUS_MASK_REG     (* (reg8 *) SPIMaster_BSPIM_TxStsReg__MASK_REG)
#define SPIMaster_TX_STATUS_MASK_PTR     (  (reg8 *) SPIMaster_BSPIM_TxStsReg__MASK_REG)
#define SPIMaster_RX_STATUS_MASK_REG     (* (reg8 *) SPIMaster_BSPIM_RxStsReg__MASK_REG)
#define SPIMaster_RX_STATUS_MASK_PTR     (  (reg8 *) SPIMaster_BSPIM_RxStsReg__MASK_REG)

#define SPIMaster_TX_STATUS_ACTL_REG     (* (reg8 *) SPIMaster_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define SPIMaster_TX_STATUS_ACTL_PTR     (  (reg8 *) SPIMaster_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define SPIMaster_RX_STATUS_ACTL_REG     (* (reg8 *) SPIMaster_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)
#define SPIMaster_RX_STATUS_ACTL_PTR     (  (reg8 *) SPIMaster_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)

#if(SPIMaster_USE_SECOND_DATAPATH)
    #define SPIMaster_AUX_CONTROLDP1     (SPIMaster_AUX_CONTROL_DP1_REG)
#endif /* (SPIMaster_USE_SECOND_DATAPATH) */


/***************************************
*       Register Constants
***************************************/

/* Status Register Definitions */
#define SPIMaster_STS_SPI_DONE_SHIFT             (0x00u)
#define SPIMaster_STS_TX_FIFO_EMPTY_SHIFT        (0x01u)
#define SPIMaster_STS_TX_FIFO_NOT_FULL_SHIFT     (0x02u)
#define SPIMaster_STS_BYTE_COMPLETE_SHIFT        (0x03u)
#define SPIMaster_STS_SPI_IDLE_SHIFT             (0x04u)
#define SPIMaster_STS_RX_FIFO_FULL_SHIFT         (0x04u)
#define SPIMaster_STS_RX_FIFO_NOT_EMPTY_SHIFT    (0x05u)
#define SPIMaster_STS_RX_FIFO_OVERRUN_SHIFT      (0x06u)

#define SPIMaster_STS_SPI_DONE           ((uint8) (0x01u << SPIMaster_STS_SPI_DONE_SHIFT))
#define SPIMaster_STS_TX_FIFO_EMPTY      ((uint8) (0x01u << SPIMaster_STS_TX_FIFO_EMPTY_SHIFT))
#define SPIMaster_STS_TX_FIFO_NOT_FULL   ((uint8) (0x01u << SPIMaster_STS_TX_FIFO_NOT_FULL_SHIFT))
#define SPIMaster_STS_BYTE_COMPLETE      ((uint8) (0x01u << SPIMaster_STS_BYTE_COMPLETE_SHIFT))
#define SPIMaster_STS_SPI_IDLE           ((uint8) (0x01u << SPIMaster_STS_SPI_IDLE_SHIFT))
#define SPIMaster_STS_RX_FIFO_FULL       ((uint8) (0x01u << SPIMaster_STS_RX_FIFO_FULL_SHIFT))
#define SPIMaster_STS_RX_FIFO_NOT_EMPTY  ((uint8) (0x01u << SPIMaster_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define SPIMaster_STS_RX_FIFO_OVERRUN    ((uint8) (0x01u << SPIMaster_STS_RX_FIFO_OVERRUN_SHIFT))

/* TX and RX masks for clear on read bits */
#define SPIMaster_TX_STS_CLR_ON_RD_BYTES_MASK    (0x09u)
#define SPIMaster_RX_STS_CLR_ON_RD_BYTES_MASK    (0x40u)

/* StatusI Register Interrupt Enable Control Bits */
/* As defined by the Register map for the AUX Control Register */
#define SPIMaster_INT_ENABLE     (0x10u) /* Enable interrupt from statusi */
#define SPIMaster_TX_FIFO_CLR    (0x01u) /* F0 - TX FIFO */
#define SPIMaster_RX_FIFO_CLR    (0x02u) /* F1 - RX FIFO */
#define SPIMaster_FIFO_CLR       (SPIMaster_TX_FIFO_CLR | SPIMaster_RX_FIFO_CLR)

/* Bit Counter (7-bit) Control Register Bit Definitions */
/* As defined by the Register map for the AUX Control Register */
#define SPIMaster_CNTR_ENABLE    (0x20u) /* Enable CNT7 */

/* Bi-Directional mode control bit */
#define SPIMaster_CTRL_TX_SIGNAL_EN  (0x01u)

/* Datapath Auxillary Control Register definitions */
#define SPIMaster_AUX_CTRL_FIFO0_CLR         (0x01u)
#define SPIMaster_AUX_CTRL_FIFO1_CLR         (0x02u)
#define SPIMaster_AUX_CTRL_FIFO0_LVL         (0x04u)
#define SPIMaster_AUX_CTRL_FIFO1_LVL         (0x08u)
#define SPIMaster_STATUS_ACTL_INT_EN_MASK    (0x10u)

/* Component disabled */
#define SPIMaster_DISABLED   (0u)


/***************************************
*       Macros
***************************************/

/* Returns true if componentn enabled */
#define SPIMaster_IS_ENABLED (0u != (SPIMaster_TX_STATUS_ACTL_REG & SPIMaster_INT_ENABLE))

/* Retuns TX status register */
#define SPIMaster_GET_STATUS_TX(swTxSts) ( (uint8)(SPIMaster_TX_STATUS_REG | \
                                                          ((swTxSts) & SPIMaster_TX_STS_CLR_ON_RD_BYTES_MASK)) )
/* Retuns RX status register */
#define SPIMaster_GET_STATUS_RX(swRxSts) ( (uint8)(SPIMaster_RX_STATUS_REG | \
                                                          ((swRxSts) & SPIMaster_RX_STS_CLR_ON_RD_BYTES_MASK)) )


/***************************************
* The following code is DEPRECATED and 
* should not be used in new projects.
***************************************/

#define SPIMaster_WriteByte   SPIMaster_WriteTxData
#define SPIMaster_ReadByte    SPIMaster_ReadRxData
void  SPIMaster_SetInterruptMode(uint8 intSrc)       ;
uint8 SPIMaster_ReadStatus(void)                     ;
void  SPIMaster_EnableInt(void)                      ;
void  SPIMaster_DisableInt(void)                     ;

#define SPIMaster_TXDATA                 (SPIMaster_TXDATA_REG)
#define SPIMaster_RXDATA                 (SPIMaster_RXDATA_REG)
#define SPIMaster_AUX_CONTROLDP0         (SPIMaster_AUX_CONTROL_DP0_REG)
#define SPIMaster_TXBUFFERREAD           (SPIMaster_txBufferRead)
#define SPIMaster_TXBUFFERWRITE          (SPIMaster_txBufferWrite)
#define SPIMaster_RXBUFFERREAD           (SPIMaster_rxBufferRead)
#define SPIMaster_RXBUFFERWRITE          (SPIMaster_rxBufferWrite)

#define SPIMaster_COUNTER_PERIOD         (SPIMaster_COUNTER_PERIOD_REG)
#define SPIMaster_COUNTER_CONTROL        (SPIMaster_COUNTER_CONTROL_REG)
#define SPIMaster_STATUS                 (SPIMaster_TX_STATUS_REG)
#define SPIMaster_CONTROL                (SPIMaster_CONTROL_REG)
#define SPIMaster_STATUS_MASK            (SPIMaster_TX_STATUS_MASK_REG)
#define SPIMaster_STATUS_ACTL            (SPIMaster_TX_STATUS_ACTL_REG)

#define SPIMaster_INIT_INTERRUPTS_MASK  (SPIMaster_INT_ON_SPI_DONE     | \
                                                SPIMaster_INT_ON_TX_EMPTY     | \
                                                SPIMaster_INT_ON_TX_NOT_FULL_DEF  | \
                                                SPIMaster_INT_ON_RX_FULL      | \
                                                SPIMaster_INT_ON_RX_NOT_EMPTY | \
                                                SPIMaster_INT_ON_RX_OVER      | \
                                                SPIMaster_INT_ON_BYTE_COMP)
                                                
#define SPIMaster_DataWidth                  (SPIMaster_DATA_WIDTH)
#define SPIMaster_InternalClockUsed          (SPIMaster_INTERNAL_CLOCK)
#define SPIMaster_InternalTxInterruptEnabled (SPIMaster_INTERNAL_TX_INT_ENABLED)
#define SPIMaster_InternalRxInterruptEnabled (SPIMaster_INTERNAL_RX_INT_ENABLED)
#define SPIMaster_ModeUseZero                (SPIMaster_MODE_USE_ZERO)
#define SPIMaster_BidirectionalMode          (SPIMaster_BIDIRECTIONAL_MODE)
#define SPIMaster_Mode                       (SPIMaster_MODE)
#define SPIMaster_DATAWIDHT                  (SPIMaster_DATA_WIDTH)
#define SPIMaster_InternalInterruptEnabled   (0u)

#define SPIMaster_TXBUFFERSIZE   (SPIMaster_TX_BUFFER_SIZE)
#define SPIMaster_RXBUFFERSIZE   (SPIMaster_RX_BUFFER_SIZE)

#define SPIMaster_TXBUFFER       SPIMaster_txBuffer
#define SPIMaster_RXBUFFER       SPIMaster_rxBuffer

#endif /* (CY_SPIM_SPIMaster_H) */


/* [] END OF FILE */
