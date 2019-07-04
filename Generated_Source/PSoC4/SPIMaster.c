/*******************************************************************************
* File Name: SPIMaster.c
* Version 2.50
*
* Description:
*  This file provides all API functionality of the SPI Master component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPIMaster_PVT.h"

#if(SPIMaster_TX_SOFTWARE_BUF_ENABLED)
    volatile uint8 SPIMaster_txBuffer[SPIMaster_TX_BUFFER_SIZE];
    volatile uint8 SPIMaster_txBufferFull;
    volatile uint8 SPIMaster_txBufferRead;
    volatile uint8 SPIMaster_txBufferWrite;
#endif /* (SPIMaster_TX_SOFTWARE_BUF_ENABLED) */

#if(SPIMaster_RX_SOFTWARE_BUF_ENABLED)
    volatile uint8 SPIMaster_rxBuffer[SPIMaster_RX_BUFFER_SIZE];
    volatile uint8 SPIMaster_rxBufferFull;
    volatile uint8 SPIMaster_rxBufferRead;
    volatile uint8 SPIMaster_rxBufferWrite;
#endif /* (SPIMaster_RX_SOFTWARE_BUF_ENABLED) */

uint8 SPIMaster_initVar = 0u;

volatile uint8 SPIMaster_swStatusTx;
volatile uint8 SPIMaster_swStatusRx;


/*******************************************************************************
* Function Name: SPIMaster_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default SPIM configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  When this function is called it initializes all of the necessary parameters
*  for execution. i.e. setting the initial interrupt mask, configuring the
*  interrupt service routine, configuring the bit-counter parameters and
*  clearing the FIFO and Status Register.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPIMaster_Init(void) 
{
    /* Initialize the Bit counter */
    SPIMaster_COUNTER_PERIOD_REG = SPIMaster_BITCTR_INIT;

    /* Init TX ISR  */
    #if(0u != SPIMaster_INTERNAL_TX_INT_ENABLED)
        CyIntDisable         (SPIMaster_TX_ISR_NUMBER);
        CyIntSetPriority     (SPIMaster_TX_ISR_NUMBER,  SPIMaster_TX_ISR_PRIORITY);
        (void) CyIntSetVector(SPIMaster_TX_ISR_NUMBER, &SPIMaster_TX_ISR);
    #endif /* (0u != SPIMaster_INTERNAL_TX_INT_ENABLED) */

    /* Init RX ISR  */
    #if(0u != SPIMaster_INTERNAL_RX_INT_ENABLED)
        CyIntDisable         (SPIMaster_RX_ISR_NUMBER);
        CyIntSetPriority     (SPIMaster_RX_ISR_NUMBER,  SPIMaster_RX_ISR_PRIORITY);
        (void) CyIntSetVector(SPIMaster_RX_ISR_NUMBER, &SPIMaster_RX_ISR);
    #endif /* (0u != SPIMaster_INTERNAL_RX_INT_ENABLED) */

    /* Clear any stray data from the RX and TX FIFO */
    SPIMaster_ClearFIFO();

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

    (void) SPIMaster_ReadTxStatus(); /* Clear Tx status and swStatusTx */
    (void) SPIMaster_ReadRxStatus(); /* Clear Rx status and swStatusRx */

    /* Configure TX and RX interrupt mask */
    SPIMaster_TX_STATUS_MASK_REG = SPIMaster_TX_INIT_INTERRUPTS_MASK;
    SPIMaster_RX_STATUS_MASK_REG = SPIMaster_RX_INIT_INTERRUPTS_MASK;
}


/*******************************************************************************
* Function Name: SPIMaster_Enable
********************************************************************************
*
* Summary:
*  Enable SPIM component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SPIMaster_Enable(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    SPIMaster_COUNTER_CONTROL_REG |= SPIMaster_CNTR_ENABLE;
    SPIMaster_TX_STATUS_ACTL_REG  |= SPIMaster_INT_ENABLE;
    SPIMaster_RX_STATUS_ACTL_REG  |= SPIMaster_INT_ENABLE;
    CyExitCriticalSection(enableInterrupts);

    #if(0u != SPIMaster_INTERNAL_CLOCK)
        SPIMaster_IntClock_Enable();
    #endif /* (0u != SPIMaster_INTERNAL_CLOCK) */

    SPIMaster_EnableTxInt();
    SPIMaster_EnableRxInt();
}


/*******************************************************************************
* Function Name: SPIMaster_Start
********************************************************************************
*
* Summary:
*  Initialize and Enable the SPI Master component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SPIMaster_initVar - used to check initial configuration, modified on
*  first function call.
*
* Theory:
*  Enable the clock input to enable operation.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPIMaster_Start(void) 
{
    if(0u == SPIMaster_initVar)
    {
        SPIMaster_Init();
        SPIMaster_initVar = 1u;
    }

    SPIMaster_Enable();
}


/*******************************************************************************
* Function Name: SPIMaster_Stop
********************************************************************************
*
* Summary:
*  Disable the SPI Master component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the clock input to enable operation.
*
*******************************************************************************/
void SPIMaster_Stop(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    SPIMaster_TX_STATUS_ACTL_REG &= ((uint8) ~SPIMaster_INT_ENABLE);
    SPIMaster_RX_STATUS_ACTL_REG &= ((uint8) ~SPIMaster_INT_ENABLE);
    CyExitCriticalSection(enableInterrupts);

    #if(0u != SPIMaster_INTERNAL_CLOCK)
        SPIMaster_IntClock_Disable();
    #endif /* (0u != SPIMaster_INTERNAL_CLOCK) */

    SPIMaster_DisableTxInt();
    SPIMaster_DisableRxInt();
}


/*******************************************************************************
* Function Name: SPIMaster_EnableTxInt
********************************************************************************
*
* Summary:
*  Enable internal Tx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal Tx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void SPIMaster_EnableTxInt(void) 
{
    #if(0u != SPIMaster_INTERNAL_TX_INT_ENABLED)
        CyIntEnable(SPIMaster_TX_ISR_NUMBER);
    #endif /* (0u != SPIMaster_INTERNAL_TX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: SPIMaster_EnableRxInt
********************************************************************************
*
* Summary:
*  Enable internal Rx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal Rx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void SPIMaster_EnableRxInt(void) 
{
    #if(0u != SPIMaster_INTERNAL_RX_INT_ENABLED)
        CyIntEnable(SPIMaster_RX_ISR_NUMBER);
    #endif /* (0u != SPIMaster_INTERNAL_RX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: SPIMaster_DisableTxInt
********************************************************************************
*
* Summary:
*  Disable internal Tx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal Tx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void SPIMaster_DisableTxInt(void) 
{
    #if(0u != SPIMaster_INTERNAL_TX_INT_ENABLED)
        CyIntDisable(SPIMaster_TX_ISR_NUMBER);
    #endif /* (0u != SPIMaster_INTERNAL_TX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: SPIMaster_DisableRxInt
********************************************************************************
*
* Summary:
*  Disable internal Rx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal Rx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void SPIMaster_DisableRxInt(void) 
{
    #if(0u != SPIMaster_INTERNAL_RX_INT_ENABLED)
        CyIntDisable(SPIMaster_RX_ISR_NUMBER);
    #endif /* (0u != SPIMaster_INTERNAL_RX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: SPIMaster_SetTxInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void SPIMaster_SetTxInterruptMode(uint8 intSrc) 
{
    SPIMaster_TX_STATUS_MASK_REG = intSrc;
}


/*******************************************************************************
* Function Name: SPIMaster_SetRxInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void SPIMaster_SetRxInterruptMode(uint8 intSrc) 
{
    SPIMaster_RX_STATUS_MASK_REG  = intSrc;
}


/*******************************************************************************
* Function Name: SPIMaster_ReadTxStatus
********************************************************************************
*
* Summary:
*  Read the Tx status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the Tx status register.
*
* Global variables:
*  SPIMaster_swStatusTx - used to store in software status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the Tx status register for error
*  detection and flow control.
*
* Side Effects:
*  Clear Tx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 SPIMaster_ReadTxStatus(void) 
{
    uint8 tmpStatus;

    #if(SPIMaster_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        SPIMaster_DisableTxInt();

        tmpStatus = SPIMaster_GET_STATUS_TX(SPIMaster_swStatusTx);
        SPIMaster_swStatusTx = 0u;

        SPIMaster_EnableTxInt();

    #else

        tmpStatus = SPIMaster_TX_STATUS_REG;

    #endif /* (SPIMaster_TX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: SPIMaster_ReadRxStatus
********************************************************************************
*
* Summary:
*  Read the Rx status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the Rx status register.
*
* Global variables:
*  SPIMaster_swStatusRx - used to store in software Rx status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the Rx status register for error
*  detection and flow control.
*
* Side Effects:
*  Clear Rx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 SPIMaster_ReadRxStatus(void) 
{
    uint8 tmpStatus;

    #if(SPIMaster_RX_SOFTWARE_BUF_ENABLED)
        /* Disable RX interrupt to protect global veriables */
        SPIMaster_DisableRxInt();

        tmpStatus = SPIMaster_GET_STATUS_RX(SPIMaster_swStatusRx);
        SPIMaster_swStatusRx = 0u;

        SPIMaster_EnableRxInt();

    #else

        tmpStatus = SPIMaster_RX_STATUS_REG;

    #endif /* (SPIMaster_RX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: SPIMaster_WriteTxData
********************************************************************************
*
* Summary:
*  Write a byte of data to be sent across the SPI.
*
* Parameters:
*  txDataByte: The data value to send across the SPI.
*
* Return:
*  None.
*
* Global variables:
*  SPIMaster_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer, modified every function
*  call if TX Software Buffer is used.
*  SPIMaster_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer.
*  SPIMaster_txBuffer[SPIMaster_TX_BUFFER_SIZE] - used to store
*  data to sending, modified every function call if TX Software Buffer is used.
*
* Theory:
*  Allows the user to transmit any byte of data in a single transfer.
*
* Side Effects:
*  If this function is called again before the previous byte is finished then
*  the next byte will be appended to the transfer with no time between
*  the byte transfers. Clear Tx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPIMaster_WriteTxData(uint8 txData) 
{
    #if(SPIMaster_TX_SOFTWARE_BUF_ENABLED)

        uint8 tempStatus;
        uint8 tmpTxBufferRead;

        /* Block if TX buffer is FULL: don't overwrite */
        do
        {
            tmpTxBufferRead = SPIMaster_txBufferRead;
            if(0u == tmpTxBufferRead)
            {
                tmpTxBufferRead = (SPIMaster_TX_BUFFER_SIZE - 1u);
            }
            else
            {
                tmpTxBufferRead--;
            }

        }while(tmpTxBufferRead == SPIMaster_txBufferWrite);

        /* Disable TX interrupt to protect global veriables */
        SPIMaster_DisableTxInt();

        tempStatus = SPIMaster_GET_STATUS_TX(SPIMaster_swStatusTx);
        SPIMaster_swStatusTx = tempStatus;


        if((SPIMaster_txBufferRead == SPIMaster_txBufferWrite) &&
           (0u != (SPIMaster_swStatusTx & SPIMaster_STS_TX_FIFO_NOT_FULL)))
        {
            /* Put data element into the TX FIFO */
            CY_SET_REG8(SPIMaster_TXDATA_PTR, txData);
        }
        else
        {
            /* Add to the TX software buffer */
            SPIMaster_txBufferWrite++;
            if(SPIMaster_txBufferWrite >= SPIMaster_TX_BUFFER_SIZE)
            {
                SPIMaster_txBufferWrite = 0u;
            }

            if(SPIMaster_txBufferWrite == SPIMaster_txBufferRead)
            {
                SPIMaster_txBufferRead++;
                if(SPIMaster_txBufferRead >= SPIMaster_TX_BUFFER_SIZE)
                {
                    SPIMaster_txBufferRead = 0u;
                }
                SPIMaster_txBufferFull = 1u;
            }

            SPIMaster_txBuffer[SPIMaster_txBufferWrite] = txData;

            SPIMaster_TX_STATUS_MASK_REG |= SPIMaster_STS_TX_FIFO_NOT_FULL;
        }

        SPIMaster_EnableTxInt();

    #else
        /* Wait until TX FIFO has a place */
        while(0u == (SPIMaster_TX_STATUS_REG & SPIMaster_STS_TX_FIFO_NOT_FULL))
        {
        }

        /* Put data element into the TX FIFO */
        CY_SET_REG8(SPIMaster_TXDATA_PTR, txData);

    #endif /* (SPIMaster_TX_SOFTWARE_BUF_ENABLED) */
}


/*******************************************************************************
* Function Name: SPIMaster_ReadRxData
********************************************************************************
*
* Summary:
*  Read the next byte of data received across the SPI.
*
* Parameters:
*  None.
*
* Return:
*  The next byte of data read from the FIFO.
*
* Global variables:
*  SPIMaster_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer.
*  SPIMaster_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified every function
*  call if RX Software Buffer is used.
*  SPIMaster_rxBuffer[SPIMaster_RX_BUFFER_SIZE] - used to store
*  received data.
*
* Theory:
*  Allows the user to read a byte of data received.
*
* Side Effects:
*  Will return invalid data if the FIFO is empty. The user should Call
*  GetRxBufferSize() and if it returns a non-zero value then it is safe to call
*  ReadByte() function.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 SPIMaster_ReadRxData(void) 
{
    uint8 rxData;

    #if(SPIMaster_RX_SOFTWARE_BUF_ENABLED)

        /* Disable RX interrupt to protect global veriables */
        SPIMaster_DisableRxInt();

        if(SPIMaster_rxBufferRead != SPIMaster_rxBufferWrite)
        {
            if(0u == SPIMaster_rxBufferFull)
            {
                SPIMaster_rxBufferRead++;
                if(SPIMaster_rxBufferRead >= SPIMaster_RX_BUFFER_SIZE)
                {
                    SPIMaster_rxBufferRead = 0u;
                }
            }
            else
            {
                SPIMaster_rxBufferFull = 0u;
            }
        }

        rxData = SPIMaster_rxBuffer[SPIMaster_rxBufferRead];

        SPIMaster_EnableRxInt();

    #else

        rxData = CY_GET_REG8(SPIMaster_RXDATA_PTR);

    #endif /* (SPIMaster_RX_SOFTWARE_BUF_ENABLED) */

    return(rxData);
}


/*******************************************************************************
* Function Name: SPIMaster_GetRxBufferSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes/words of data currently held in the RX buffer.
*  If RX Software Buffer not used then function return 0 if FIFO empty or 1 if
*  FIFO not empty. In another case function return size of RX Software Buffer.
*
* Parameters:
*  None.
*
* Return:
*  Integer count of the number of bytes/words in the RX buffer.
*
* Global variables:
*  SPIMaster_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer.
*  SPIMaster_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
uint8 SPIMaster_GetRxBufferSize(void) 
{
    uint8 size;

    #if(SPIMaster_RX_SOFTWARE_BUF_ENABLED)

        /* Disable RX interrupt to protect global veriables */
        SPIMaster_DisableRxInt();

        if(SPIMaster_rxBufferRead == SPIMaster_rxBufferWrite)
        {
            size = 0u;
        }
        else if(SPIMaster_rxBufferRead < SPIMaster_rxBufferWrite)
        {
            size = (SPIMaster_rxBufferWrite - SPIMaster_rxBufferRead);
        }
        else
        {
            size = (SPIMaster_RX_BUFFER_SIZE - SPIMaster_rxBufferRead) + SPIMaster_rxBufferWrite;
        }

        SPIMaster_EnableRxInt();

    #else

        /* We can only know if there is data in the RX FIFO */
        size = (0u != (SPIMaster_RX_STATUS_REG & SPIMaster_STS_RX_FIFO_NOT_EMPTY)) ? 1u : 0u;

    #endif /* (SPIMaster_TX_SOFTWARE_BUF_ENABLED) */

    return(size);
}


/*******************************************************************************
* Function Name: SPIMaster_GetTxBufferSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes/words of data currently held in the TX buffer.
*  If TX Software Buffer not used then function return 0 - if FIFO empty, 1 - if
*  FIFO not full, 4 - if FIFO full. In another case function return size of TX
*  Software Buffer.
*
* Parameters:
*  None.
*
* Return:
*  Integer count of the number of bytes/words in the TX buffer.
*
* Global variables:
*  SPIMaster_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  SPIMaster_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
uint8  SPIMaster_GetTxBufferSize(void) 
{
    uint8 size;

    #if(SPIMaster_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        SPIMaster_DisableTxInt();

        if(SPIMaster_txBufferRead == SPIMaster_txBufferWrite)
        {
            size = 0u;
        }
        else if(SPIMaster_txBufferRead < SPIMaster_txBufferWrite)
        {
            size = (SPIMaster_txBufferWrite - SPIMaster_txBufferRead);
        }
        else
        {
            size = (SPIMaster_TX_BUFFER_SIZE - SPIMaster_txBufferRead) + SPIMaster_txBufferWrite;
        }

        SPIMaster_EnableTxInt();

    #else

        size = SPIMaster_TX_STATUS_REG;

        if(0u != (size & SPIMaster_STS_TX_FIFO_EMPTY))
        {
            size = 0u;
        }
        else if(0u != (size & SPIMaster_STS_TX_FIFO_NOT_FULL))
        {
            size = 1u;
        }
        else
        {
            size = SPIMaster_FIFO_SIZE;
        }

    #endif /* (SPIMaster_TX_SOFTWARE_BUF_ENABLED) */

    return(size);
}


/*******************************************************************************
* Function Name: SPIMaster_ClearRxBuffer
********************************************************************************
*
* Summary:
*  Clear the RX RAM buffer by setting the read and write pointers both to zero.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SPIMaster_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer, modified every function
*  call - resets to zero.
*  SPIMaster_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified every function call -
*  resets to zero.
*
* Theory:
*  Setting the pointers to zero makes the system believe there is no data to
*  read and writing will resume at address 0 overwriting any data that may have
*  remained in the RAM.
*
* Side Effects:
*  Any received data not read from the RAM buffer will be lost when overwritten.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPIMaster_ClearRxBuffer(void) 
{
    /* Clear Hardware RX FIFO */
    while(0u !=(SPIMaster_RX_STATUS_REG & SPIMaster_STS_RX_FIFO_NOT_EMPTY))
    {
        (void) CY_GET_REG8(SPIMaster_RXDATA_PTR);
    }

    #if(SPIMaster_RX_SOFTWARE_BUF_ENABLED)
        /* Disable RX interrupt to protect global veriables */
        SPIMaster_DisableRxInt();

        SPIMaster_rxBufferFull  = 0u;
        SPIMaster_rxBufferRead  = 0u;
        SPIMaster_rxBufferWrite = 0u;

        SPIMaster_EnableRxInt();
    #endif /* (SPIMaster_RX_SOFTWARE_BUF_ENABLED) */
}


/*******************************************************************************
* Function Name: SPIMaster_ClearTxBuffer
********************************************************************************
*
* Summary:
*  Clear the TX RAM buffer by setting the read and write pointers both to zero.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SPIMaster_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer, modified every function
*  call - resets to zero.
*  SPIMaster_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified every function call -
*  resets to zero.
*
* Theory:
*  Setting the pointers to zero makes the system believe there is no data to
*  read and writing will resume at address 0 overwriting any data that may have
*  remained in the RAM.
*
* Side Effects:
*  Any data not yet transmitted from the RAM buffer will be lost when
*  overwritten.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPIMaster_ClearTxBuffer(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    /* Clear TX FIFO */
    SPIMaster_AUX_CONTROL_DP0_REG |= ((uint8)  SPIMaster_TX_FIFO_CLR);
    SPIMaster_AUX_CONTROL_DP0_REG &= ((uint8) ~SPIMaster_TX_FIFO_CLR);

    #if(SPIMaster_USE_SECOND_DATAPATH)
        /* Clear TX FIFO for 2nd Datapath */
        SPIMaster_AUX_CONTROL_DP1_REG |= ((uint8)  SPIMaster_TX_FIFO_CLR);
        SPIMaster_AUX_CONTROL_DP1_REG &= ((uint8) ~SPIMaster_TX_FIFO_CLR);
    #endif /* (SPIMaster_USE_SECOND_DATAPATH) */
    CyExitCriticalSection(enableInterrupts);

    #if(SPIMaster_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        SPIMaster_DisableTxInt();

        SPIMaster_txBufferFull  = 0u;
        SPIMaster_txBufferRead  = 0u;
        SPIMaster_txBufferWrite = 0u;

        /* Buffer is EMPTY: disable TX FIFO NOT FULL interrupt */
        SPIMaster_TX_STATUS_MASK_REG &= ((uint8) ~SPIMaster_STS_TX_FIFO_NOT_FULL);

        SPIMaster_EnableTxInt();
    #endif /* (SPIMaster_TX_SOFTWARE_BUF_ENABLED) */
}


#if(0u != SPIMaster_BIDIRECTIONAL_MODE)
    /*******************************************************************************
    * Function Name: SPIMaster_TxEnable
    ********************************************************************************
    *
    * Summary:
    *  If the SPI master is configured to use a single bi-directional pin then this
    *  will set the bi-directional pin to transmit.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void SPIMaster_TxEnable(void) 
    {
        SPIMaster_CONTROL_REG |= SPIMaster_CTRL_TX_SIGNAL_EN;
    }


    /*******************************************************************************
    * Function Name: SPIMaster_TxDisable
    ********************************************************************************
    *
    * Summary:
    *  If the SPI master is configured to use a single bi-directional pin then this
    *  will set the bi-directional pin to receive.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void SPIMaster_TxDisable(void) 
    {
        SPIMaster_CONTROL_REG &= ((uint8) ~SPIMaster_CTRL_TX_SIGNAL_EN);
    }

#endif /* (0u != SPIMaster_BIDIRECTIONAL_MODE) */


/*******************************************************************************
* Function Name: SPIMaster_PutArray
********************************************************************************
*
* Summary:
*  Write available data from ROM/RAM to the TX buffer while space is available
*  in the TX buffer. Keep trying until all data is passed to the TX buffer.
*
* Parameters:
*  *buffer: Pointer to the location in RAM containing the data to send
*  byteCount: The number of bytes to move to the transmit buffer.
*
* Return:
*  None.
*
* Side Effects:
*  Will stay in this routine until all data has been sent.  May get locked in
*  this loop if data is not being initiated by the master if there is not
*  enough room in the TX FIFO.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPIMaster_PutArray(const uint8 buffer[], uint8 byteCount)
                                                                          
{
    uint8 bufIndex;

    bufIndex = 0u;

    while(byteCount > 0u)
    {
        SPIMaster_WriteTxData(buffer[bufIndex]);
        bufIndex++;
        byteCount--;
    }
}


/*******************************************************************************
* Function Name: SPIMaster_ClearFIFO
********************************************************************************
*
* Summary:
*  Clear the RX and TX FIFO's of all data for a fresh start.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
void SPIMaster_ClearFIFO(void) 
{
    uint8 enableInterrupts;

    /* Clear Hardware RX FIFO */
    while(0u !=(SPIMaster_RX_STATUS_REG & SPIMaster_STS_RX_FIFO_NOT_EMPTY))
    {
        (void) CY_GET_REG8(SPIMaster_RXDATA_PTR);
    }

    enableInterrupts = CyEnterCriticalSection();
    /* Clear TX FIFO */
    SPIMaster_AUX_CONTROL_DP0_REG |= ((uint8)  SPIMaster_TX_FIFO_CLR);
    SPIMaster_AUX_CONTROL_DP0_REG &= ((uint8) ~SPIMaster_TX_FIFO_CLR);

    #if(SPIMaster_USE_SECOND_DATAPATH)
        /* Clear TX FIFO for 2nd Datapath */
        SPIMaster_AUX_CONTROL_DP1_REG |= ((uint8)  SPIMaster_TX_FIFO_CLR);
        SPIMaster_AUX_CONTROL_DP1_REG &= ((uint8) ~SPIMaster_TX_FIFO_CLR);
    #endif /* (SPIMaster_USE_SECOND_DATAPATH) */
    CyExitCriticalSection(enableInterrupts);
}


/* Following functions are for version Compatibility, they are obsolete.
*  Please do not use it in new projects.
*/


/*******************************************************************************
* Function Name: SPIMaster_EnableInt
********************************************************************************
*
* Summary:
*  Enable internal interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void SPIMaster_EnableInt(void) 
{
    SPIMaster_EnableRxInt();
    SPIMaster_EnableTxInt();
}


/*******************************************************************************
* Function Name: SPIMaster_DisableInt
********************************************************************************
*
* Summary:
*  Disable internal interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void SPIMaster_DisableInt(void) 
{
    SPIMaster_DisableTxInt();
    SPIMaster_DisableRxInt();
}


/*******************************************************************************
* Function Name: SPIMaster_SetInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void SPIMaster_SetInterruptMode(uint8 intSrc) 
{
    SPIMaster_TX_STATUS_MASK_REG  = (intSrc & ((uint8) ~SPIMaster_STS_SPI_IDLE));
    SPIMaster_RX_STATUS_MASK_REG  =  intSrc;
}


/*******************************************************************************
* Function Name: SPIMaster_ReadStatus
********************************************************************************
*
* Summary:
*  Read the status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the status register.
*
* Global variables:
*  SPIMaster_swStatus - used to store in software status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the status register for error detection
*  and flow control.
*
* Side Effects:
*  Clear status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 SPIMaster_ReadStatus(void) 
{
    uint8 tmpStatus;

    #if(SPIMaster_TX_SOFTWARE_BUF_ENABLED || SPIMaster_RX_SOFTWARE_BUF_ENABLED)

        SPIMaster_DisableInt();

        tmpStatus  = SPIMaster_GET_STATUS_RX(SPIMaster_swStatusRx);
        tmpStatus |= SPIMaster_GET_STATUS_TX(SPIMaster_swStatusTx);
        tmpStatus &= ((uint8) ~SPIMaster_STS_SPI_IDLE);

        SPIMaster_swStatusTx = 0u;
        SPIMaster_swStatusRx = 0u;

        SPIMaster_EnableInt();

    #else

        tmpStatus  = SPIMaster_RX_STATUS_REG;
        tmpStatus |= SPIMaster_TX_STATUS_REG;
        tmpStatus &= ((uint8) ~SPIMaster_STS_SPI_IDLE);

    #endif /* (SPIMaster_TX_SOFTWARE_BUF_ENABLED || SPIMaster_RX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/* [] END OF FILE */
