/*******************************************************************************
* File Name: .h
* Version 2.50
*
* Description:
*  This private header file contains internal definitions for the SPIM
*  component. Do not use these definitions directly in your application.
*
* Note:
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SPIM_PVT_SPIMaster_H)
#define CY_SPIM_PVT_SPIMaster_H

#include "SPIMaster.h"


/**********************************
*   Functions with external linkage
**********************************/


/**********************************
*   Variables with external linkage
**********************************/

extern volatile uint8 SPIMaster_swStatusTx;
extern volatile uint8 SPIMaster_swStatusRx;

#if(SPIMaster_TX_SOFTWARE_BUF_ENABLED)
    extern volatile uint8 SPIMaster_txBuffer[SPIMaster_TX_BUFFER_SIZE];
    extern volatile uint8 SPIMaster_txBufferRead;
    extern volatile uint8 SPIMaster_txBufferWrite;
    extern volatile uint8 SPIMaster_txBufferFull;
#endif /* (SPIMaster_TX_SOFTWARE_BUF_ENABLED) */

#if(SPIMaster_RX_SOFTWARE_BUF_ENABLED)
    extern volatile uint8 SPIMaster_rxBuffer[SPIMaster_RX_BUFFER_SIZE];
    extern volatile uint8 SPIMaster_rxBufferRead;
    extern volatile uint8 SPIMaster_rxBufferWrite;
    extern volatile uint8 SPIMaster_rxBufferFull;
#endif /* (SPIMaster_RX_SOFTWARE_BUF_ENABLED) */

#endif /* CY_SPIM_PVT_SPIMaster_H */


/* [] END OF FILE */
