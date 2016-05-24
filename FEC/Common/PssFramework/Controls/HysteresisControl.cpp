/*
 * HysteresisControl.cpp
 *
 *  Created on: 11 Jun 2013
 *      Author: maora
 */

#include <PscServer/PscMasterServer.h>
#include "HysteresisControl.h"
#include "ControlRepository.h"
#include <PscSubsystem.h>
#include <logger.h>
#include <Persistency/ControlSerializers.h>

HysteresisControl::HysteresisControl()
{
    m_input = NULL;
    m_output = NULL;
    setElementActivateSetpoint(ElementRepository::getInstance().addValidationElementFloat());
    setElementDeactivateSetpoint(ElementRepository::getInstance().addValidationElementFloat());
    m_coolingMode = false;
    m_activateSetpoint = 0;
    m_deactivateSetpoint = 0;
//    m_isProtectionActive = false;
//    m_fromMove2Standby = false;
}

HysteresisControl::~HysteresisControl()
{
    m_input->removeObserver(this);
    m_activateSetpointElement->removeObserver(this);
    m_deactivateSetpointElement->removeObserver(this);
}

void HysteresisControl::updateNotification(ElementBase* element)
{
    if (element == m_output)
    {
        raiseError(element->getPssId(), E_PSSErrors_SensorMalfunction, !m_output->isValid());
        if (!m_output->isValid())
            move2Error(MSG_ActivatePIDControl, m_lastSn);
        return;
    }

    if (m_controlState == E_ControlState_On)
        return;

    if (m_controlState >= E_ControlState_Standby && m_controlState <= E_ControlState_Ready)
    {
        if (element == m_input)
        {
            execute();
            return;
        }
        if (executeProtectionCheck(element/*, E_PSSErrors_DeviceExceedsSoftLimits*/))
        {
            // TODO: Change error to "Protection Activated"
//            raiseError(E_PSSErrors_DeviceExceedsSoftLimits, true);
            // we should also stop the heating:
            m_output->setValue(0);
            m_output->sendDeviceStatus();
        }
//        else
//        {
//            raiseError(E_PSSErrors_DeviceExceedsSoftLimits, false);
//        }
    }
    if (element == m_activateSetpointElement || element == m_deactivateSetpointElement)
    {
        setSetpoint(m_activateSetpointElement, m_deactivateSetpointElement);
        return;
    }

}

void HysteresisControl::executeControl(float inputValue)
{
    if (m_coolingMode)
    {
        if (inputValue > m_activateSetpointElement->getValue())
        {
            if (!m_isProtectionActive)
                m_output->setValue(m_outputValue);
        }
        if (inputValue <= m_deactivateSetpointElement->getValue())
            m_output->setValue(0);
    }
    else
    {
        if (inputValue < m_activateSetpointElement->getValue())
        {
            if (!m_isProtectionActive)
                m_output->setValue(m_outputValue);
        }
        if (inputValue >= m_deactivateSetpointElement->getValue())
            m_output->setValue(0);
    }
}

//void HysteresisControl::executeProtectionCheck(ElementBase* element)
//{
//    // Protection elements should stop heating if temp is above max working, and continue when below min working.
//
//    // if the protection is not active, we can simply check the updated element if it should
//    // turn on the protection:
//    if (!m_isProtectionActive)
//    {
//        // if the temp is above protection limit, turn on the protection.
//        if (element->isOutsideSoftLimit())
//        {
//            m_isProtectionActive = true;
//            // TODO: Change error to "Protection Activated"
//            raiseError(E_PSSErrors_TemperatureExceedsLimits, true);
//            // we should also stop the heating:
//            *m_output = 0;
//            m_output->sendDeviceStatus();
//        }
//    }
//    // the protection is active, so we should check all protection elements to see if they are below limit:
//    else
//    {
//        int i;
//        bool safe = true;
//        for (i = 0; i < m_protectionElements.size(); ++i)
//        {
//            safe = (safe && m_protectionElements[i]->isInsideLimit());
//        }
//        if (safe)
//        {
//            m_isProtectionActive = false;
//            // TODO: Change error to "Protection Activated"
//            raiseError(E_PSSErrors_TemperatureExceedsLimits, false);
//        }
//    }
//}

void HysteresisControl::execute()
{
    float inputValue = m_input->getValue();

    // if protection is not active, set the output result to the output element:
    if (m_isProtectionActive)
        m_output->setValue(0);

    bool outputValue = false;
    switch (m_controlState)
    {
    case E_ControlState_Move2Ready:
        if (m_input->isValid())
        {
            executeControl(inputValue);

            if (m_coolingMode)
            {
                if (inputValue < m_activateSetpointElement->getValue())
                    m_controlState = E_ControlState_Ready;
            }
            else
            {
                if (inputValue > m_activateSetpointElement->getValue())
                    m_controlState = E_ControlState_Ready;
            }
            // TODO: add a timeout for reaching the set point
            if (m_controlState == E_ControlState_Ready)
            {
                m_controlState = E_ControlState_Ready;
                sendNotification();
                if (m_lastSn != 0)
                    PscMasterServer::getInstance().sendSeqEnded(MSG_ActivateHysteresisTemperatureControl, m_lastSn,
                            Psc_ControllerId, getPssId(), E_SeqEndedStatus_Success);
            }
        }
        break;
    case E_ControlState_Ready:
        executeControl(inputValue);

        // TODO: Check working range and warning range, and issue an error
        if (!m_input->isValid())
        {
            m_output->setValue(0);
        }
        else
        {
            if (m_activateSetpointElement->isInWorkingRange(inputValue))
            {
                if (getControlExceptions() != 0)
                {
                    raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
                    raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);
                }
            }
            else
            {
                if ((inputValue < m_activateSetpointElement->getMinWorking())
                        || (inputValue >= m_activateSetpointElement->getMaxWorking()))
                {
                    raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, true);
                }
                else
                {
                    raiseError(0, E_PSSErrors_ControlExceedsLimits, true);
                }
            }
        }
        break;
    }
}

bool HysteresisControl::sendNotification()
{
    // static instance for the master server.
    PscMessageStruct replyMessage;

    replyMessage.header.id.split.id = MSG_ControlStatusNotification;
    replyMessage.payload.pSSControlStatusNotificationMsg.pssId = getPssId();
    replyMessage.payload.pSSControlStatusNotificationMsg.dataType = E_ValueType_Float;
    replyMessage.payload.pSSControlStatusNotificationMsg.setPoint = m_deactivateSetpointElement->getValue();
    replyMessage.payload.pSSControlStatusNotificationMsg.state = m_controlState;
    replyMessage.payload.pSSControlStatusNotificationMsg.exceptions = getControlExceptions();
    replyMessage.payload.pSSControlStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSControlStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);

    return true;
}

bool HysteresisControl::setSetpoint(float actSp, float deactSp, float loRange, float hiRange, float loWarn,
        float hiWarn, uint32_t sn)
{
    if (m_controlState == E_ControlState_On)
        return false;

    // change the state
    // TODO: Implement a state machine, so that seq ended sending will be encapsulated.
    m_controlState = E_ControlState_Move2Ready;

    // if the activation setpoint is higher than the deactivation setpoint, we're in cooling mode.
    m_coolingMode = false;
    if (actSp > deactSp)
    {
        m_coolingMode = true;
    }

    // set the ranges to the input:
    if (m_activateSetpointElement->getValueF() != actSp)
    {
        m_activateSetpointElement->updateWorkingRange(actSp + loRange, deactSp + hiRange, true, true);
        m_activateSetpointElement->updateWarningRange(actSp + loWarn, deactSp + hiWarn, true, true);
        m_activateSetpointElement->setValue(actSp);
        m_activateSetpoint = actSp;
    }
    if (m_deactivateSetpointElement->getValueF() != deactSp)
    {
        m_deactivateSetpointElement->updateWorkingRange(actSp + loRange, deactSp + hiRange, true, true);
        m_deactivateSetpointElement->updateWarningRange(actSp + loWarn, deactSp + hiWarn, true, true);
        m_deactivateSetpointElement->setValue(deactSp);
        m_deactivateSetpoint = deactSp;
    }

    sendNotification();

    m_lastSn = sn;

    return true;
}

bool HysteresisControl::setSetpoint(ValidationElementFloat* activateElement, ValidationElementFloat* deactivateElement)
{
    float actVal = activateElement->getValue();
    float deactVal = deactivateElement->getValue();

    if (actVal == m_activateSetpoint && deactVal == m_deactivateSetpoint)
        return true;

    float minRange, maxRange, minWarn, maxWarn;

    // Check if the values are correct - activate is lower than deactivate.
    if (actVal < deactVal)
    {
        minRange = actVal - 5;
        maxRange = deactVal + 5;
        minWarn = actVal - 10;
        maxWarn = deactVal + 10;
    }
    else
    {
        minRange = deactVal - 5;
        maxRange = actVal + 5;
        minWarn = deactVal - 10;
        maxWarn = actVal + 10;
    }
    return setSetpoint(actVal, deactVal, minRange, maxRange, minWarn, maxWarn, 0);
    //return setSetpoint(actVal, deactVal, actVal - 5, deactVal + 5, actVal - 10, deactVal + 10, 0);
//    return true;
}

bool HysteresisControl::onMove2Standby(uint32_t delay)
{
    if (m_output->getValueF() != 0)
        m_output->setValue(0);
    m_output->sendDeviceStatus();
    endMove2Standby();
    return true;
}

bool HysteresisControl::onReset2On()
{
    if (m_output->getValueF() != 0)
        m_output->setValue(0);
    m_output->sendDeviceStatus();
    endReset2On();
    return true;
}

bool HysteresisControl::onInitControl()
{
    endInitControl();
    return true;
}

bool HysteresisControl::onMove2Error()
{
    if (m_output->getValueF() != 0)
        m_output->setValue(0);
    m_output->sendDeviceStatus();
    endMove2Error();
    return true;
}

#ifdef OLD_PROTECTION
void HysteresisControl::addProtectionElement(ValidationElementBase* element)
{
    if (element == m_input)
    {
        m_inputIsInProtection = true;
    }
    ControlBase::addProtectionElement(element);
}
#endif

bool HysteresisControl::onStopOnEmr()
{
    if (m_output->getValueF() != 0)
        m_output->setValue(0);
    m_output->sendDeviceStatus();
    endStopOnEmr();
    return true;
}

bool HysteresisControl::onRecoverFromEmr()
{
    endRecoverFromEmr();
    return true;
}

bool HysteresisControl::setSetpoint(float setpoint, uint32_t sn)
{
    if (m_coolingMode)
    {
        return setSetpoint(setpoint + 1, setpoint, setpoint - 5, setpoint + 5, setpoint - 10, setpoint + 10, sn);
    }
    else
    {
        return setSetpoint(setpoint - 1, setpoint, setpoint - 5, setpoint + 5, setpoint - 10, setpoint + 10, sn);
    }
}

E_ActivationState HysteresisControl::getActivationState()
{
    if (m_controlState == E_ControlState_Move2Ready || m_controlState == E_ControlState_Ready)
        return E_ActivationState_Active;
    else
        return E_ActivationState_Inactive;
}

void HysteresisControl::serialize(F_FILE* f)
{
    Serializer<HysteresisControl> s;
    s.serialize(f, *this);
}

HysteresisControl::HysteresisControl(F_FILE* f)
{
    Serializer<HysteresisControl> s;
    s.deserialize(f, *this);
}

