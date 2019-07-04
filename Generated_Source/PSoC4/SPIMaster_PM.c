/*******************************************************************************
* File Name: SPIMaster_PM.c
* Version 2.50
*
* Description:
*  This file contains the setup, control and status commands to support
*  component operations in low power mode.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPIMaster_PVT.h"

static SPIMaster_BACKUP_STRUCT SPIMaster_backup =
{
    SPIMaster_DISABLED,
    SPIMaster_BITCTR_INIT,
};


/*******************************************************************************
* Function Name: SPIMaster_SaveConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SPIMaster_SaveConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPIMaster_RestoreConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SPIMaster_RestoreConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPIMaster_Sleep
********************************************************************************
*
* Summary:
*  Prepare SPIM Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SPIMaster_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPIMaster_Sleep(void) 
{
    /* Save components enable state */
    SPIMaster_backup.enableState = ((uint8) SPIMaster_IS_ENABLED);

    SPIMaster_Stop();
}


/*******************************************************************************
* Function Name: SPIMaster_Wakeup
********************************************************************************
*
* Summary:
*  Prepare SPIM Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SPIMaster_backup - used when non-retention registers are restored.
*  SPIMaster_txBufferWrite - modified every function call - resets to
*  zero.
*  SPIMaster_txBufferRead - modified every function call - resets to
*  zero.
*  SPIMaster_rxBufferWrite - modified every function call - resets to
*  zero.
*  SPIMaster_rxBufferRead - modified every function call - resets to
*  zero.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPIMaster_Wakeup(void) 
{
    #if(SPIMaster_RX_SOFTWARE_BUF_ENABLED)
        SPIMaster_rxBufferFull  = 0u;
        SPIMaster_rxBufferRead  = 0u;
        SPIMaster_rxBufferWrite = 0u;
    #endif /* (SPIMaster_RX_SOFTWARE_BUF_ENABLED) */

    #if(SPIMaster_TX_SOFTWARE_BUF_ENABLED)
        SPIMaster_txBufferFull  = 0u;
        SPIMaster_txBufferRead  = 0u;
        SPIMaster_txBufferWrite = 0u;
    #endif /* (SPIMaster_TX_SOFTWARE_BUF_ENABLED) */

    /* Clear any data from the RX and TX FIFO */
    SPIMaster_ClearFIFO();

    /* Restore components block enable state */
    if(0u != SPIMaster_backup.enableState)
    {
        SPIMaster_Enable();
    }
}


/* [] END OF FILE */
