/*
 * AnalogLiquidLevelControl.cpp
 *
 *  Created on: 1 בדצמ 2013
 *      Author: maora
 */

#include "AnalogLiquidLevelControl.h"
#include <PscSubsystem.h>
#include <Elements/ElementRepository.h>
#include <PscServer/PscMasterServer.h>
#include <Persistency/ControlSerializers.h>

AnalogLiquidLevelControl::AnalogLiquidLevelControl()
{
    m_levelSensorElement = NULL;
    m_calculatedOutputLevel = ElementRepository::getInstance().addElementI32();
    m_highLevelValue = 0;
    m_lowLevelValue = 0;
    m_midLevelValue = 0;
}

AnalogLiquidLevelControl::~AnalogLiquidLevelControl()
{
    m_levelSensorElement->removeObserver(this);
}

void AnalogLiquidLevelControl::execute()
{
    float currentLevel = m_levelSensorElement->getValueF();
    int calculatedOutputLevel;

    if (currentLevel < m_lowLevelValue)
    {
        calculatedOutputLevel = 0;
//        raiseError(E_PSSErrors_DeviceExceedsSoftLimits, true);
//        raiseWarning(E_PSSWarnings_DeviceExceedsLimits, false);
    }
    else if (currentLevel < m_midLevelValue)
    {
        calculatedOutputLevel = 1;
//        raiseError(E_PSSErrors_DeviceExceedsSoftLimits, false);
//        raiseWarning(E_PSSWarnings_DeviceExceedsLimits, true);
    }
    else if (currentLevel < m_highLevelValue)
    {
        calculatedOutputLevel = 2;
//        raiseError(E_PSSErrors_DeviceExceedsSoftLimits, false);
//        raiseWarning(E_PSSWarnings_DeviceExceedsLimits, false);
    }
    else
    {
        calculatedOutputLevel = 3;
//        raiseError(E_PSSErrors_DeviceExceedsSoftLimits, true);
//        raiseWarning(E_PSSWarnings_DeviceExceedsLimits, false);
    }
    if (calculatedOutputLevel != m_calculatedOutputLevel->getValueI32())
    {
        m_calculatedOutputLevel->setValue(calculatedOutputLevel);
        sendNotification();
    }
}

bool AnalogLiquidLevelControl::onInitControl()
{
    execute();
    endInitControl();
    return true;
}

bool AnalogLiquidLevelControl::onMove2Standby(uint32_t delay)
{
    execute();
    endMove2Standby();
    return true;
}

bool AnalogLiquidLevelControl::onReset2On()
{
    execute();
    endReset2On();
    return true;
}

void AnalogLiquidLevelControl::updateNotification(ElementBase* element)
{
    // since this class only reads devices and calculates an outputvalue, there's no "state"
    // in which we not executing.
    execute();

    if (m_controlState == E_ControlState_Move2Ready)
    {
        m_controlState = E_ControlState_Ready;
        sendNotification();
    }
}

bool AnalogLiquidLevelControl::sendNotification()
{
    // static instance for the master server.
    PscMessageStruct replyMessage;

    replyMessage.header.id.split.id = MSG_ControlStatusNotification;
    replyMessage.payload.pSSControlStatusNotificationMsg.pssId = getPssId();
    replyMessage.payload.pSSControlStatusNotificationMsg.setPoint = m_calculatedOutputLevel->getValueU32();
    replyMessage.payload.pSSControlStatusNotificationMsg.dataType = E_ValueType_U32;
    replyMessage.payload.pSSControlStatusNotificationMsg.state = m_controlState;
    replyMessage.payload.pSSControlStatusNotificationMsg.exceptions = getControlExceptions();
    replyMessage.payload.pSSControlStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSControlStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);

    return true;
}

bool AnalogLiquidLevelControl::onStopOnEmr()
{
    execute();
    endStopOnEmr();
    return true;
}

bool AnalogLiquidLevelControl::onRecoverFromEmr()
{
    execute();
    endRecoverFromEmr();
    return true;
}

void AnalogLiquidLevelControl::setMinInterval(int16_t minInterval)
{
    ControlBase::setMinInterval(minInterval);
    m_calculatedOutputLevel->setMinInterval(minInterval);
}

//void AnalogLiquidLevelControl::addProtectionElement(ValidationElementBase* element)
//{
//}
//
void AnalogLiquidLevelControl::setLevelThresholds(float low, float mid, float high)
{
    m_lowLevelValue = low;
    m_midLevelValue = mid;
    m_highLevelValue = high;
}

bool AnalogLiquidLevelControl::onMove2Error()
{
    execute();
    endMove2Error();
    return true;
}

void AnalogLiquidLevelControl::serialize(F_FILE* f)
{
    Serializer<AnalogLiquidLevelControl> s;
    s.serialize(f, *this);
}

AnalogLiquidLevelControl::AnalogLiquidLevelControl(F_FILE* f)
{
    Serializer<AnalogLiquidLevelControl> s;
    s.deserialize(f, *this);
    setElementLevelSensor(m_levelSensorElement);
}

