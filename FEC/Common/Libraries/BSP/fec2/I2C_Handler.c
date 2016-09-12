/*
 I2C_Handler.c
 --------------
 this file will handle the reading and writing to a "Raytek MI3" temperature sensor
 with an I2C communication port.
 */

#include "I2C_Handler.h"
#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

/********************/
/* global veriables */
/********************/
//uint8_t g_I2C1_Rx_ready_flag = DISABLE;
//uint8_t g_I2C1_Tx_ready_flag = DISABLE;

//uint8_t g_I2C_Rx_Process_Flag = DISABLE;
//uint8_t g_I2C_Tx_Process_Flag = DISABLE;

uint8_t g_I2C_State = CONNECT_2_SENSOR;
uint8_t g_MI3_Address;
uint8_t g_MI3_Register;
uint8_t g_I2C_RW_Dirctn;
uint8_t g_I2C1_ErrorReg;

/*****************/
/* global unions */
/*****************/
U_MasterRxMsg g_MasterRxMsg;
U_MasterTxMsg g_MasterTxMsg;

// Semaphore for notifying that the transfer was complete.
xSemaphoreHandle I2CDmaTransferCompleteSemaphore = NULL;

#define M_SEMAPHORE_TIMEOUT 100

/****************************************************/
/* nestet vector interrupt controller init structs */
/****************************************************/

#define I2C_TIMEOUT     10000
#define TIMEOUT_CHECK(x)     x++; if(x > I2C_TIMEOUT){return(0);}



/*****************************/
/* initiate the I2C module  */
/*****************************/
void I2C1_DMA_IRQ_init(void)
{
    g_I2C1_ErrorReg = 0;

    NVIC_InitTypeDef NVIC_I2C_EV_InitStructure;
    NVIC_InitTypeDef NVIC_I2C_ER_InitStructure;
    NVIC_InitTypeDef NVIC_I2C_Rx_DMA_InitStruct;
    NVIC_InitTypeDef NVIC_I2C_Tx_DMA_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef I2C_InitStruct;
    DMA_InitTypeDef DMA_InitStructure;

    // enable I2C1
    I2C_Cmd(I2C1, DISABLE);
    //Enable EVT interrupts
    I2C_ITConfig(I2C1, (I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR ), DISABLE);

    /****************  Initiate peripherial clocks *********************/
    // enable APB1 peripheral clock for I2C1
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    // enable clock for SCL and SDA pins
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    // Configure and enable I2C1 event interrupt

    /****************  Configure the I2C I/O pins ************************/
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8; // we are going to use PB6 and PB8
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;		// set pins to alternate function
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		// set GPIO speed
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;	// set output to open drain --> the line has to be only pulled low, not driven high
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;			// Disable pull up resistors
    GPIO_Init(GPIOB, &GPIO_InitStruct);					// init GPIOB

    // Connect I2C1 pins to AF
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1 );	// SCL
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1 ); // SDA

    /*****************************  Configure the I2C module **********************************/
    // configure I2C1
    I2C_InitStruct.I2C_ClockSpeed = I2C_SPEED; 		// 10kHz
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;		// I2C mode
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;	// 50% duty cycle --> standard
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;		// own address, not relevant in master mode
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;		// Enable acknowledge when reading (can be changed later on)
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses
    I2C_Init(I2C1, &I2C_InitStruct);
    // Slow down the PCLK frequency.
    I2C1->CR2 = (I2C1->CR2 & 0xFFC0) | 11;
    /* Configure I2C Filters */
    I2C_AnalogFilterCmd(I2C1, ENABLE);
    I2C_DigitalFilterConfig(I2C1, 0x0F);
    // enable I2C1
    I2C_Cmd(I2C1, ENABLE);
    //Enable EVT interrupts
    I2C_ITConfig(I2C1, (I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR ), ENABLE);

    /****************  Initiate the I2C NVIC's *********************/
    /* Enable I2C Events Interrupt */
    NVIC_I2C_EV_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
    NVIC_I2C_EV_InitStructure.NVIC_IRQChannelPreemptionPriority = (uint8_t)(configMAX_SYSCALL_INTERRUPT_PRIORITY >> 4) + 3;
    NVIC_I2C_EV_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_I2C_EV_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_I2C_EV_InitStructure);
    /* Enable I2C Errors Interrupt */
    NVIC_I2C_ER_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
    NVIC_I2C_ER_InitStructure.NVIC_IRQChannelPreemptionPriority = (uint8_t)(configMAX_SYSCALL_INTERRUPT_PRIORITY >> 4) + 3;
    NVIC_I2C_ER_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_I2C_ER_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_I2C_ER_InitStructure);
    /* Enable the Rx DMA Interrupt */
    NVIC_I2C_Rx_DMA_InitStruct.NVIC_IRQChannel = DMA1_Stream5_IRQn;
    NVIC_I2C_Rx_DMA_InitStruct.NVIC_IRQChannelPreemptionPriority = (uint8_t)(configMAX_SYSCALL_INTERRUPT_PRIORITY >> 4) + 3;
    NVIC_I2C_Rx_DMA_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_I2C_Rx_DMA_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_I2C_Rx_DMA_InitStruct);
    /* Enable the Tx DMA Interrupt */
    NVIC_I2C_Tx_DMA_InitStruct.NVIC_IRQChannel = DMA1_Stream6_IRQn;
    NVIC_I2C_Tx_DMA_InitStruct.NVIC_IRQChannelPreemptionPriority = (uint8_t)(configMAX_SYSCALL_INTERRUPT_PRIORITY >> 4) + 3;
    NVIC_I2C_Tx_DMA_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_I2C_Tx_DMA_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_I2C_Tx_DMA_InitStruct);

    /****************************** DMA Configuration **********************************************/

    /*--------- DMA module configuration ---------*/
    /* Initialize the DMA_Channel member */
    DMA_InitStructure.DMA_Channel = I2C1_DMA_CHANNEL;
    /* Initialize the DMA_PeripheralBaseAddr member */
    DMA_InitStructure.DMA_PeripheralBaseAddr = I2C1_DR_ADDRESS;
    /* Initialize the DMA_PeripheralInc member */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    /* Initialize the DMA_MemoryInc member */
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    /* Initialize the DMA_PeripheralDataSize member */
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    /* Initialize the DMA_MemoryDataSize member */
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    /* Initialize the DMA_Mode member */
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    /* Initialize the DMA_Priority member */
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    /* Initialize the DMA_FIFOMode member */
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
    /* Initialize the DMA_FIFOThreshold member */
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    /* Initialize the DMA_MemoryBurst member */
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    /* Initialize the DMA_PeripheralBurst member */
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

    /*--------------- Tx DMA --------------- */
    /* Clear any pending flag on Tx Stream  */
    DMA_ClearFlag(I2C1_DMA_STREAM_TX,
            I2C1_TX_DMA_TCFLAG | I2C1_TX_DMA_FEIFLAG | I2C1_TX_DMA_DMEIFLAG | I2C1_TX_DMA_TEIFLAG
                    | I2C1_TX_DMA_HTIFLAG );
    /* Disable the I2C Tx DMA stream */
    DMA_Cmd(I2C1_DMA_STREAM_TX, DISABLE);
    /* Configure the DMA stream for the I2C peripheral TX direction */
    DMA_DeInit(I2C1_DMA_STREAM_TX );
    /* Initialize the DMA_DIR member */
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    /* Initialize the DMA_Memory0BaseAddr member */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) & g_MasterTxMsg.TxPacket[0];
    /* Initialize the DMA_BufferSize member */
    DMA_InitStructure.DMA_BufferSize = NUM_OF_RX_BYTES;
    DMA_DeInit(I2C1_DMA_STREAM_TX );
    DMA_Init(I2C1_DMA_STREAM_TX, &DMA_InitStructure);
    /* Clearing the Rx interrupts  */
    DMA_ClearITPendingBit(I2C1_DMA_STREAM_TX, DMA_IT_TCIF6 );
    /* Anable the Rx Interrupts*/
    DMA_ITConfig(I2C1_DMA_STREAM_TX, DMA_IT_TC, ENABLE);

    /*--------------- Rx DMA --------------- */
    /* Clear any pending flag on Rx Stream  */
    DMA_ClearFlag(I2C1_DMA_STREAM_RX,
            I2C1_RX_DMA_TCFLAG | I2C1_RX_DMA_FEIFLAG | I2C1_RX_DMA_DMEIFLAG | I2C1_RX_DMA_TEIFLAG
                    | I2C1_RX_DMA_HTIFLAG );
    /* Disable the I2C Rx DMA stream */
    DMA_Cmd(I2C1_DMA_STREAM_RX, DISABLE);
    /* Configure the DMA stream for the I2C peripheral RX direction */
    DMA_DeInit(I2C1_DMA_STREAM_RX );
    /* Initialize the DMA_DIR member */
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    /* Initialize the DMA_Memory0BaseAddr member */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) & g_MasterRxMsg.RxPacket[0];
    /* Initialize the DMA_BufferSize member */
    DMA_InitStructure.DMA_BufferSize = NUM_OF_RX_BYTES;
    DMA_DeInit(I2C1_DMA_STREAM_RX );
    DMA_Init(I2C1_DMA_STREAM_RX, &DMA_InitStructure);
    /* Clearing the Rx interrupts  */
    DMA_ClearITPendingBit(I2C1_DMA_STREAM_RX, DMA_IT_TCIF5 );
    /* Anable the Rx Interrupts*/
    DMA_ITConfig(I2C1_DMA_STREAM_RX, DMA_IT_TC, ENABLE);
    /* Enable the Last byte transfer command */
    I2C_DMALastTransferCmd(I2C1, ENABLE);

    if (I2CDmaTransferCompleteSemaphore == NULL)
        vSemaphoreCreateBinary(I2CDmaTransferCompleteSemaphore);
    xSemaphoreTake(I2CDmaTransferCompleteSemaphore, 0);

}

/********************************************************/
/*                      IRQ Handlers                    */
/********************************************************/

void GiveBackTheCompleteSemaphore()
{
    static portBASE_TYPE xHigherPriorityTaskWoken;
    // yield before giving the semaphore, in case the interrupt returned before than the
    // calling task could wait for the semaphore.
//    taskYIELD();

    xSemaphoreGiveFromISR(I2CDmaTransferCompleteSemaphore, &xHigherPriorityTaskWoken);

    // FreeRTOS manual specifies that if a higher priority task is waken, the task should yield.
//    if (xHigherPriorityTaskWoken)
//    {
//        taskYIELD();
//    }
}

/*-------- Rx DMA TC Interrupt --------*/
void DMA1_Stream5_IRQHandler(void)
{
    // Disable DMA RX Channel
    DMA_Cmd(I2C1_DMA_STREAM_RX, DISABLE);
    // Disable I2C DMA request
    I2C_DMACmd(I2C1, DISABLE);
    // Clear any pending flag on Rx Stream
    DMA_ClearFlag(I2C1_DMA_STREAM_RX,
            I2C1_RX_DMA_TCFLAG | I2C1_RX_DMA_FEIFLAG | I2C1_RX_DMA_DMEIFLAG | I2C1_RX_DMA_TEIFLAG
                    | I2C1_RX_DMA_HTIFLAG );
    I2C1 ->CR1 |= I2C_CR1_STOP;
    // Calculate and check the checksum
    Checksum_Calc(&g_MasterRxMsg);
    if (g_MasterRxMsg.RxPacketBytes.Checksum)
    {
        g_I2C1_ErrorReg |= I2C_ERR_CHECKSUM_ER;
    }
//    g_I2C_Rx_Process_Flag = DISABLE;
//    g_I2C1_Rx_ready_flag = ENABLE;
    GiveBackTheCompleteSemaphore();
}

/*-------- Tx DMA TC Interrupt --------*/
//void DMA1_Stream6_IRQHandler(void)
//{
//    // Disable DMA RX Channel
//    DMA_Cmd(I2C1_DMA_STREAM_TX, DISABLE);
//    // Disable I2C DMA request
//    I2C_DMACmd(I2C1, DISABLE);
//    // Clear any pending flag on Rx Stream
//    DMA_ClearFlag(I2C1_DMA_STREAM_TX,
//            I2C1_TX_DMA_TCFLAG | I2C1_TX_DMA_FEIFLAG | I2C1_TX_DMA_DMEIFLAG | I2C1_TX_DMA_TEIFLAG
//                    | I2C1_TX_DMA_HTIFLAG );
//    I2C1 ->CR1 |= I2C_CR1_STOP;
////    g_I2C_Tx_Process_Flag = 0;
//}

/*-------- I2C Event Interrupt --------*/
void I2C1_EV_IRQHandler(void)
{
    if (g_I2C_RW_Dirctn)
        Read_I2C1_EV_IRQMngr();
    else
        Write_I2C1_EV_IRQMngr();
}

/*-------- I2C Error Interrupt --------*/
void I2C1_ER_IRQHandler(void)
{
    Errors_I2C1_EV_IRQMngr();
}

/******************************************************************************/
/******************************************************************************/

/*-------- I2C Event Interrupt manager to write to the I2C sensor --------*/
void Write_I2C1_EV_IRQMngr(void)
{
    static uint32_t stat;

    stat = I2C_GetLastEvent(I2C1 );
    if (g_I2C_State == CONNECT_2_SENSOR)
    {
        if (stat == I2C_EVENT_MASTER_MODE_SELECT )
        {
            I2C1 ->DR = (g_MI3_Address << 1) & (~0x01);
        }
        if (stat == I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED )
        {
            I2C_DMACmd(I2C1, ENABLE);
            DMA_Cmd(I2C1_DMA_STREAM_TX, ENABLE);
            g_I2C_State = 0xFF;
        }
    }
}

/*-------- I2C Event Interrupt manager to read the I2C sensor --------*/
void Read_I2C1_EV_IRQMngr(void)
{
    static uint32_t stat;

    stat = I2C_GetLastEvent(I2C1 );
    switch (g_I2C_State)
    {
    case CONNECT_2_SENSOR:
    {
        if (stat == I2C_EVENT_MASTER_MODE_SELECT )
        {
            I2C1 ->DR = (g_MI3_Address << 1) & (~0x01);
        }
        if (stat == I2C_EVENT_MASTER_BYTE_TRANSMITTING )
        {
            I2C1 ->DR = g_MI3_Register;
            I2C1 ->CR1 |= I2C_CR1_START;
            g_I2C_State = READ_SENSOR;
        }
        break;
    }
    case READ_SENSOR:
    {
        if (stat == I2C_EVENT_MASTER_MODE_SELECT )
        {
            I2C1 ->DR = (g_MI3_Address << 1) | 0x01;
        }
        if (stat == I2C_EVENT_MASTER_BYTE_RECEIVED )
        {
            taskYIELD();
            I2C_DMACmd(I2C1, ENABLE);
            DMA_Cmd(I2C1_DMA_STREAM_RX, ENABLE);
            g_I2C_State = CONNECT_2_SENSOR;
        }
        break;
    }
    }
}

/*-------- I2C Errors Interrupt manager --------*/
void Errors_I2C1_EV_IRQMngr()
{
    if (I2C_GetFlagStatus(I2C1, I2C_FLAG_BERR ))
    {
        // Misplaced Start or Stop condition- Set by hardware when the interface
        // detects an SDA rising or falling edge while SCL is high, occurring in
        // a non-valid position during a byte transfer.
        g_I2C1_ErrorReg |= I2C_ERR_BERR;
        I2C_ClearFlag(I2C1, I2C_FLAG_BERR );
    }

    if (I2C_GetFlagStatus(I2C1, I2C_FLAG_ARLO ))
    {
        // Set by hardware when the interface loses the arbitration of the bus
        // to another master. After an ARLO event the interface switches back
        // automatically to Slave mode (M/SL=0).
        g_I2C1_ErrorReg |= I2C_ERR_ARLO;
        I2C_ClearFlag(I2C1, I2C_FLAG_ARLO );
    }

    if (I2C_GetFlagStatus(I2C1, I2C_FLAG_AF ))
    {
        // Acknowledge failure
        g_I2C1_ErrorReg |= I2C_ERR_AF;
        I2C_ClearFlag(I2C1, I2C_FLAG_AF );
    }

    if (I2C_GetFlagStatus(I2C1, I2C_FLAG_OVR ))
    {
        // Can't get here in master mode!
        I2C_ClearFlag(I2C1, I2C_FLAG_OVR );
    }

    if (I2C_GetFlagStatus(I2C1, I2C_FLAG_TIMEOUT ))
    {
        // SCL remained LOW for 25 ms (Timeout)
        // Master cumulative clock low extend time more than 10 ms (Tlow:mext)
        g_I2C1_ErrorReg |= I2C_ERR_TIMEOUT;
        I2C_ClearFlag(I2C1, I2C_FLAG_TIMEOUT );
    }

//    GiveBackTheCompleteSemaphore();
}

/*******************************************************************************
 * Function Name: void MI3_Temperature_Sensors_Handler(void);
 ********************************************************************************
 * Summary:
 *
 * Parameters:
 *
 * Return:
 *   none
 *******************************************************************************/
void MI3_Temperature_Sensors_Handler(void)
{
    /* write your pocess */
}

/*******************************************************************************
 * Function Name: MI3_Read(uint8_t SensAddrs, uint8_t SensReg, u8 * pRxData );
 ********************************************************************************
 * Summary:
 *
 * Parameters:
 *
 * Return:
 *   none
 *******************************************************************************/
uint8_t MI3_Read(uint8_t SensAddrs, uint8_t SensReg, u8 * pRxData)
{
    g_I2C1_ErrorReg = 0;
    /* initiating Rx IRQ parameters */
    g_I2C_RW_Dirctn = I2C_Direction_Receiver;
    g_MI3_Address = SensAddrs;
    g_MI3_Register = SensReg;
    g_I2C_State = CONNECT_2_SENSOR;
    /* Starting the Rx process */
//    g_I2C_Rx_Process_Flag = ENABLE;
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    I2C_GenerateSTART(I2C1, ENABLE);

    // now we need to wait for the semaphore to return:
    if (xSemaphoreTake(I2CDmaTransferCompleteSemaphore, M_SEMAPHORE_TIMEOUT) == pdFAIL)
        g_I2C1_ErrorReg |= I2C_ERR_TIMEOUT;

    *((uint32_t*)pRxData) = g_MasterRxMsg.RxPacketBytes.Data.u32_Cali_Date;

    return g_I2C1_ErrorReg;
}

/*******************************************************************************
 * Function Name: uint8_t MI3_Write(uint8_t *pTxData);
 ********************************************************************************
 * Summary:
 *
 * Parameters:
 *
 * Return:
 *   none
 *******************************************************************************/
void MI3_Write(uint8_t SensAddrs, uint8_t SensReg, uint8_t *pTxData)
{
    /* Parsing the command */
//    Parse_Command(pTxData);
    g_MasterTxMsg.TxPacketBytes.SlaveRegByte = SensReg;
    g_MasterTxMsg.TxPacketBytes.Data.u32_Head_ID = *((uint32_t*)pTxData);
    /* initiating Tx IRQ parameters */
    g_I2C_RW_Dirctn = I2C_Direction_Transmitter;
    g_I2C_State = CONNECT_2_SENSOR;
    g_MI3_Address = SensAddrs;  // writting the address

    /* Starting the Rx process */
//    g_I2C_Tx_Process_Flag = ENABLE;
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    I2C_GenerateSTART(I2C1, ENABLE);
}

/*******************************************************************************
 * Function Name: void PParse_Command(uint8_t *pTxData);
 ********************************************************************************
 * Summary:
 *
 * Parameters:
 *
 * Return:
 *   none
 *******************************************************************************/
void Parse_Command(uint8_t *pTxData)
{
    switch (pTxData[0])
    // Command
    {
    case e_Head_Address:
    {
        // 7 MSB Address!!
        pTxData[1] <<= 1;
        break;
    }
    case e_Status:
    {
        // masking all bits but bit 1 and 7.
        pTxData[1] &= 0x81;
        // masking rest of the data bytes.
        pTxData[2] &= 0x00;
        pTxData[3] &= 0x00;
        pTxData[4] &= 0x00;
        break;
    }
    case e_Flicker_Filter:
    {
        // masking rest of the data bytes.
        pTxData[1] &= 0x00;
        pTxData[2] &= 0x00;
        break;
    }
    case e_Laser_Control:
    {
        // masking all bits but bit 1 and 2.
        pTxData[1] &= 0x03;
        // masking rest of the data bytes.
        pTxData[2] &= 0x00;
        pTxData[3] &= 0x00;
        pTxData[4] &= 0x00;
        break;
    }
    default:
    {
        break;
    }
    }
}

/*****************/
/* global unions */
/*****************/
//Mi3RxPacket      MasterRxMsg;
//Mi3TxPacket      MasterTxMsg;
void I2C1_init(uint32_t baudrate)
{
#ifndef WIN32
    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef I2C_InitStruct;

    // enable APB1 peripheral clock for I2C1
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    // enable clock for SCL and SDA pins
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* setup SCL and SDA pins
     * You can connect the I2C1 functions to two different
     * pins:
     * 1. SCL on PB6 or PB8
     * 2. SDA on PB7 or PB9
     */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8; // we are going to use PB6 and PB8
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;       // set pins to alternate function
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;      // set GPIO speed
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD; // set output to open drain --> the line has to be only pulled low, not driven high
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;           // Disable pull up resistors
    GPIO_Init(GPIOB, &GPIO_InitStruct);                 // init GPIOB

    // Connect I2C1 pins to AF
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1); // SCL
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1); // SDA

    // configure I2C1
    I2C_InitStruct.I2C_ClockSpeed = baudrate;         // 100kHz
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;         // I2C mode
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2; // 50% duty cycle --> standard
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;          // own address, not relevant in master mode
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;        // Enable acknowledge when reading (can be changed later on)
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses
    I2C_Init(I2C1, &I2C_InitStruct);                // init I2C1

//    // Slow down the PCLK frequency.
//    I2C1->CR2 = (I2C1->CR2 & 0xFFC0) | 11;
//    /* Configure I2C Filters */
//    I2C_AnalogFilterCmd(I2C1, ENABLE);
//    I2C_DigitalFilterConfig(I2C1, 0x0F);

    //
    // enable I2C1
    I2C_Cmd(I2C1, ENABLE);
//    I2C_StretchClockCmd(I2C1, ENABLE);
    //I2C_Init(I2C1, &I2C_InitStruct);
#endif
}

void I2C2_init(uint32_t baudrate)
{
#ifndef WIN32
    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef I2C_InitStruct;

    // enable APB1 peripheral clock for I2C2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    // enable clock for SCL and SDA pins
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; // we are going to use PF0 and PF1
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;       // set pins to alternate function
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;      // set GPIO speed
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD; // set output to open drain --> the line has to be only pulled low, not driven high
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;           // Disable pull up resistors
    GPIO_Init(GPIOF, &GPIO_InitStruct);                 // init GPIOB

    // Connect I2C2 pins to AF
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_I2C2); // SCL
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_I2C2); // SDA

    // configure I2C2
    I2C_InitStruct.I2C_ClockSpeed = baudrate;         // 100kHz
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;         // I2C mode
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2; // 50% duty cycle --> standard
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;          // own address, not relevant in master mode
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;        // Enable acknowledge when reading (can be changed later on)
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses
    I2C_Init(I2C2, &I2C_InitStruct);                // init I2C2

//    // Slow down the PCLK frequency.
//    I2C2->CR2 = (I2C2->CR2 & 0xFFC0) | 11;
//    /* Configure I2C Filters */
//    I2C_AnalogFilterCmd(I2C2, ENABLE);
//    I2C_DigitalFilterConfig(I2C2, 0x0F);

    //
    // enable I2C2
    I2C_Cmd(I2C2, ENABLE);
//    I2C_StretchClockCmd(I2C2, ENABLE);
    //I2C_Init(I2C2, &I2C_InitStruct);
#endif
}

//------------------------------------------------------------------
// This function issues a start condition and
// transmits the slave address + R/W bit
//
// Parameters:
//  I2Cx  --> the I2C peripheral e.g. I2C1
//  address  --> the 7 bit slave address
//  direction  --> the tranmission direction can be:
//              I2C_Direction_Tranmitter for Master transmitter mode
//              I2C_Direction_Receiver for Master receiver
//------------------------------------------------------------------
uint8_t I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction)
{
#ifndef WIN32
    int TimeOut = 0;
    // wait until I2Cx is not busy anymore
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
    {
        TimeOut++;
        if (TimeOut > I2C_TIMEOUT)
        {
            return (0);
        }
    }
    TimeOut = 0;
    return (I2C_restart(I2Cx, address, direction));
#else
    return 1;
#endif
}

/* This function issues a start condition and
 * transmits the slave address + R/W bit
 *
 * Parameters:
 *      I2Cx --> the I2C peripheral e.g. I2C1
 *      address --> the 7 bit slave address
 *      direction --> the transmission direction can be:
 *                      I2C_Direction_Tranmitter for Master transmitter mode
 *                      I2C_Direction_Receiver for Master receiver
 */
uint8_t I2C_restart(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction)
{
#ifndef WIN32
    int TimeOut = 0;
    // Send I2Cx START condition
    I2C_GenerateSTART(I2Cx, ENABLE);
//    vTaskDelay(1);
//    I2C_StretchClockCmd(I2Cx, ENABLE);
    // wait for I2Cx EV5 --> Slave has acknowledged start condition
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        TIMEOUT_CHECK(TimeOut);
    }
    TimeOut = 0;

    // Send slave Address for write
    I2C_Send7bitAddress(I2Cx, address << 1, direction);

    /* wait for I2Cx EV6, check if
     * either Slave has acknowledged Master transmitter or
     * Master receiver mode, depending on the transmission
     * direction
     */
    switch (direction)
    {
    case I2C_Direction_Transmitter:
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        {
            TIMEOUT_CHECK(TimeOut);
        }
        TimeOut = 0;
        break;
    case I2C_Direction_Receiver:
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        {
            TIMEOUT_CHECK(TimeOut);
        }
        TimeOut = 0;
        break;
    }
#endif
    return (1);
}

/* This function transmits one byte to the slave device
 * Parameters:
 *      I2Cx --> the I2C peripheral e.g. I2C1
 *      data --> the data byte to be transmitted
 */
uint8_t I2C_write(I2C_TypeDef* I2Cx, uint8_t data)
{
#ifndef WIN32
    int TimeOut = 0;
    // wait for I2Cx EV8 --> last byte is still being transmitted (last byte in SR, buffer empty), next byte can already be written
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    {
        TimeOut++;
        TIMEOUT_CHECK(TimeOut);
    }
    TimeOut = 0;

    I2C_SendData(I2Cx, data);
    // wait for I2Cx EV8 --> last byte is still being transmitted (last byte in SR, buffer empty), next byte can already be written
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        TimeOut++;
        TIMEOUT_CHECK(TimeOut);
    }
    TimeOut = 0;
#endif
    return (1);
}

/* This function reads one byte from the slave device
 * and acknowledges the byte (requests another byte)
 */
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx)
{
#ifndef WIN32
    int TimeOut = 0;
    // enable acknowledge of received data
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
    // wait until one byte has been received
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
        TimeOut++;
        TIMEOUT_CHECK(TimeOut);
    }
    // read data from I2C data register and return data byte
    uint8_t data = I2C_ReceiveData(I2Cx);
    return data;
#else
    return 0;
#endif
}

/* This function reads one byte from the slave device
 * and doesn't acknowledge the received data
 * after that a STOP condition is transmitted
 */
uint8_t I2C_read_nack(I2C_TypeDef* I2Cx)
{
#ifndef WIN32
    int TimeOut = 0;
    // disable acknowledge of received data
    // nack also generates stop condition after last byte received
    // see reference manual for more info
    I2C_AcknowledgeConfig(I2Cx, DISABLE);
    I2C_GenerateSTOP(I2Cx, ENABLE);
    // wait until one byte has been received
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
        TimeOut++;
        TIMEOUT_CHECK(TimeOut);
    }
    // read data from I2C data register and return data byte
    uint8_t data = I2C_ReceiveData(I2Cx);
    return data;
#else
    return 0;
#endif
}

/* This function issues a stop condition and therefore
 * releases the bus
 */
uint8_t I2C_stop(I2C_TypeDef* I2Cx)
{
#ifndef WIN32
    int TimeOut = 0;

    I2C_GenerateSTOP(I2Cx, ENABLE);
    // wait for I2Cx EV8_2 --> byte has been transmitted
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        TimeOut++;
        TIMEOUT_CHECK(TimeOut);
    }
#endif
    return (1);
}

/*******************************************************************************
 * Function Name: void Checksum_Calc(U_MasterRxMsg * pMasterRxMsg);
 ********************************************************************************
 * Summary:
 *
 * Parameters:
 *
 * Return:
 *   none
 *******************************************************************************/
void Checksum_Calc(U_MasterRxMsg * pMasterRxMsg)
{
    u8 Checksum = 0;
    for (int i = 0; i < (NUM_OF_RX_BYTES - 1); ++i)
    {
        Checksum -= pMasterRxMsg->RxPacket[i];
    }
    Checksum -= 0x05;
    pMasterRxMsg->RxPacketBytes.Checksum -= Checksum;
}

uint8_t Checksum_Calc_Compare(uint8_t* data, uint8_t reg2, uint8_t rxChecksum)
{
    uint8_t checksum = 0;
    for (int i = 0; i < 4; ++i)
    {
        checksum -= data[i];
    }
    checksum -= reg2;
    checksum -= 5;
    rxChecksum -= checksum;
    return (rxChecksum);
}
//
//int Rotated_Checksum_Calc(uint8_t* buffer)
//{
//    uint8_t checksum = 0;
//    checksum = (buffer[4] + buffer[5] + buffer[0] + buffer[1] + buffer[2] + 5);
//    return (checksum == buffer[3]);
//}

