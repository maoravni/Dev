/*
 * Mi3Sensor.cpp
 *
 *  Created on: 9 αιεπ 2014
 *      Author: maora
 */

#include "Peripherals/Mi3Sensor.h"
#include "Peripherals/Mi3I2CIrPeripheral.h"

#if defined STM32F4XX
#include <stm32f4xx.h>
#elif defined WIN32
#include <Win32MissingDefines.h>
#else
#include <stm32f2xx.h>
#endif

#include <fec2/I2C_Handler.h>
#include <Win32/PortAllocations.h>

#define I2C_TIMEOUT     10000
#define TIMEOUT_CHECK(x)     x++; if(x > I2C_TIMEOUT){return(0);}

/* Define I2C Speed --------------------------------------------------------*/
//#define I2C_SPEED 10000
//#define I2C_DUTYCYCLE  I2C_DutyCycle_16_9
/* I2Cx Communication boards Interface */
#define I2C1_DMA                      DMA1
#define I2C1_DMA_CHANNEL              DMA_Channel_1
#define I2C1_DR_ADDRESS               ((uint32_t)0x40005410)
#define I2C1_DMA_STREAM_TX            DMA1_Stream6
#define I2C1_DMA_STREAM_RX            DMA1_Stream5
#define I2C1_TX_DMA_TCFLAG            DMA_FLAG_TCIF6
#define I2C1_TX_DMA_FEIFLAG           DMA_FLAG_FEIF6
#define I2C1_TX_DMA_DMEIFLAG          DMA_FLAG_DMEIF6
#define I2C1_TX_DMA_TEIFLAG           DMA_FLAG_TEIF6
#define I2C1_TX_DMA_HTIFLAG           DMA_FLAG_HTIF6
#define I2C1_RX_DMA_TCFLAG            DMA_FLAG_TCIF5
#define I2C1_RX_DMA_FEIFLAG           DMA_FLAG_FEIF5
#define I2C1_RX_DMA_DMEIFLAG          DMA_FLAG_DMEIF5
#define I2C1_RX_DMA_TEIFLAG           DMA_FLAG_TEIF5
#define I2C1_RX_DMA_HTIFLAG           DMA_FLAG_HTIF5
#define DMA1_CLK                      RCC_AHB1Periph_DMA1
#define NUM_OF_RX_BYTES               6

//uint8_t I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
uint8_t I2C_restart(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
//uint8_t I2C_write(I2C_TypeDef* I2Cx, uint8_t data);
//uint8_t I2C_read_ack(I2C_TypeDef* I2Cx);
//uint8_t I2C_read_nack(I2C_TypeDef* I2Cx);
//bool Checksum_Calc(char* data, uint8_t reg2, uint8_t rxChecksum);

//extern xSemaphoreHandle i2cTransferCompleteSemaphore;

/*****************/
/* global unions */
/*****************/
//Mi3RxPacket      MasterRxMsg;
//Mi3TxPacket      MasterTxMsg;
void I2C1_init(void)
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
    I2C_InitStruct.I2C_ClockSpeed = 10000;         // 100kHz
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

void I2C2_init(void)
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
    I2C_InitStruct.I2C_ClockSpeed = 10000;         // 100kHz
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

bool Checksum_Calc(uint8_t* data, uint8_t reg2, uint8_t rxChecksum)
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

bool Rotated_Checksum_Calc(uint8_t* buffer)
{
    uint8_t checksum = 0;
    checksum = (buffer[4] + buffer[5] + buffer[0] + buffer[1] + buffer[2] + 5);
    return (checksum == buffer[3]);
}

// todo: conside moving the pointer when reading the sensors, and not when initializing the sensor.
// this is a question of performance vs storage
Mi3Sensor::Mi3Sensor()
{
    m_targTempElement = ElementRepository::getInstance().addValidationElementFloat();
    m_numberOfReadFailures = 0;
    m_i2cChannel = I2C1;
//    m_ambTempElement = ElementRepository::getInstance().addValidationElementFloat();
}

SensorConfiguration::SensorConfiguration()
{
    m_ambientBackgroundCompensation = false;
    m_bottomTemp = 0;
    m_topTemp = 300;
    m_gain = 1;
    m_offset = 0;
    m_emissivity = 0.95;
    m_transmissivity = 1;
    m_ambientBackground = 23;
//    m_flickerFilter
}

uint8_t Mi3Sensor::read(uint8_t reg, float& data)
{
    return read(reg, (uint8_t*) &data);
}

uint8_t Mi3Sensor::read(uint8_t reg, int32_t& data)
{
    return read(reg, (uint8_t*) &data);
}

uint8_t Mi3Sensor::read(uint8_t reg, uint32_t& data)
{
    return read(reg, (uint8_t*) &data);
}

uint8_t txBuffer[10];
uint8_t rxBuffer[10];

uint8_t Mi3Sensor::read(uint8_t reg, uint8_t* data)
{
    uint8_t ErrorReg = 0;
    uint8_t Error = 0;
    uint8_t reg2;
    uint8_t rxChecksum;
    portBASE_TYPE semResult;

//    I2C_Cmd(m_i2cChannel, ENABLE);

    Error = I2C_start(m_i2cChannel, m_address, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
//    vTaskDelay(1);
    Error = I2C_write(m_i2cChannel, reg); // write one byte to the slave
//    vTaskDelay(1);
    Error = I2C_restart(m_i2cChannel, m_address, I2C_Direction_Receiver); // start a transmission in Master receiver mode
    for (int i = 0; i < 4; ++i)
    {
        data[i] = I2C_read_ack(m_i2cChannel);
    }
    reg2 = I2C_read_ack(m_i2cChannel);
    rxChecksum = I2C_read_nack(m_i2cChannel); // read one byte and don't request another byte, stop transmission
    if (Checksum_Calc(data, reg2, rxChecksum))
    {
        ErrorReg |= E_I2cErrors_ChecksumError;
    }
    if (!Error)
    {
        ErrorReg |= E_I2cErrors_Timeout;
    }
    if (reg != reg2)
    {
        ErrorReg |= E_I2cErrors_RegAddressError;
    }

//    I2C_Cmd(m_i2cChannel, DISABLE);

    return ErrorReg;
}

uint8_t Mi3Sensor::write(uint8_t reg, float& data)
{
    return write(reg, (uint8_t*) &data);
}

uint8_t Mi3Sensor::write(uint8_t reg, uint32_t& data)
{
    return write(reg, (uint8_t*) &data);
}

uint8_t Mi3Sensor::write(uint8_t reg, int32_t& data)
{
    return write(reg, (uint8_t*) &data);
}

bool Mi3Sensor::readTargetTemp()
{
    float val;
    uint8_t readResult;
    readResult = read(E_Mi3Registers_TargetTemp, val);
//    taskENTER_CRITICAL();
    if (readResult != 0)
    {
        ++m_numberOfReadFailures;
        if (m_numberOfReadFailures >= M_NUMBER_OF_RETRIES && (m_numberOfReadFailures % M_NUMBER_OF_RETRIES) != 0)
        {
//        printf("MI3 Error %x\n", readResult);
            m_targTempElement->setValueValid(false);
            // TODO: error handling when Mi3 can't be read.
        }
        return false;
    }
    if (m_numberOfReadFailures > 0)
    {
        m_targTempElement->setValueValid(true);
        m_numberOfReadFailures = 0;
    }
    m_targTempElement->setValue(val);
//    taskEXIT_CRITICAL();
//    printf("MI3 Address %d: %f\n", m_address, val);
    return true;
}

bool Mi3Sensor::readStatus()
{
    uint32_t status;
    if (read(E_Mi3Registers_Status, status))
    {
        // TODO: error handling when Mi3 can't be read.
        return false;
    }
    // TODO: handle previous valid value.
    if (status & 0x6)
        m_targTempElement->setValueValid(false);
    else
        m_targTempElement->setValueValid(true);
    return true;
}

bool Mi3Sensor::writeAmbientBackgroundCompensation(bool val)
{
    uint32_t status = 0;
    if (val)
        status = 0x80;
    return (write(E_Mi3Registers_Status, status) == 0);
}

bool Mi3Sensor::writeEmissivity(float val)
{
    return (write(E_Mi3Registers_Emissivity, val) == 0);
}

bool Mi3Sensor::writeTransmissivity(float val)
{
    return (write(E_Mi3Registers_Transmissivity, val) == 0);
}

bool Mi3Sensor::writeBottomTemp(float val)
{
    return (write(E_Mi3Registers_BottomTemp, val) == 0);
}

bool Mi3Sensor::writeTopTemp(float val)
{
    return (write(E_Mi3Registers_TopTemp, val) == 0);
}

bool Mi3Sensor::writeGain(float val)
{
    return (write(E_Mi3Registers_AdjGain, val) == 0);
}

bool Mi3Sensor::writeOffset(float val)
{
    return (write(E_Mi3Registers_AdjOffset, val) == 0);
}

bool Mi3Sensor::writeAmbientBackground(float val)
{
    return (write(E_Mi3Registers_AmbBackground, val) == 0);
}

void Mi3Sensor::setTargTempElementPssId(int pssId)
{
    m_targTempElement->setPssId(pssId);
}

uint8_t Mi3Sensor::write(uint8_t reg, uint8_t* data)
{
    uint8_t ErrorReg = 0;
    uint8_t Error = 0;

    return 0;

//    Mi3TxPacket txPacket;
//    txPacket.address = m_address;
//    txPacket.reg = reg;
//    txPacket.data.ui = *(uint32_t*) data;

    //I2C_StretchClockCmd(m_i2cChannel, ENABLE);
    Error = I2C_start(m_i2cChannel, m_address, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode

    if (!Error)
        ErrorReg++;
//    Error = I2C_write(m_i2cChannel, m_address);
//    if (!Error) ErrorReg++;
    Error = I2C_write(m_i2cChannel, reg);
    if (!Error)
        ErrorReg++;
    for (int i = 0; i < 4; ++i)
    {
        Error = I2C_write(m_i2cChannel, data[i]);
        if (!Error)
            ErrorReg++;
    }
    I2C_stop(m_i2cChannel);
    if (ErrorReg != 0)
        ErrorReg = E_I2cErrors_RegAddressError;

    vTaskDelay(45);

    return ErrorReg;
}

bool Mi3Sensor::setI2CChannel(int channelIndex)
{
    switch (channelIndex)
    {
    case 1:
        m_i2cChannel = I2C1;
        break;
    case 2:
        if (Psc_GetBoardType() == E_BoardType_Fec3)
            m_i2cChannel = I2C2;
        else
            return false;
        break;
    default:
        return false;
    }
    return true;
}
