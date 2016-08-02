/*
 * Mi3I2CIrPeripheral.cpp
 *
 *  Created on: 9 αιεπ 2014
 *      Author: maora
 */

#include "Mi3I2CIrPeripheral.h"
#include <logger.h>
#include <Win32/PortAllocations.h>
#include <Persistency/Mi3PeripheralSerializer.h>

xSemaphoreHandle i2cTransferCompleteSemaphore = NULL;

#define M_I2C_POWERDOWN_TIME 2000
#define M_I2C_POWERUP_TIME 5000
#define M_RETRIES_AFTER_ALL_INVALID 10
#include <Persistency/PeripheralSerializers.h>

//void CPAL_I2C_DMATXTC_UserCallback(CPAL_InitTypeDef* cpalInitStruct)
//{
//    signed portBASE_TYPE highPriorityWoken;
//    xSemaphoreGiveFromISR(i2cTransferCompleteSemaphore, &highPriorityWoken);
////    TransferCompleteSemaphore.give();
//    if (highPriorityWoken)
//        taskYIELD();
//}
//
//void CPAL_I2C_DMARXTC_UserCallback(CPAL_InitTypeDef* cpalInitStruct)
//{
//    signed portBASE_TYPE highPriorityWoken;
//    xSemaphoreGiveFromISR(i2cTransferCompleteSemaphore, &highPriorityWoken);
//    if (highPriorityWoken)
//        taskYIELD();
//}

Mi3I2CIrPeripheral::Mi3I2CIrPeripheral()
{
    setUpdateInterval(1000);
    m_previousWakeupTime = 0;
    m_boardInReady = false;
    create("Mi3IrTask", configMINIMAL_STACK_SIZE + 200, 0);

    if (i2cTransferCompleteSemaphore == NULL)
        vSemaphoreCreateBinary(i2cTransferCompleteSemaphore);

    xSemaphoreTake(i2cTransferCompleteSemaphore, 0);

    if (Psc_GetBoardType() == E_BoardType_Fec3)
    {
        m_powerGpioPort = GPIOF;
        m_powerGpioPin = GPIO_Pin_4;
    }
    else
    {
        m_powerGpioPort = NULL;
        m_powerGpioPin = 0xffff;
    }

    m_performReset = true;

    powerEnable();

    m_resetCount = ElementRepository::getInstance().addElementU32();

    m_updateCycleFinishedSem.create();
    m_updateCycleFinishedSem.take(0);
}

Mi3I2CIrPeripheral::~Mi3I2CIrPeripheral()
{
    T_SensorListIterator it;
    for (it = m_sensorList.begin(); it != m_sensorList.end(); ++it)
        delete (*it);
}

void Mi3I2CIrPeripheral::readInputs()
{
    T_SensorListIterator it;
    for (it = m_sensorList.begin(); it != m_sensorList.end(); ++it)
    {
        (*it)->readTargetTemp();
        delay(10);
    }
}

ElementBase* Mi3I2CIrPeripheral::getElementByIndex(int address)
{
    if (address == 0)
        return m_resetCount;

    Mi3Sensor* sensor = getSensorByAddress(address);

    if (sensor != NULL)
        return sensor->getTargTempElement();

    return NULL;
}

void Mi3I2CIrPeripheral::enableElementByIndex(int index, bool enable)
{
}

Mi3Sensor* Mi3I2CIrPeripheral::getSensorByAddress(int address)
{
    // first check that the address is not reserved:
    if ((address & 0x50) == 0x50)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "MI3 IR Sensor address %x(%d) is reserved", address, address);
        return NULL;
    }

    T_SensorListIterator it;
    for (it = m_sensorList.begin(); it != m_sensorList.end(); ++it)
        if ((*it)->getAddress() == address)
            return (*it);

    // if the sensor wasn't found create a new one:
    if (it == m_sensorList.end())
    {
        Mi3Sensor* mi3Sensor = new Mi3Sensor();
        mi3Sensor->setAddress(address);
        m_sensorList.push_back(mi3Sensor);
        return mi3Sensor;
    }
    return NULL;
}

bool Mi3I2CIrPeripheral::configureSensor(int address, SensorConfiguration& sensorConfig)
{
    Mi3Sensor* sensor;

    sensor = getSensorByAddress(address);
    if (sensor == NULL)
        return false;

    return sensor->downloadConfiguraton();
}

void Mi3I2CIrPeripheral::run()
{
    T_SensorListIterator it;
    m_previousWakeupTime = getTickCount();
    bool allSensorsInvalid;
    int allSensorInvalidRetries = 0;

    portBASE_TYPE semResult;

    m_updateCycleFinishedSem.give();

    for (;;)
    {
        semResult = m_updateCycleFinishedSem.take(1000);

        if (m_boardInReady && semResult == pdPASS)
        {
            if (m_performReset)
            {
                m_performReset = false;
                // todo: check if we need to re-initialize the i2c busses.
                powerDisable();
                delay(M_I2C_POWERDOWN_TIME);
                powerEnable();
                delay(M_I2C_POWERUP_TIME);
                allSensorInvalidRetries = 0;
                m_previousWakeupTime = getTickCount();
            }
            allSensorsInvalid = false;

            for (it = m_sensorList.begin(); it != m_sensorList.end(); ++it)
            {
                (*it)->readTargetTemp();
                // set allSensorsInvalid to TRUE if at least one sensor is working.
                allSensorsInvalid |= ((*it)->getTargTempElement()->isValid());
                delay(10);
            }

            if (allSensorsInvalid)
            {
                //if we're here it means at least one sensor is working.
                allSensorInvalidRetries = 0;
            }
            else
            {
                // if we're here it means all sensors are not responding.
                ++allSensorInvalidRetries;

                // if retries are exceeded, we need to reset the I2C bus and increment the failure count.
                if (allSensorInvalidRetries > M_RETRIES_AFTER_ALL_INVALID)
                {
                    m_resetCount->setValue(m_resetCount->getValueU32() + 1);
                    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Resetting I2C power. Reset Count: %d",
                            m_resetCount->getValueU32());
                    m_performReset = true;
                }
            }

            m_updateCycleFinishedSem.give();
        }

        delayUntil(&m_previousWakeupTime, m_updateInterval);
    }
}

void Mi3I2CIrPeripheral::setUpdateInterval(uint16_t updateInterval)
{
    m_updateInterval = updateInterval;
}

ElementBase* Mi3I2CIrPeripheral::getElementByPssId(int pssId)
{
    for (int i = 0; i < m_sensorList.size(); ++i)
    {
        if (m_sensorList[i]->getPssId() == pssId)
            return m_sensorList[i]->getTargTempElement();
    }
    return NULL;
}

long Mi3I2CIrPeripheral::onCreate(const char* const pcName, unsigned short usStackDepth, unsigned long uxPriority)
{
    return pdTRUE;
}

void Mi3I2CIrPeripheral::setBoardInReady(bool state)
{
    m_performReset = state;
    PeripheralBase::setBoardInReady(state);
}

void Mi3I2CIrPeripheral::setResetCountPssId(uint16_t pssId)
{
    m_resetCount->setPssId(pssId);
}

void Mi3I2CIrPeripheral::startRecovery()
{
    m_performReset = true;
}

Mi3I2CIrPeripheral::Mi3I2CIrPeripheral(F_FILE* f)
{
    Serializer<Mi3I2CIrPeripheral> s;
    s.deserialize(f, *this);
}

void Mi3I2CIrPeripheral::serialize(F_FILE* f)
{
    Serializer<Mi3I2CIrPeripheral> s;
    s.serialize(f, *this);
}

void Mi3I2CIrPeripheral::setIsInSerialization(bool isInSerialization)
{
    portBASE_TYPE semResult;

    if (isInSerialization)
    {
        m_updateCycleFinishedSem.take(getUpdateInterval()*10);
//        for (int i = 0; i < 10; ++i)
//        {
//            semResult = m_updateCycleFinishedSem.take(10000);
//            if (semResult == pdPASS)
//                break;
//        }
//        if (semResult == pdFAIL)
//            throw "Cannot halt Mi3 task";
    }
    else
        m_updateCycleFinishedSem.give();
}

