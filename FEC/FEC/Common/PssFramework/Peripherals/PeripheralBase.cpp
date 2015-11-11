/*
 * DeviceBase.cpp
 *
 *  Created on: 9 Jun 2013
 *      Author: maora
 */

#include "PeripheralBase.h"
#include <Tasks/UpdateSchedulerTask.h>
#include "PeripheralRepository.h"

PeripheralBase::PeripheralBase()
{
    m_pssId = 0;
    m_peripheralRepIndex = 0;
    m_updateInterval = 0;
    m_boardInReady = false;
//    PeripheralRepository::getInstance().addPeripheral(this);
}

uint16_t PeripheralBase::getPssId() const
{
    return m_pssId;
}

void PeripheralBase::setPssId(uint16_t deviceId)
{
    m_pssId = deviceId;
    setUpdateInterval(m_updateInterval);

}

uint16_t PeripheralBase::getPeripheralRepIndex() const
{
    return m_peripheralRepIndex;
}

uint16_t PeripheralBase::getUpdateInterval() const
{
    return m_updateInterval;
}

void PeripheralBase::setUpdateInterval(uint16_t updateInterval)
{
    uint16_t oldInterval = m_updateInterval;
    m_updateInterval = updateInterval;

    // if the old update interval is 0 and the new update interval != 0,
    // we need to insert this device to the update scheduler:
    if (/*oldInterval == 0 && */updateInterval != 0)
        getSchedulerTask()->addDevice(this);
}

void PeripheralBase::setPeripheralRepIndex(uint16_t deviceRepIndex)
{
    m_peripheralRepIndex = deviceRepIndex;
}

PeripheralBase::~PeripheralBase()
{
}

UpdateSchedulerTaskBase* PeripheralBase::getSchedulerTask()
{
    return UpdateSchedulerTask::getInstance();
}

void PeripheralBase::setBoardInReady(bool state)
{
    m_boardInReady = state;
}
