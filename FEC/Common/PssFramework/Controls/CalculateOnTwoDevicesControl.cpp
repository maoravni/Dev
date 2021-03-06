/*
 * CalculateOnTwoDevicesControl.cpp
 *
 *  Created on: 6 Sep 2015
 *      Author: maora
 */

#include <Controls/CalculateOnTwoDevicesControl.h>
#include <PscServer/PscMasterServer.h>
#include <Persistency/ControlSerializers.h>

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

bool CalculateOnTwoDevicesControl::onMove2Standby(uint32_t delay)
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
    replyMessage.payload.pSSControlStatusNotificationMsg.dataType = E_ValueType_Float;
    replyMessage.payload.pSSControlStatusNotificationMsg.setPoint = m_calculatedOutput->getValueU32();
    replyMessage.payload.pSSControlStatusNotificationMsg.state = m_controlState;
    replyMessage.payload.pSSControlStatusNotificationMsg.exceptions = getControlExceptions();
    replyMessage.payload.pSSControlStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSControlStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);

    return true;
}

SubtractTwoDevicesControl::SubtractTwoDevicesControl()
{
}

SubtractTwoDevicesControl::~SubtractTwoDevicesControl()
{
}

void SubtractTwoDevicesControl::execute()
{
    float input1 = m_input1->getValueF();
    float input2 = m_input2->getValueF();
    float result = input1 - input2;
    m_calculatedOutput->setValue(result);
}

AddTwoDevicesControl::AddTwoDevicesControl()
{
}

AddTwoDevicesControl::~AddTwoDevicesControl()
{
}

void AddTwoDevicesControl::execute()
{
    float input1 = m_input1->getValueF();
    float input2 = m_input2->getValueF();
    float result = input1 + input2;
    m_calculatedOutput->setValue(result);
}

MultiplyTwoDevicesControl::MultiplyTwoDevicesControl()
{
}

MultiplyTwoDevicesControl::~MultiplyTwoDevicesControl()
{
}

void MultiplyTwoDevicesControl::execute()
{
    float input1 = m_input1->getValueF();
    float input2 = m_input2->getValueF();
    float result = input1 * input2;
    m_calculatedOutput->setValue(result);
}

DivideTwoDevicesControl::DivideTwoDevicesControl()
{
}

DivideTwoDevicesControl::~DivideTwoDevicesControl()
{
}

void DivideTwoDevicesControl::execute()
{
    float input1 = m_input1->getValueF();
    float input2 = m_input2->getValueF();
    float result = input1 / input2;
    m_calculatedOutput->setValue(result);
}

MaxTwoDevicesControl::MaxTwoDevicesControl()
{
}

MaxTwoDevicesControl::~MaxTwoDevicesControl()
{
}

void MaxTwoDevicesControl::execute()
{
    float input1 = m_input1->getValueF();
    float input2 = m_input2->getValueF();
    float result = (input1 > input2) ? input1 : input2;
    m_calculatedOutput->setValue(result);
}

MinTwoDevicesControl::MinTwoDevicesControl()
{
}

MinTwoDevicesControl::~MinTwoDevicesControl()
{
}

void MinTwoDevicesControl::execute()
{
    float input1 = m_input1->getValueF();
    float input2 = m_input2->getValueF();
    float result = (input1 < input2) ? input1 : input2;
    m_calculatedOutput->setValue(result);
}

void MinTwoDevicesControl::serialize(F_FILE* f)
{
    Serializer<MinTwoDevicesControl> s;
    s.serialize(f, *this);
}

MinTwoDevicesControl::MinTwoDevicesControl(F_FILE* f)
{
    Serializer<MinTwoDevicesControl> s;
    s.deserialize(f, *this);
    setInput1Element(m_input1);
    setInput2Element(m_input1);
}

void DivideTwoDevicesControl::serialize(F_FILE* f)
{
    Serializer<DivideTwoDevicesControl> s;
    s.serialize(f, *this);
}

DivideTwoDevicesControl::DivideTwoDevicesControl(F_FILE* f)
{
    Serializer<DivideTwoDevicesControl> s;
    s.deserialize(f, *this);
    setInput1Element(m_input1);
    setInput2Element(m_input1);
}

void AddTwoDevicesControl::serialize(F_FILE* f)
{
    Serializer<AddTwoDevicesControl> s;
    s.serialize(f, *this);
}

AddTwoDevicesControl::AddTwoDevicesControl(F_FILE* f)
{
    Serializer<AddTwoDevicesControl> s;
    s.deserialize(f, *this);
    setInput1Element(m_input1);
    setInput2Element(m_input1);
}

void MaxTwoDevicesControl::serialize(F_FILE* f)
{
    Serializer<MaxTwoDevicesControl> s;
    s.serialize(f, *this);
}

MaxTwoDevicesControl::MaxTwoDevicesControl(F_FILE* f)
{
    Serializer<MaxTwoDevicesControl> s;
    s.deserialize(f, *this);
    setInput1Element(m_input1);
    setInput2Element(m_input1);
}

void MultiplyTwoDevicesControl::serialize(F_FILE* f)
{
    Serializer<MultiplyTwoDevicesControl> s;
    s.serialize(f, *this);
}

MultiplyTwoDevicesControl::MultiplyTwoDevicesControl(F_FILE* f)
{
    Serializer<MultiplyTwoDevicesControl> s;
    s.deserialize(f, *this);
    setInput1Element(m_input1);
    setInput2Element(m_input1);
}

void SubtractTwoDevicesControl::serialize(F_FILE* f)
{
    Serializer<SubtractTwoDevicesControl> s;
    s.serialize(f, *this);
}

SubtractTwoDevicesControl::SubtractTwoDevicesControl(F_FILE* f)
{
    Serializer<SubtractTwoDevicesControl> s;
    s.deserialize(f, *this);
    setInput1Element(m_input1);
    setInput2Element(m_input1);
}

