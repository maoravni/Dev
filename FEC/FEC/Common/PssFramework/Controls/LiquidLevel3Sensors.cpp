/*
 * LiquidLevel3Sensors.cpp
 *
 *  Created on: 1 בדצמ 2013
 *      Author: maora
 */

#include "LiquidLevel3Sensors.h"
#include <PscSubsystem.h>
#include <Elements/ElementRepository.h>
#include <PscServer/PscMasterServer.h>

LiquidLevel3Sensors::LiquidLevel3Sensors()
{
    m_lowSensor = NULL;
    m_midSensor = NULL;
    m_highSensor = NULL;
    m_calculatedOutputLevel = ElementRepository::getInstance().addElementI32();
}

LiquidLevel3Sensors::~LiquidLevel3Sensors()
{
    m_highSensor->removeObserver(this);
    m_lowSensor->removeObserver(this);
    m_midSensor->removeObserver(this);
}

void LiquidLevel3Sensors::execute()
{
    int level = (m_highSensor->getValueI32() << 2 | m_midSensor->getValueI32() << 1 | m_lowSensor->getValueI32());

    switch (level)
    {
    case 7:
        *m_calculatedOutputLevel = 0;
        break;
    case 3:
        *m_calculatedOutputLevel = 1;
        break;
    case 1:
        *m_calculatedOutputLevel = 2;
        break;
    case 0:
        *m_calculatedOutputLevel = 3;
        break;
    default:
        //TODO: issue an error about an invalid sensor state.
        break;
    }
}

bool LiquidLevel3Sensors::onInitControl()
{
    execute();
    endInitControl();
    return true;
}

bool LiquidLevel3Sensors::onMove2Standby()
{
    execute();
    endMove2Standby();
    return true;
}

bool LiquidLevel3Sensors::onReset2On()
{
    execute();
    endReset2On();
    return true;
}

bool LiquidLevel3Sensors::onMove2Error()
{
    execute();
    endMove2Error();
    return true;
}

void LiquidLevel3Sensors::updateNotification(ElementBase* element)
{
    // since this class only reads devices and calculates an outputvalue, there's no "state"
    // in which we not executing.
    execute();

    if (m_controlState == E_ControlState_Move2Ready)
    {
        m_controlState = E_ControlState_Ready;
    }
    sendNotification();
}

bool LiquidLevel3Sensors::onStopOnEmr()
{
    execute();
    endStopOnEmr();
    return true;
}

bool LiquidLevel3Sensors::onRecoverFromEmr()
{
    execute();
    endRecoverFromEmr();
    return true;
}

bool LiquidLevel3Sensors::sendNotification()
{
    // static instance for the master server.
    PscMessageStruct replyMessage;

    replyMessage.header.id.split.id = MSG_ControlStatusNotification;
    replyMessage.payload.pSSControlStatusNotificationMsg.pssId = getPssId();
    replyMessage.payload.pSSControlStatusNotificationMsg.setPoint = m_calculatedOutputLevel->getValueU32();
    replyMessage.payload.pSSControlStatusNotificationMsg.state = m_controlState;
    replyMessage.payload.pSSControlStatusNotificationMsg.exceptions = getControlExceptions();
    replyMessage.payload.pSSControlStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSControlStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);

    return true;
}

//void LiquidLevel3Sensors::addProtectionElement(ValidationElementBase* element)
//{
//}
