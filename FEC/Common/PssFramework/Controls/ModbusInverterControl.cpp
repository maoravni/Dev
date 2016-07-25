/*
 * ModbusInverterControl.cpp
*
*  Created on: 10 בספט 2013
*      Author: maora
*/

#include "ModbusInverterControl.h"
#include <PscServer/PscMasterServer.h>
#include <PscSubsystem.h>
#include <Peripherals/PeripheralRepository.h>
#include "logger.h"
#include <Tasks/UpdateSchedulerTask.h>
#include <Persistency/ControlSerializers.h>

#define M_TIMEOUT_STOP 0
#define M_TIMEOUT_DELAY 1
#define M_TIMEOUT_ACTIVATE 2
#define M_DEFAULT_ACTIVATION_TIMEOUT 60000

ModbusInverterControl::ModbusInverterControl()
{
	m_enableOutput = NULL;
	m_outputCurrent = NULL;
	m_outputFrequency = NULL;
	m_setpoint = NULL;
	m_driveStatus = NULL;
	m_stopping = false;
	m_requestedSetpoint = ElementRepository::getInstance().addValidationElementFloat();
	m_lastSn = 0;
	m_isProtectionActive = false;
	//    m_setpoint = ElementRepository::getInstance().addValidationElementFloat();
	//    m_setpoint->addObserver(this);

}

ModbusInverterControl::~ModbusInverterControl()
{
	//    *m_enableOutput = 0;
	//    *m_setpoint = 0;
	//    vTaskDelay(100);
	m_setpoint->removeObserver(this);
	m_outputFrequency->removeObserver(this);
	m_outputFrequency->removeObserver(this);
}

void ModbusInverterControl::startRecovery()
{
	raiseError(0, E_PSSErrors_ActivationFailed, false);
	raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
	raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);
}

void ModbusInverterControl::updateSetpoints()
{
	// set the ranges to the input:
	if (m_setpoint->getValueF() != m_requestedSetpoint->getValueF())
	{
		m_setpoint->setValue(m_requestedSetpoint->getValueF());
	}
	m_enableOutput->setValue((m_requestedSetpoint->getValueF() != 0 && !m_isProtectionActive) ? 1 : 0);
	//    if (m_lastSn != 0)
	//        ModbusSchedulerTask::getInstance()->addTimeout(this, M_DEFAULT_ACTIVATION_TIMEOUT, M_TIMEOUT_ACTIVATE);
}

bool ModbusInverterControl::setSetpointActivationDelay(float value, uint32_t activationDelay)
{
	if (/*m_stopping || */m_controlState == E_ControlState_On || m_controlState == E_ControlState_Error)
		return false;

	// if the old requested setpoint equals the new requested setpoint don't do anything.
	//    if (m_requestedSetpoint == value && m_controlState == E_ControlState_Standby)
	//        return true;

	// change the state
	// TODO: Implement a state machine, so that seq ended sending will be encapsulated.
	//if (!m_stopping)

    taskENTER_CRITICAL();

	m_controlState = E_ControlState_Move2Ready;
	m_stopping = false;

	raiseError(0, E_PSSErrors_ActivationFailed, false);

	// remove current timeouts if they exist:
	ModbusSchedulerTask::getInstance()->addTimeout(this, 0, M_TIMEOUT_DELAY);
	ModbusSchedulerTask::getInstance()->addTimeout(this, 0, M_TIMEOUT_STOP);

	m_requestedSetpoint->setValue(value);
	m_requestedSetpoint->updateWorkingRange(value - 1, value + 1, true, true);
	m_requestedSetpoint->updateWarningRange(value - 2, value + 2, true, true);

	if (activationDelay == 0)
	{
		// set the ranges to the input:
		updateSetpoints();
	}
	else
	{
		ModbusSchedulerTask::getInstance()->addTimeout(this, activationDelay, M_TIMEOUT_DELAY);
	}

	taskEXIT_CRITICAL();

	sendNotification();

	return true;
}

bool ModbusInverterControl::setSetpointSnActivationDelay(float value, uint32_t sn, uint32_t activationDelay)
{
	m_lastSn = sn;
	//    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "m_lastSn=%d", m_lastSn);
	return setSetpointActivationDelay(value, activationDelay);
}

bool ModbusInverterControl::setSetpoint(float value, uint32_t sn)
{
	m_lastSn = sn;
	//    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "m_lastSn=%d", m_lastSn);
	return setSetpointActivationDelay(value, 0);
}

void ModbusInverterControl::setOutputEnableElement(ElementBase* element)
{
	m_enableOutput = element;
}

void ModbusInverterControl::setInverterPeripheral(ModbusInverterPeripheralBase* inverter)
{
	//    m_setpoint = inverter->getFrequencySetpointElement();
    setOutputCurrentElement(inverter->getOutputCurrentElement());
    setOutputFrequencyElement(inverter->getOutputFrequencyElement());
	//m_setpoint->addObserver(this);
	//    m_outputCurrent->addObserver(this);
}

void ModbusInverterControl::setOutputFrequencyElement(ElementBase* element)
{
	m_outputFrequency = element;
	m_outputFrequency->addObserver(this);
	m_outputFrequency->setMissingDevicePriority(E_MissingDevicePriority_High);

	PeripheralBase* periph = PeripheralRepository::getInstance().getPeripheralContainingElementPssId(element->getPssId());

    if (periph != NULL && periph->getPeripheralType() == E_PeripheralType_Inverter && m_enableOutput != NULL)
    {
        dynamic_cast<ModbusInverterPeripheralBase*>(periph)->setOutputEnableElement(m_enableOutput);
    }
}

void ModbusInverterControl::setOutputCurrentElement(ElementBase* element)
{
	m_outputCurrent = element;
	//    m_outputCurrent->addObserver(this);
}

void ModbusInverterControl::setOutputSetpointElement(ElementBase* element)
{
	m_setpoint = element;
	m_setpoint->addObserver(this);
}

void ModbusInverterControl::execute()
{
	switch (m_controlState)
	{
	case E_ControlState_On:
	case E_ControlState_Standby:
		// when we're on standby, we want the blower to stop, so we set the setpoint to "0".
		// if the blower is stopped, we need to lower the output so the motor would not get fried.
		if (m_outputFrequency->getValueF() != 0)
		{
			m_setpoint->setValue(0);
			m_requestedSetpoint->setValue((int)0);
		}
		else
		{
			if (m_enableOutput->getValueU32())
				m_enableOutput->setValue(0);
		}
		if (m_stopping && m_outputFrequency->getValueI32() == 0)
		{
			m_stopping = false;
			//            endMove2Standby();
		}
		break;
	case E_ControlState_Move2Ready:
		if (((int)(m_outputFrequency->getValueF() * 10)) == ((int)(m_requestedSetpoint->getValueF() * 10)))
		{
			m_controlState = E_ControlState_Ready;
			sendNotification();
			//            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "m_lastSn=%d", m_lastSn);
			if (m_lastSn != 0)
			{
				M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "Inverter {[PSSID:%d]} changed state to Ready", getPssId());
				PscMasterServer::getInstance().sendSeqEnded(MSG_ActivateInverterControl, m_lastSn, Psc_ControllerId,
					getPssId(), E_SeqEndedStatus_Success);
				m_lastSn = 0;
			}
		}
		break;
	case E_ControlState_Ready:
		// TODO: Check output current and issue an error when required.
		// if there's already some errors or warnings, this might remove them, so we remove the exceeded limit
		// errors and warnings:
		float inputValue = m_outputFrequency->getValueF();

		if (m_requestedSetpoint->isInWorkingRange(inputValue))
		{
			if (getControlExceptions() != 0)
			{
				raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
				raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);
			}
		}
		else
		{
			if (m_requestedSetpoint->isInWarningRange(inputValue))
			{
				raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
				raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, true);
			}
			else
			{
				raiseError(0, E_PSSErrors_ControlExceedsLimits, true);
				raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);
			}
		}
		break;
	}
}

void ModbusInverterControl::updateNotification(ElementBase* element)
{
	if (!m_outputFrequency->isValid() || !m_outputCurrent->isValid()
		|| (m_outputFrequency->getErrors() & E_PSSErrors_InverterInLocalMode))
	{
		//        m_controlState = E_ControlState_Error;
		//        sendNotification();
		move2Error(MSG_ActivateInverterControl, m_lastSn);
	}
	else
	{
		if (m_controlState == E_ControlState_Error)
		{
			// todo: call Reset2On.
			//            m_controlState = E_ControlState_On;
			//            sendNotification();
			reset2On(0, 0);
		}
	}

	if (element == m_outputFrequency)
	{
		execute();
	}
	else if (element == m_setpoint)
	{
	    if (m_requestedSetpoint->getValueF() != m_setpoint->getValueF())
	    {
//	        if (m_setpoint->getValueF() == 0)
	        setSetpointActivationDelay(m_setpoint->getValueF(), 0);
	    }
	}
	else if (element == m_driveStatus)
	{
	}
	else
	{
		// if it's not the input and not the setpoint, then it must be the protection.
		//      so instead of checking which one is it, we simply check the element
		//      we don't observe the value of the output element, so we can't receive events on it.
		if (executeProtectionCheck(element/*, E_PSSErrors_DeviceExceedsSoftLimits*/))
		{
			//            raiseError(E_PSSErrors_DeviceExceedsSoftLimits, true);
			// we should also stop the inverter
			m_enableOutput->setValue(0);
			m_enableOutput->sendDeviceStatus();
		}
		else
		{
			m_enableOutput->setValue((m_requestedSetpoint != 0) ? 1 : 0);
			m_enableOutput->sendDeviceStatus();
			//            raiseError(E_PSSErrors_DeviceExceedsSoftLimits, false);
		}
	}
}

bool ModbusInverterControl::sendNotification()
{
	// static instance for the master server.
	PscMessageStruct replyMessage;

	replyMessage.header.id.split.id = MSG_ControlStatusNotification;
	replyMessage.payload.pSSControlStatusNotificationMsg.pssId = getPssId();
	replyMessage.payload.pSSControlStatusNotificationMsg.dataType = E_ValueType_Float;
	replyMessage.payload.pSSControlStatusNotificationMsg.setPoint = m_setpoint->getValueF();
	replyMessage.payload.pSSControlStatusNotificationMsg.state = m_controlState;
	replyMessage.payload.pSSControlStatusNotificationMsg.exceptions = getControlExceptions();
	replyMessage.payload.pSSControlStatusNotificationMsg.cableId = Psc_ControllerId;
	replyMessage.header.length = sizeof(replyMessage.header)
		+ sizeof(replyMessage.payload.pSSControlStatusNotificationMsg);

	PscMasterServer::getInstance().sendMessage(replyMessage);

	return true;
}

//void ModbusInverterControl::addProtectionElement(ValidationElementBase* element)
//{
//    m_protectionElements.push_back(element);
//    element->addObserver(this);
//}

bool ModbusInverterControl::onInitControl()
{
	//    *m_enableOutput = 1;
	execute();
	endInitControl();
	return true;
}

bool ModbusInverterControl::onMove2Standby(uint32_t delay)
{
	if (delay == 0)
		return onMove2Standby();
	else
	{
		ModbusSchedulerTask::getInstance()->addTimeout(this, delay, M_TIMEOUT_STOP);
		return true;
	}
}

bool ModbusInverterControl::onMove2Standby()
{
	m_enableOutput->setValue(0);
	if (m_setpoint->getValueF() != 0)
	{
		m_setpoint->setValue(0);
		m_requestedSetpoint->setValue((int)0);
	}
	m_controlState = E_ControlState_Standby;
	raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
	raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);
	m_stopping = true;
	endMove2Standby();
	execute();
	return true;
}

bool ModbusInverterControl::onReset2On()
{
	m_enableOutput->setValue(0);
	if (m_setpoint->getValueF() != 0)
	{
		m_setpoint->setValue(0);
		m_requestedSetpoint->setValue(0);
	}
	raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
	raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);
	m_stopping = true;
	endReset2On();
	execute();
	return true;
}

bool ModbusInverterControl::onMove2Error()
{
	m_enableOutput->setValue(0);
	if (m_setpoint->getValueF() != 0)
	{
		m_setpoint->setValue(0);
		m_requestedSetpoint->setValue(0);
	}
	m_stopping = true;
	raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
	raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);
	endMove2Error();
	execute();
	return true;
}

bool ModbusInverterControl::onStopOnEmr()
{
	m_enableOutput->setValue(0);
	if (m_setpoint->getValueF() != 0)
	{
		m_setpoint->setValue(0);
		m_requestedSetpoint->setValue(0);
	}
	m_controlState = E_ControlState_Standby;
	m_stopping = true;
	raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
	raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);
	endStopOnEmr();
	execute();
	return true;
}

bool ModbusInverterControl::onRecoverFromEmr()
{
	endRecoverFromEmr();
	execute();
	return true;
}

void ModbusInverterControl::timeoutExpired(uint16_t timeoutType)
{
	switch (timeoutType)
	{
	case M_TIMEOUT_DELAY:
		updateSetpoints();
		break;
	case M_TIMEOUT_STOP:
		onMove2Standby();
		break;
		//    case M_TIMEOUT_ACTIVATE:
		//        // if the last serial number is not 0, this means that the activation was from the OPC:
		//        // if the state is still M2R it means the inverter didn't start operation:
		//        if (m_lastSn != 0 && m_controlState != E_ControlState_Ready)
		//        {
		//            raiseError(0, E_PSSErrors_ActivationFailed, true);
		//        }
	}
}

void ModbusInverterControl::setBoardInReady(bool state)
{
	ControlBase::setBoardInReady(state);
	//    if (state)
	//    {
	//        m_driveStatus = PeripheralRepository::getInstance().getPeripheralContainingElementPssId(
	//                m_outputFrequency->getPssId())->getElementByIndex(4);
	//        if (m_driveStatus != NULL)
	//        {
	//            m_driveStatus->addObserver(this);
	//        }
	//    }
}

//void ModbusInverterControl::executeProtectionCheck(ElementBase* element)
//{
//    // Protection elements should stop the inverter is above max working, and continue when below min working.
//
//    // if the protection is not active, we can simply check the updated element if it should
//    // turn on the protection:
//    if (!m_isProtectionActive)
//    {
//        // if the temp is above protection limit, turn on the protection.
//        if (element->isOutsideSoftLimit())
//        {
//            m_isProtectionActive = true;
//            raiseError(E_PSSErrors_TemperatureExceedsLimits, true);
//            // we should also stop the inverter
//            *m_enableOutput = 0;
//            m_enableOutput->sendDeviceStatus();
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
//            *m_enableOutput = (m_requestedSetpoint != 0) ? 1 : 0;
//            m_enableOutput->sendDeviceStatus();
//            raiseError(E_PSSErrors_TemperatureExceedsLimits, false);
//        }
//    }
//}

void ModbusInverterControl::serialize(F_FILE* f)
{
    Serializer<ModbusInverterControl> s;
    s.serialize(f, *this);
}

ModbusInverterControl::ModbusInverterControl(F_FILE* f)
{
    Serializer<ModbusInverterControl> s;
    s.deserialize(f, *this);

    setOutputSetpointElement(m_setpoint);
    setOutputFrequencyElement(m_outputFrequency);
    setOutputCurrentElement(m_outputCurrent);
}

