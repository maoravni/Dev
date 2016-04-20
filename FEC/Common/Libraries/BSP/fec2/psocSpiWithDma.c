/*
 * spiWithDma.c
 *
 *  Created on: 5 бреб 2013
 *      Author: maora
 */

#include <psocSpiWithDma.h>
#ifdef STM32F4XX
#include <stm32f4xx.h>
#else
#include <stm32f2xx.h>
#endif

#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <stdio.h>
#include <ll_typedef.h>
#include <assert.h>
//#include <arm_itm.h>
#include <PscSubsystem.h>
#include <cybootloaderutils/cybtldr_utils.h>
#include <cybootloaderutils/cybtldr_api.h>

/* This structure contains function pointers to the four communication layer functions
 contained in the communication_api.c / .h */
CyBtldr_CommunicationsData CyBtldr_Callbacks;

#define CS1_HIGH()    GPIOD->BSRRL = GPIO_Pin_2
#define CS2_HIGH()    GPIOC->BSRRL = GPIO_Pin_6
#define CS3_HIGH()    GPIOC->BSRRL = GPIO_Pin_7
#define CS4_HIGH()    GPIOC->BSRRL = GPIO_Pin_8

#define CS5_FEC2_HIGH()    GPIOF->BSRRL = GPIO_Pin_0
#define CS5_FEC3_HIGH()    GPIOE->BSRRL = GPIO_Pin_2

#define CS6_HIGH()    GPIOD->BSRRL = GPIO_Pin_15
#define CS7_HIGH()    GPIOD->BSRRL = GPIO_Pin_6
#define CS8_HIGH()    GPIOC->BSRRL = GPIO_Pin_9

#define CS9_HIGH()    GPIOD->BSRRL = GPIO_Pin_7
#define CS10_HIGH()    GPIOB->BSRRL = GPIO_Pin_6

//#define CS1_LOW()    GPIOD->BSRRH = GPIO_Pin_2
//#define CS2_LOW()    GPIOC->BSRRH = GPIO_Pin_6
//#define CS3_LOW()    GPIOC->BSRRH = GPIO_Pin_7
//#define CS4_LOW()    GPIOC->BSRRH = GPIO_Pin_8
//#define CS5_LOW()    GPIOC->BSRRH = GPIO_Pin_9
//#define CS6_LOW()    GPIOD->BSRRH = GPIO_Pin_6
//#define CS7_LOW()    GPIOD->BSRRH = GPIO_Pin_15
//#define CS8_LOW()    GPIOF->BSRRH = GPIO_Pin_0

xSemaphoreHandle psocSpiResponseCompleteSemaphore = NULL;
T_PsocSpiReceiveFrame psocSpiResponseBuffer;
T_PsocSpiReceiveFrame psocSpiTransmitBuffer;
//int g_txInterruptState;
//int g_rxInterruptState;

#define M_DELAY_BETWEEN_PSOC_TRANSFERS 3

void setupSpi()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    /*!< Enable the SPI clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

    /*!< Enable GPIO clocks */
    RCC_AHB1PeriphClockCmd(
    RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOB,
            ENABLE);

    /*!< SPI pins configuration *************************************************/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

    /*!< SPI SCK MISO MOSI pin configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /*!< Connect SPI3 pins to AF6 */
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);

#ifdef STM32F4XX
    /*!< Configure CS Pins for the PSoC */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    if (Psc_BoardType == E_BoardType_Fec2)
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_6 | GPIO_Pin_15;
        GPIO_Init(GPIOD, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
        GPIO_Init(GPIOF, &GPIO_InitStructure);
        CS1_HIGH();
        CS2_HIGH();
        CS3_HIGH();
        CS4_HIGH();
        CS5_FEC2_HIGH();
        CS6_HIGH();
        CS7_HIGH();
        CS8_HIGH();
    }
    if (Psc_BoardType == E_BoardType_Fec3)
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_6 | GPIO_Pin_15 | GPIO_Pin_7;
        GPIO_Init(GPIOD, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        GPIO_Init(GPIOE, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        // This is needed for resetting the PSoCs:
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
        GPIO_Init(GPIOF, &GPIO_InitStructure);

        CS1_HIGH();
        CS2_HIGH();
        CS3_HIGH();
        CS4_HIGH();
        CS5_FEC3_HIGH();
        CS6_HIGH();
        CS7_HIGH();
        CS8_HIGH();
        CS9_HIGH();
        CS10_HIGH();
    }

#endif

    SPI_I2S_DeInit(PSOC_SPI_PORT);
    /*!< SPI configuration */
    SPI_StructInit(&SPI_InitStructure);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;   //SPI_CPHA_1Edge
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(PSOC_SPI_PORT, &SPI_InitStructure);

    /*!< Enable the SPI  */
    SPI_Cmd(PSOC_SPI_PORT, ENABLE);

}

void setupTxDma()
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // Set up the DMA
    DMA_DeInit(PSOC_SPI_PORT_TX_DMA_STREAM);
    /*
     * Check if the DMA Stream is disabled before enabling it.
     * Note that this step is useful when the same Stream is used multiple times:
     * enabled, then disabled then re-enabled... In this case, the DMA Stream disable
     * will be effective only at the end of the ongoing data transfer and it will
     * not be possible to re-configure it before making sure that the Enable bit
     * has been cleared by hardware. If the Stream is used only once, this step might
     * be bypassed.
     */
    while (DMA_GetCmdStatus(PSOC_SPI_PORT_TX_DMA_STREAM) != DISABLE)
        ;
    // Configure DMA controller to manage TX DMA requests
    // first make sure we are using the default values
    DMA_StructInit(&DMA_InitStructure);
    // these are the only parameters that change from the defaults
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (PSOC_SPI_PORT->DR);
    DMA_InitStructure.DMA_Channel = PSOC_SPI_PORT_TX_DMA_CHANNEL;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    /*
     * It is not possible to call DMA_Init without values for the source
     * address and non-zero size even though a transfer is not done here.
     * These are checked only when the assert macro are active though.
     * */
    DMA_InitStructure.DMA_Memory0BaseAddr = 0;
    DMA_InitStructure.DMA_BufferSize = 1;
    DMA_Init(PSOC_SPI_PORT_TX_DMA_STREAM, &DMA_InitStructure);
    // Enable the DMA transfer complete interrupt
    DMA_ITConfig(PSOC_SPI_PORT_TX_DMA_STREAM, DMA_IT_TC, ENABLE);

    // enable the interrupt in the NVIC
    NVIC_InitStructure.NVIC_IRQChannel = PSOC_SPI_PORT_DMA_TX_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (uint8_t)(configMAX_SYSCALL_INTERRUPT_PRIORITY >> 4) + 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // Enable dma tx request.
    SPI_I2S_DMACmd(PSOC_SPI_PORT, SPI_I2S_DMAReq_Tx, ENABLE);
}

void setupRxDma()
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // Set up the DMA
    DMA_DeInit(PSOC_SPI_PORT_RX_DMA_STREAM);
    /*
     * Check if the DMA Stream is disabled before enabling it.
     * Note that this step is useful when the same Stream is used multiple times:
     * enabled, then disabled then re-enabled... In this case, the DMA Stream disable
     * will be effective only at the end of the ongoing data transfer and it will
     * not be possible to re-configure it before making sure that the Enable bit
     * has been cleared by hardware. If the Stream is used only once, this step might
     * be bypassed.
     */
    while (DMA_GetCmdStatus(PSOC_SPI_PORT_RX_DMA_STREAM) != DISABLE)
        ;
    // Configure DMA controller to manage RX DMA requests
    // first make sure we are using the default values
    DMA_StructInit(&DMA_InitStructure);
    // these are the only parameters that change from the defaults
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (PSOC_SPI_PORT->DR);
    DMA_InitStructure.DMA_Channel = PSOC_SPI_PORT_RX_DMA_CHANNEL;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;

    /*
     * It is not possible to call DMA_Init without values for the source
     * address and non-zero size even though a transfer is not done here.
     * These are checked only when the assert macro are active though.
     * */
    DMA_InitStructure.DMA_Memory0BaseAddr = 0;
    DMA_InitStructure.DMA_BufferSize = 1;
    DMA_Init(PSOC_SPI_PORT_RX_DMA_STREAM, &DMA_InitStructure);
    // Enable the DMA transfer complete interrupt

    DMA_ITConfig(PSOC_SPI_PORT_RX_DMA_STREAM, DMA_IT_TC, ENABLE);

    // enable the interrupt in the NVIC
    NVIC_InitStructure.NVIC_IRQChannel = PSOC_SPI_PORT_DMA_RX_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (uint8_t)(configMAX_SYSCALL_INTERRUPT_PRIORITY >> 4) + 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void setupDma()
{
    // first enable the clock
    RCC_AHB1PeriphClockCmd(PSOC_SPI_PORT_DMAx_CLK, ENABLE); // start with a blank DMA configuration just to be sure

    setupTxDma();
    setupRxDma();

    // Enable dma rx +tx request.
    SPI_I2S_DMACmd(PSOC_SPI_PORT, SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx, ENABLE);
}

void PSOC_setupSpiWithDma()
{
    setupSpi();
    setupDma();

    // setup the semaphore for notification of transfer complete:
    if (psocSpiResponseCompleteSemaphore == NULL)
        vSemaphoreCreateBinary(psocSpiResponseCompleteSemaphore);
    xSemaphoreTake(psocSpiResponseCompleteSemaphore, 0);
}

void PSOC_fullDuplexTransfer(uint8_t *transmitBuffer, uint8_t length)
{
    assert(length <= M_PSOC_SPI_MAX_TRANSFER_SIZE);

    memcpy(psocSpiTransmitBuffer.buffer, transmitBuffer, length);
    psocSpiResponseBuffer.crcOk = false;
    psocSpiResponseBuffer.transferComplete = false;
    PSOC_SPI_PORT_TX_DMA_STREAM->NDTR = (uint32_t) length;
    PSOC_SPI_PORT_TX_DMA_STREAM->M0AR = (uint32_t) psocSpiTransmitBuffer.buffer;
    PSOC_SPI_PORT_RX_DMA_STREAM->NDTR = (uint32_t) length;
    PSOC_SPI_PORT_RX_DMA_STREAM->M0AR = (uint32_t) & (psocSpiResponseBuffer.buffer);
//    CS1_LOW();
    SPI_CalculateCRC(PSOC_SPI_PORT, ENABLE);
    DMA_Cmd(PSOC_SPI_PORT_RX_DMA_STREAM, DISABLE);
    DMA_Cmd(PSOC_SPI_PORT_TX_DMA_STREAM, DISABLE);
    DMA_Cmd(PSOC_SPI_PORT_RX_DMA_STREAM, ENABLE);
    DMA_Cmd(PSOC_SPI_PORT_TX_DMA_STREAM, ENABLE);
}

void PSOC_fullDuplexTransferNoCrc(uint8_t *transmitBuffer, uint8_t length)
{
    assert(length <= M_PSOC_SPI_MAX_TRANSFER_SIZE);

    memcpy(psocSpiTransmitBuffer.buffer, transmitBuffer, length);
    psocSpiResponseBuffer.crcOk = false;
    psocSpiResponseBuffer.transferComplete = false;
    PSOC_SPI_PORT_TX_DMA_STREAM->NDTR = (uint32_t) length;
    PSOC_SPI_PORT_TX_DMA_STREAM->M0AR = (uint32_t) psocSpiTransmitBuffer.buffer;
    PSOC_SPI_PORT_RX_DMA_STREAM->NDTR = (uint32_t) length;
    PSOC_SPI_PORT_RX_DMA_STREAM->M0AR = (uint32_t) & (psocSpiResponseBuffer.buffer);
//    CS1_LOW();
    SPI_CalculateCRC(PSOC_SPI_PORT, DISABLE);
    DMA_Cmd(PSOC_SPI_PORT_RX_DMA_STREAM, DISABLE);
    DMA_Cmd(PSOC_SPI_PORT_TX_DMA_STREAM, DISABLE);
    DMA_Cmd(PSOC_SPI_PORT_RX_DMA_STREAM, ENABLE);
    DMA_Cmd(PSOC_SPI_PORT_TX_DMA_STREAM, ENABLE);
}

//void PSOC_sendBuffer(uint8_t *buf, uint8_t len)
//{
//    PSOC_SPI_PORT_TX_DMA_STREAM->NDTR = (uint32_t) len;
//    PSOC_SPI_PORT_TX_DMA_STREAM->M0AR = (uint32_t) buf;
//    SPI_BiDirectionalLineConfig(PSOC_SPI_PORT, SPI_Direction_Tx);
//    CS1_LOW();
//    DMA_Cmd(PSOC_SPI_PORT_TX_DMA_STREAM, ENABLE);
//}

void PSOC_startReceive()
{
//    uint8_t buf[16];
//    memset(buf, 2, 16);
//    PSOC_SPI_PORT_RX_DMA_STREAM ->NDTR = (uint32_t) 16;
//    PSOC_SPI_PORT_RX_DMA_STREAM ->M0AR = (uint32_t) receiveBuffer;
//    PSOC_SPI_PORT_TX_DMA_STREAM ->NDTR = (uint32_t) 16;
//    PSOC_SPI_PORT_TX_DMA_STREAM ->M0AR = (uint32_t) buf;
//    DMA_Cmd(PSOC_SPI_PORT_RX_DMA_STREAM, ENABLE);
////    SPI_BiDirectionalLineConfig(PSOC_SPI_PORT, SPI_Direction_Rx);
//    DMA_Cmd(PSOC_SPI_PORT_TX_DMA_STREAM, ENABLE);
////    SPI_BiDirectionalLineConfig(PSOC_SPI_PORT, SPI_Direction_Rx);
}

#define M_CHECK_RXNE_RESET_TIMEOUT 0x1000000

// the transmit complete interrupt handler
void PSOC_SPI_PORT_DMA_TX_IRQHandler()
{
//    ITM_EVENT8(2,1);
    static int checkRxneResetCounter;
    static portBASE_TYPE xHigherPriorityTaskWoken;
//    g_txInterruptState = 0;
    // Test if DMA Stream Transfer Complete interrupt
    if (DMA_GetITStatus(PSOC_SPI_PORT_TX_DMA_STREAM, PSOC_SPI_PORT_TX_DMA_IT_TCIF))
    {
//        g_txInterruptState = 1;
        DMA_ClearITPendingBit(PSOC_SPI_PORT_TX_DMA_STREAM, PSOC_SPI_PORT_TX_DMA_IT_TCIF);
//        g_txInterruptState = 2;
        /*
         * There is an unpleasant wait until we are certain the data has been sent.
         * The need for this has been verified by oscilloscope. The shift register
         * at this point may still be clocking out data and it is not safe to
         * release the chip select line until it has finished. It only costs half
         * a microsecond so better safe than sorry. Is it...
         *
         *  a) flushed from the transmit buffer
         */
//        while (SPI_I2S_GetFlagStatus(PSOC_SPI_PORT, SPI_I2S_FLAG_TXE ) == RESET)
//            ;
//        g_txInterruptState = 3;
        /*
         * b) flushed out of the shift register
         */
//        while (SPI_I2S_GetFlagStatus(PSOC_SPI_PORT, SPI_I2S_FLAG_BSY ) == SET)
//            ;
//        g_txInterruptState = 4;
        /*
         * The DMA stream is disabled in hardware at the end of the transfer
         * Now we can deselect the display. If more than one peripheral was being run
         * on this SPI peripheral, we would have to do both/all of them, or work out
         * which one was active and deselect that one.
         */

//        checkRxneResetCounter = 0;
//        // read the last byte from the receive buffer - it's the received CRC;
//        while (checkRxneResetCounter < M_CHECK_RXNE_RESET_TIMEOUT
//                && SPI_I2S_GetFlagStatus(PSOC_SPI_PORT, SPI_I2S_FLAG_RXNE ) == RESET)
//            ++checkRxneResetCounter;
//        uint8_t tempCRC = PSOC_SPI_PORT ->DR;
//
////        psocSpiResponseBuffer.transferComplete = (checkRxneResetCounter < M_CHECK_RXNE_RESET_TIMEOUT);
//        if (checkRxneResetCounter < M_CHECK_RXNE_RESET_TIMEOUT)
//            psocSpiResponseBuffer.transferComplete = true;
//        else
//            psocSpiResponseBuffer.transferComplete = false;
//
//        if (SPI_I2S_GetFlagStatus(PSOC_SPI_PORT, SPI_FLAG_CRCERR ) == SET)
//        {
////            puts("CRC Error");
//            psocSpiResponseBuffer.crcOk = false;
//            SPI_I2S_ClearFlag(PSOC_SPI_PORT, SPI_FLAG_CRCERR );
//        }
//
//        SPI_CalculateCRC(PSOC_SPI_PORT, DISABLE);
//
//        xSemaphoreGiveFromISR(psocSpiResponseCompleteSemaphore, &xHigherPriorityTaskWoken);
//        if (xHigherPriorityTaskWoken)
//            taskYIELD();
    }
//    g_txInterruptState = 5;
//    ITM_EVENT8(2,0);
}

// the receive complete interrupt handler
void PSOC_SPI_PORT_DMA_RX_IRQHandler()
{
//    ITM_EVENT8(3,1);
    static int checkRxneResetCounter;
    static portBASE_TYPE xHigherPriorityTaskWoken;
//    g_rxInterruptState = 0;
    // Test if DMA Stream Transfer Complete interrupt
    if (DMA_GetITStatus(PSOC_SPI_PORT_RX_DMA_STREAM, PSOC_SPI_PORT_RX_DMA_IT_TCIF))
    {
//        g_rxInterruptState = 1;
        DMA_ClearITPendingBit(PSOC_SPI_PORT_RX_DMA_STREAM, PSOC_SPI_PORT_RX_DMA_IT_TCIF);
//        g_rxInterruptState = 2;
//        SPI_BiDirectionalLineConfig(PSOC_SPI_PORT, SPI_Direction_Tx);
        /*
         * There is an unpleasant wait until we are certain the data has been sent.
         * The need for this has been verified by oscilloscope. The shift register
         * at this point may still be clocking out data and it is not safe to
         * release the chip select line until it has finished. It only costs half
         * a microsecond so better safe than sorry. Is it...
         *
         *  a) flushed from the transmit buffer
         */
//        while (SPI_I2S_GetFlagStatus(PSOC_SPI_PORT, SPI_I2S_FLAG_TXE ) == RESET)
//            ;
        /*
         * b) flushed out of the shift register
         */
//        while (SPI_I2S_GetFlagStatus(PSOC_SPI_PORT, SPI_I2S_FLAG_BSY ) == SET)
//            ;
        /*
         * The DMA stream is disabled in hardware at the end of the transfer
         * Now we can deselect the display. If more than one peripheral was being run
         * on this SPI peripheral, we would have to do both/all of them, or work out
         * which one was active and deselect that one.
         */
        if (PSOC_SPI_PORT->CR1 & SPI_CR1_CRCEN)
        {
            checkRxneResetCounter = 0;
            // read the last byte from the receive buffer - it's the received CRC;
            while (checkRxneResetCounter < M_CHECK_RXNE_RESET_TIMEOUT
                    && SPI_I2S_GetFlagStatus(PSOC_SPI_PORT, SPI_I2S_FLAG_RXNE) == RESET)
                ++checkRxneResetCounter;
            uint8_t tempCRC = PSOC_SPI_PORT->DR;

//        g_rxInterruptState = 3;
//        psocSpiResponseBuffer.transferComplete = (checkRxneResetCounter < M_CHECK_RXNE_RESET_TIMEOUT);
            if (checkRxneResetCounter < M_CHECK_RXNE_RESET_TIMEOUT)
                psocSpiResponseBuffer.transferComplete = true;
            else
                psocSpiResponseBuffer.transferComplete = false;

//        g_rxInterruptState = 4;
            if (SPI_I2S_GetFlagStatus(PSOC_SPI_PORT, SPI_FLAG_CRCERR) == SET)
            {
//            puts("CRC Error");
                psocSpiResponseBuffer.crcOk = false;
                SPI_I2S_ClearFlag(PSOC_SPI_PORT, SPI_FLAG_CRCERR);
            }
            else
                psocSpiResponseBuffer.crcOk = true;
        }
        else
        {
            psocSpiResponseBuffer.transferComplete = true;
            psocSpiResponseBuffer.crcOk = true;
        }

//        g_rxInterruptState = 5;

        SPI_CalculateCRC(PSOC_SPI_PORT, DISABLE);

//        g_rxInterruptState = 6;

        // yield before giving the semaphore, in case the interrupt returned before than the
        // calling task could wait for the semaphore.
//        taskYIELD();

        xSemaphoreGiveFromISR(psocSpiResponseCompleteSemaphore, &xHigherPriorityTaskWoken);

//        g_rxInterruptState = 7;
        // FreeRTOS manual specifies that if a higher priority task is waken, the task should yield.
        // TOD: Check if the "taskYIELD" call can be removed, as the interrupt is finished here, and the
        // scheduler will probably context-switch to a higher prioriy task anyway.
        if (xHigherPriorityTaskWoken)
        {
            taskYIELD();
        }

//        g_rxInterruptState = 8;
    }
//    g_rxInterruptState = 9;
//    ITM_EVENT8(3,0);
}

/*******************************************************************************
 * Function Name: OpenConnection
 ********************************************************************************
 *
 * Summary:
 *  Initializes the communications component : In this case SPI
 *
 * Parameters:
 *  void
 *
 * Return:
 *  Returns a flag to indicate whether the operation was successful or not
 *
 *
 *******************************************************************************/
int PsocBootloader_OpenConnection(void)
{
    SPI_CalculateCRC(PSOC_SPI_PORT, DISABLE);
    //SPIM_Start();
    return (CYRET_SUCCESS);
}

/*******************************************************************************
 * Function Name: CloseConnection
 ********************************************************************************
 *
 * Summary:
 *  Clears the status and stops the communications component (SPI).
 *
 * Parameters:
 *  void
 *
 * Return:
 *  Returns a flag to indicate whether the operation was successful or not
 *
 *
 *******************************************************************************/
int PsocBootloader_CloseConnection(void)
{
    SPI_CalculateCRC(PSOC_SPI_PORT, ENABLE);
    //SPIM_Stop();
    return (CYRET_SUCCESS);
}

uint16_t Send_Receive_HalfWord(uint16_t HalfWord)
{

    /*!< Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(PSOC_SPI_PORT, SPI_I2S_FLAG_TXE) == RESET)
        ;

    /*!< Send Half Word through the the SPI bus */
    SPI_I2S_SendData(PSOC_SPI_PORT, HalfWord);

    /*!< Wait to receive a Half Word */
    while (SPI_I2S_GetFlagStatus(PSOC_SPI_PORT, SPI_I2S_FLAG_RXNE) == RESET)
        ;
    while (SPI_I2S_GetFlagStatus(PSOC_SPI_PORT, SPI_I2S_FLAG_BSY) == SET)
        ;
    /*!< Return the Half Word read from the SPI bus */
    return SPI_I2S_ReceiveData(PSOC_SPI_PORT);

}

/*******************************************************************************
 * Function Name: WriteData
 ********************************************************************************
 *
 * Summary:
 *  Writes the specified number of bytes using the communications component (SPI)
 *
 * Parameters:
 *  wrData - Pointer to write data buffer
 *  byteCnt - No. of bytes to be written
 *
 * Return:
 *  Returns a flag to indicate whether the operation was successful or not
 *
 *
 *******************************************************************************/
int PsocBootloader_WriteDataNoDma(uint8_t* wrData, int byteCnt)
{
    uint8_t dummy;
    SPI_CalculateCRC(PSOC_SPI_PORT, DISABLE);

    /* Enable the SPI peripheral */
//    SPI_Cmd(PSOC_SPI_PORT, ENABLE);
    for (int i = 0; i < byteCnt; ++i)
        dummy = Send_Receive_HalfWord(wrData[i]);

//    uint8_t tempCRC = PSOC_SPI_PORT->DR;

    vTaskDelay(M_DELAY_BETWEEN_PSOC_TRANSFERS);
    /* Disable the SPI peripheral */
//    SPI_Cmd(PSOC_SPI_PORT, DISABLE);
    SPI_CalculateCRC(PSOC_SPI_PORT, DISABLE);
    return (CYRET_SUCCESS);
}

int PsocBootloader_WriteDataNonBlocking(uint8_t* wrData, int byteCnt)
{
    static int checkRxneResetCounter;
    checkRxneResetCounter = 0;

    printf("%d WriteData PSOC_fullDuplexTransferNoCrc 1\n", xTaskGetTickCount());

    PSOC_fullDuplexTransferNoCrc(wrData, byteCnt);

    printf("%d WriteData PSOC_fullDuplexTransferNoCrc 2\n", xTaskGetTickCount());

//    while (checkRxneResetCounter < M_CHECK_RXNE_RESET_TIMEOUT
//            && SPI_I2S_GetFlagStatus(PSOC_SPI_PORT, SPI_I2S_FLAG_RXNE) == RESET)
//        ++checkRxneResetCounter;
//    if (checkRxneResetCounter >= M_CHECK_RXNE_RESET_TIMEOUT)
//        return CYRET_ERR_CMD;
//    taskYIELD();
    if (xSemaphoreTake(psocSpiResponseCompleteSemaphore, 50) == pdFALSE)
    {
        return CYRET_ERR_CMD;
    }
//    taskYIELD();

    printf("%d WriteData PSOC_fullDuplexTransferNoCrc 3\n", xTaskGetTickCount());

    return CYRET_SUCCESS;
}

int PsocBootloader_WriteData(uint8_t* wrData, int byteCnt)
{
    /*
     u32 timeOut =1;

     // Clears TX and RX FIFOs and the status registers
     SPIM_ClearFIFO();

     SPIM_PutArray(wrData, byteCnt);

     // Wait till read operation is complete or timeout
     while(!(SPIM_ReadTxStatus() & SPIM_STS_SPI_DONE))
     {
     timeOut++;
     // Check for timeout and if so exit with communication error code
     if(timeOut == 0)
     {
     return(CYRET_ERR_COMM_MASK);
     }
     }
     // Give some delay before the next operation so that the slave can process the written data
     CyDelay(30);
     */
    uint8_t Dummy_rdData[M_PSOC_SPI_MAX_TRANSFER_SIZE];

    DMA_InitTypeDef DMA_InitStructure;

    /* Configure DMA Initialization Structure */
    DMA_InitStructure.DMA_BufferSize = byteCnt;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(PSOC_SPI_PORT->DR));
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    /* Configure TX DMA */
    DMA_InitStructure.DMA_Channel = PSOC_SPI_PORT_TX_DMA_CHANNEL;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) wrData;
    DMA_Init(PSOC_SPI_PORT_TX_DMA_STREAM, &DMA_InitStructure);
    /* Configure RX DMA */

    DMA_InitStructure.DMA_Channel = PSOC_SPI_PORT_RX_DMA_CHANNEL;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) Dummy_rdData;
    DMA_Init(PSOC_SPI_PORT_RX_DMA_STREAM, &DMA_InitStructure);

    SPI_CalculateCRC(PSOC_SPI_PORT, DISABLE);

    /* The Data transfer is performed in the SPI using Direct Memory Access */

    /* Enable DMA SPI TX Stream */
    DMA_Cmd(PSOC_SPI_PORT_TX_DMA_STREAM, ENABLE);

    /* Enable DMA SPI RX Stream */
    DMA_Cmd(PSOC_SPI_PORT_RX_DMA_STREAM, ENABLE);

    /* Enable SPI DMA TX Requsts */
    SPI_I2S_DMACmd(PSOC_SPI_PORT, SPI_I2S_DMAReq_Tx, ENABLE);

    /* Enable SPI DMA RX Requsts */
    SPI_I2S_DMACmd(PSOC_SPI_PORT, SPI_I2S_DMAReq_Rx, ENABLE);

    /* Enable the SPI peripheral */
//    SPI_Cmd(PSOC_SPI_PORT, ENABLE);
    /* Waiting the end of Data transfer */
    while (DMA_GetFlagStatus(PSOC_SPI_PORT_TX_DMA_STREAM, PSOC_SPI_PORT_TX_DMA_FLAG_TCIF) == RESET)
        ;
    while (DMA_GetFlagStatus(PSOC_SPI_PORT_RX_DMA_STREAM, PSOC_SPI_PORT_RX_DMA_FLAG_TCIF) == RESET)
        ;

    /* Clear DMA Transfer Complete Flags */
    DMA_ClearFlag(PSOC_SPI_PORT_TX_DMA_STREAM, PSOC_SPI_PORT_TX_DMA_FLAG_TCIF);
    DMA_ClearFlag(PSOC_SPI_PORT_RX_DMA_STREAM, PSOC_SPI_PORT_RX_DMA_FLAG_TCIF);

    /* Disable DMA SPI TX Stream */
    DMA_Cmd(PSOC_SPI_PORT_TX_DMA_STREAM, DISABLE);

    /* Disable DMA SPI RX Stream */
    DMA_Cmd(PSOC_SPI_PORT_RX_DMA_STREAM, DISABLE);

    /* Disable SPI DMA TX Requsts */
    SPI_I2S_DMACmd(PSOC_SPI_PORT, SPI_I2S_DMAReq_Tx, DISABLE);

    /* Disable SPI DMA RX Requsts */
    SPI_I2S_DMACmd(PSOC_SPI_PORT, SPI_I2S_DMAReq_Rx, DISABLE);

    while (SPI_I2S_GetFlagStatus(PSOC_SPI_PORT, SPI_I2S_FLAG_RXNE) == RESET)
        ;
    //tempCRC = PSOC_SPI_PORT->DR;//dummi read

    /* Disable the SPI peripheral */
//    SPI_Cmd(PSOC_SPI_PORT, DISABLE);
    SPI_CalculateCRC(PSOC_SPI_PORT, DISABLE);
    return (CYRET_SUCCESS);
}

/*******************************************************************************
 * Function Name: ReadData
 ********************************************************************************
 *
 * Summary:
 *  Reads the specified number of bytes usign the communications component (SPI)
 *
 * Parameters:
 *  rdData - Pointer to read data buffer
 *  byteCnt - No. of bytes to be read
 *
 * Return:
 *  Returns a flag to indicate whether the operation was successful or not
 *
 *
 *******************************************************************************/
int PsocBootloader_ReadDataNoDma(uint8_t* rdData, int byteCnt)
{
    uint8_t dummy;

    SPI_CalculateCRC(PSOC_SPI_PORT, DISABLE);

    /* Enable the SPI peripheral */
    //SPI_Cmd(PSOC_SPI_PORT, ENABLE);
    for (int i = 0; i < byteCnt; ++i)
        rdData[i] = Send_Receive_HalfWord(dummy);

//    uint8_t tempCRC = PSOC_SPI_PORT->DR;
    /* Disable the SPI peripheral */
    //SPI_Cmd(PSOC_SPI_PORT, DISABLE);
    SPI_CalculateCRC(PSOC_SPI_PORT, DISABLE);

    vTaskDelay(M_DELAY_BETWEEN_PSOC_TRANSFERS);
    return (CYRET_SUCCESS);
}

int PsocBootloader_ReadDataNonBlocking(uint8_t* rdData, int byteCnt)
{
    static int checkRxneResetCounter;
    checkRxneResetCounter = 0;
    uint8_t dummyBuffer[M_PSOC_SPI_MAX_TRANSFER_SIZE];

    PSOC_fullDuplexTransferNoCrc(dummyBuffer, byteCnt);

//    while (checkRxneResetCounter < M_CHECK_RXNE_RESET_TIMEOUT
//            && SPI_I2S_GetFlagStatus(PSOC_SPI_PORT, SPI_I2S_FLAG_RXNE) == RESET)
//        ++checkRxneResetCounter;
//    if (checkRxneResetCounter >= M_CHECK_RXNE_RESET_TIMEOUT)
//        return CYRET_ERR_CMD;
//    taskYIELD();
    if (xSemaphoreTake(psocSpiResponseCompleteSemaphore, 50) == pdFALSE)
    {
        return CYRET_ERR_CMD;
    }
//    taskYIELD();

    memcpy(rdData, psocSpiResponseBuffer.buffer, byteCnt);
    return CYRET_SUCCESS;
}

int PsocBootloader_ReadData(uint8_t* rdData, int byteCnt)
{
    uint8_t Dummy_wrData[M_PSOC_SPI_MAX_TRANSFER_SIZE];

    DMA_InitTypeDef DMA_InitStructure;

    /* Configure DMA Initialization Structure */
    DMA_InitStructure.DMA_BufferSize = byteCnt;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(PSOC_SPI_PORT->DR));
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    /* Configure TX DMA */
    DMA_InitStructure.DMA_Channel = PSOC_SPI_PORT_TX_DMA_CHANNEL;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) Dummy_wrData;
    DMA_Init(PSOC_SPI_PORT_TX_DMA_STREAM, &DMA_InitStructure);
    /* Configure RX DMA */

    DMA_InitStructure.DMA_Channel = PSOC_SPI_PORT_RX_DMA_CHANNEL;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) rdData;
    DMA_Init(PSOC_SPI_PORT_RX_DMA_STREAM, &DMA_InitStructure);

    SPI_CalculateCRC(PSOC_SPI_PORT, DISABLE);

    /* The Data transfer is performed in the SPI using Direct Memory Access */

    /* Enable DMA SPI TX Stream */
    DMA_Cmd(PSOC_SPI_PORT_TX_DMA_STREAM, ENABLE);

    /* Enable DMA SPI RX Stream */
    DMA_Cmd(PSOC_SPI_PORT_RX_DMA_STREAM, ENABLE);

    /* Enable SPI DMA TX Requsts */
    SPI_I2S_DMACmd(PSOC_SPI_PORT, SPI_I2S_DMAReq_Tx, ENABLE);

    /* Enable SPI DMA RX Requsts */
    SPI_I2S_DMACmd(PSOC_SPI_PORT, SPI_I2S_DMAReq_Rx, ENABLE);

    /* Enable the SPI peripheral */
//    SPI_Cmd(PSOC_SPI_PORT, ENABLE);
    /* Waiting the end of Data transfer */
    while (DMA_GetFlagStatus(PSOC_SPI_PORT_TX_DMA_STREAM, PSOC_SPI_PORT_TX_DMA_FLAG_TCIF) == RESET)
        ;
    while (DMA_GetFlagStatus(PSOC_SPI_PORT_RX_DMA_STREAM, PSOC_SPI_PORT_RX_DMA_FLAG_TCIF) == RESET)
        ;

    /* Clear DMA Transfer Complete Flags */
    DMA_ClearFlag(PSOC_SPI_PORT_TX_DMA_STREAM, PSOC_SPI_PORT_TX_DMA_FLAG_TCIF);
    DMA_ClearFlag(PSOC_SPI_PORT_RX_DMA_STREAM, PSOC_SPI_PORT_RX_DMA_FLAG_TCIF);

    /* Disable DMA SPI TX Stream */
    DMA_Cmd(PSOC_SPI_PORT_TX_DMA_STREAM, DISABLE);

    /* Disable DMA SPI RX Stream */
    DMA_Cmd(PSOC_SPI_PORT_RX_DMA_STREAM, DISABLE);

    /* Disable SPI DMA TX Requsts */
    SPI_I2S_DMACmd(PSOC_SPI_PORT, SPI_I2S_DMAReq_Tx, DISABLE);

    /* Disable SPI DMA RX Requsts */
    SPI_I2S_DMACmd(PSOC_SPI_PORT, SPI_I2S_DMAReq_Rx, DISABLE);

    while (SPI_I2S_GetFlagStatus(PSOC_SPI_PORT, SPI_I2S_FLAG_RXNE) == RESET)
        ;
    //tempCRC = PSOC_SPI_PORT->DR;//dummi read

    /* Disable the SPI peripheral */
//    SPI_Cmd(PSOC_SPI_PORT, DISABLE);
    SPI_CalculateCRC(PSOC_SPI_PORT, DISABLE);
    return (CYRET_SUCCESS);
}

CyBtldr_CommunicationsData* PsocBootloader_Setup()
{
//    // Set up the DMA
//    DMA_DeInit(PSOC_SPI_PORT_TX_DMA_STREAM);
//    // Set up the DMA
//    DMA_DeInit(PSOC_SPI_PORT_RX_DMA_STREAM);
//
//    NVIC_InitTypeDef NVIC_InitStructure;
//    // enable the interrupt in the NVIC
//    NVIC_InitStructure.NVIC_IRQChannel = PSOC_SPI_PORT_DMA_TX_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (uint8_t)(configMAX_SYSCALL_INTERRUPT_PRIORITY >> 4) + 2;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
//    NVIC_Init(&NVIC_InitStructure);
//    // enable the interrupt in the NVIC
//    NVIC_InitStructure.NVIC_IRQChannel = PSOC_SPI_PORT_DMA_RX_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (uint8_t)(configMAX_SYSCALL_INTERRUPT_PRIORITY >> 4) + 3;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
//    NVIC_Init(&NVIC_InitStructure);
//    DMA_ITConfig(PSOC_SPI_PORT_TX_DMA_STREAM, DMA_IT_TC, DISABLE);
//    DMA_ITConfig(PSOC_SPI_PORT_RX_DMA_STREAM, DMA_IT_TC, DISABLE);

    CyBtldr_Callbacks.OpenConnection = &PsocBootloader_OpenConnection;
    CyBtldr_Callbacks.CloseConnection = &PsocBootloader_CloseConnection;
//    CyBtldr_Callbacks.ReadData = &PsocBootloader_ReadData;
//    CyBtldr_Callbacks.WriteData = &PsocBootloader_WriteData;
    CyBtldr_Callbacks.ReadData = &PsocBootloader_ReadDataNoDma;
    CyBtldr_Callbacks.WriteData = &PsocBootloader_WriteDataNoDma;
//    CyBtldr_Callbacks.ReadData = &PsocBootloader_ReadDataNonBlocking;
//    CyBtldr_Callbacks.WriteData = &PsocBootloader_WriteDataNonBlocking;
    CyBtldr_Callbacks.MaxTransferSize = M_PSOC_SPI_MAX_TRANSFER_SIZE;
    return &CyBtldr_Callbacks;
}

/* [] END OF FILE */

