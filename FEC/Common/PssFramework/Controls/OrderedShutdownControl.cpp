/*
 * OrderedShutdownControl.cpp
 *
 *  Created on: 22 Dec 2014
 *      Author: maora
 */

#include "OrderedShutdownControl.h"
#include <Controls/ControlRepository.h>
#include <Tasks/UpdateSchedulerTask.h>
#include <logger.h>

OrderedShutdownControl::OrderedShutdownControl()
{
    m_operationCompleteSemaphore.create();
    m_operationCompleteSemaphore.take(0);
}

OrderedShutdownControl::~OrderedShutdownControl()
{
}

void OrderedShutdownControl::execute()
{
}

bool OrderedShutdownControl::onInitControl()
{
    endInitControl();
    return true;
}

bool OrderedShutdownControl::onMove2Standby()
{
    endMove2Standby();
    return true;
}

bool OrderedShutdownControl::onReset2On()
{
    endReset2On();
    return true;
}

bool OrderedShutdownControl::onMove2Error()
{
    endMove2Error();
    return true;
}

bool OrderedShutdownControl::onStopOnEmr()
{
    endStopOnEmr();
    return true;
}

bool OrderedShutdownControl::onRecoverFromEmr()
{
    endRecoverFromEmr();
    return true;
}

bool OrderedShutdownControl::sendNotification()
{
    return true;
}

void OrderedShutdownControl::addFeedbackElement(ElementBase* element, float thresholdValue, bool greaterThan,
        bool deactivate)
{
}

//void OrderedShutdownControl::addDependentElement(ElementBase* element, float thresholdValue, bool greaterThan,
//        bool deactivate, bool lock, int checkType)
//{
//}

void OrderedShutdownControl::updateNotification(ElementBase* element)
{
}

bool OrderedShutdownControl::requestValueChange(ElementBase* element)
{
    return true;
}

void OrderedShutdownControl::timeoutExpired(uint16_t timeoutType)
{
    executeCurrentOperation();

    // advance the iterator and check for the next iteration:
    ++m_currentOperation;
    if (m_currentOperation == m_shutdownOperationList.end())
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Ordered Shutdown Complete");
        m_operationCompleteSemaphore.give();
    }
    else
    {
        UpdateSchedulerTask::getInstance()->addTimeout(this, m_currentOperation->delay);
    }
}

bool OrderedShutdownControl::addOperation(int delay, uint16_t controlPssId, E_ShutdownOperation op, float setpoint)
{
    T_OperationNode operation;
    if (delay == 0)
        delay = 1;
    operation.delay = delay;

    // TODO: support ALL CONTROLS
    // TODO: handled control search errors.
    if (controlPssId == 0xffff)
    {
        // we don't want to support global activation command for all commands.
        if (op == E_ShutdownOperation_Activate)
            return false;
        operation.control = NULL;
    }
    else
    {
        operation.control = ControlRepository::getInstance().getControlByPssId(controlPssId);
        if (operation.control == NULL)
            return false;
    }

    operation.operation = op;
    operation.setpoint = setpoint;
    m_shutdownOperationList.push_back(operation);

    return true;
}

void OrderedShutdownControl::performDisconnectionSequence()
{
    m_currentOperation = m_shutdownOperationList.begin();
    UpdateSchedulerTask::getInstance()->addTimeout(this, m_currentOperation->delay);

    // Try to take the semaphore. The semaphore will be given after the last operation has been performed.
    m_operationCompleteSemaphore.take(portMAX_DELAY);
}

void OrderedShutdownControl::executeCurrentOperation()
{
    ControlBase* control;
    if (m_currentOperation->control != NULL)
    {
        control = m_currentOperation->control;
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Executing Shutdown Op: delay=%d control=%d op=%d setpoint=%f",
                m_currentOperation->delay, control->getPssId(), m_currentOperation->operation,
                m_currentOperation->setpoint);

        switch (m_currentOperation->operation)
        {
        case E_ShutdownOperation_ResetToOn:
            control->reset2On(0, 0);
            break;
        case E_ShutdownOperation_Init:
            control->initControl(0, 0);
            break;
        case E_ShutdownOperation_Stop:
            control->move2Standby(0, 0);
            break;
        case E_ShutdownOperation_Activate:
            control->setSetpoint(m_currentOperation->setpoint, 0);
            break;
        }
    }
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Executing Shutdown Op: delay=%d control=AllControls op=%d setpoint=%f",
                m_currentOperation->delay, m_currentOperation->operation, m_currentOperation->setpoint);

        switch (m_currentOperation->operation)
        {
        case E_ShutdownOperation_ResetToOn:
            ControlRepository::getInstance().resetAllControlsToOn();
            break;
        case E_ShutdownOperation_Init:
            ControlRepository::getInstance().initAllControls();
            break;
        case E_ShutdownOperation_Stop:
            ControlRepository::getInstance().stopAllControls();
            break;
        }
    }
}
