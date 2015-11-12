/*
 * CalculateOnTwoDevicesControl.cpp
 *
 *  Created on: 6 Sep 2015
 *      Author: maora
 */

#include <Controls/CalculateOnTwoDevicesControl.h>
#include <PscServer/PscMasterServer.h>

CalculateOnTwoDevicesControl::CalculateOnTwoDevicesControl()
{
    m_input1 = NULL;
    m_input2 = NULL;
    m_previousInput1 = 0;
    m_previousInput2 = 0;
//    m_calculatedOutput = ElementRepository::getInstance().addValidationElementFloat();
    m_calculatedOutput = NULL;
}

CalculateOnTwoDevicesControl::~CalculateOnTwoDevicesControl()
{
    // TODO Auto-generated destructor stub
}

bool CalculateOnTwoDevicesControl::onInitControl()
{
    endInitControl();
    return true;
}

bool CalculateOnTwoDevicesControl::onMove2Standby()
{
    endMove2Standby();
    return true;
}

bool CalculateOnTwoDevicesControl::onReset2On()
{
    endReset2On();
    return true;
}

bool CalculateOnTwoDevicesControl::onMove2Error()
{
    endMove2Error();
    return true;
}

bool CalculateOnTwoDevicesControl::onStopOnEmr()
{
    endStopOnEmr();
    return true;
}

bool CalculateOnTwoDevicesControl::onRecoverFromEmr()
{
    endRecoverFromEmr();
    return true;
}

void CalculateOnTwoDevicesControl::updateNotification(ElementBase* element)
{
    if (element == m_input1 || element == m_input2)
    {
        m_calculatedOutput->setValueValid(m_input1->isValid() && m_input2->isValid());
        if (m_input1->getValueF() != m_previousInput1 || m_input2->getValueF() != m_previousInput2)
        {
            execute();
            m_previousInput1 = m_input1->getValueF();
            m_previousInput2 = m_input2->getValueF();
        }
    }
}

bool CalculateOnTwoDevicesControl::sendNotification()
{
    // static instance for the master server.
    PscMessageStruct replyMessage;

    replyMessage.header.id.split.id = MSG_ControlStatusNotification;
    replyMessage.payload.pSSControlStatusNotificationMsg.pssId = getPssId();
    replyMessage.payload.pSSControlStatusNotificationMsg.setPoint = m_calculatedOutput->getValueU32();
    replyMessage.payload.pSSControlStatusNotificationMsg.state = m_controlState;
    replyMessage.payload.pSSControlStatusNotificationMsg.exceptions = getControlExceptions();
    replyMessage.payload.pSSControlStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSControlStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);

    return true;
}
