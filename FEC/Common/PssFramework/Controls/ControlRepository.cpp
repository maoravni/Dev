/*
 * ControlRepository.cpp
 *
 *  Created on: 9 Jul 2013
 *      Author: maora
 */

#include "ControlRepository.h"

ControlRepository ControlRepository::s_instance;

ControlRepository::ControlRepository()
{
    m_protectionControl = NULL;
    m_emergencyInputControl = NULL;
    m_orderedShutdownControl = NULL;
}

ControlRepository::~ControlRepository()
{
}

ControlBase* ControlRepository::getControlByPssId(int pssId)
{
    T_ControlListIterator i;
    for (i = m_controlList.begin(); i != m_controlList.end(); ++i)
    {
        if ((*i)->getPssId() == pssId)
            return (*i);
    }
    return NULL;
}

ControlBase* ControlRepository::getControlByIndex(int index)
{
    T_ControlListIterator i;
    for (i = m_controlList.begin(); i != m_controlList.end(); ++i)
    {
        if ((*i)->getControlIndex() == index)
            return (*i);
    }
    return NULL;
}

void ControlRepository::addControl(ControlBase* control)
{
    control->setControlIndex(m_controlList.size());
    m_controlList.push_back(control);
}

void ControlRepository::setAllMonitoringEnabled(bool monitoringEnabled)
{
    T_ControlListIterator i;
    for (i = m_controlList.begin(); i != m_controlList.end(); ++i)
    {
        if ((*i)->getPssId() != M_UNASSIGNED_PSS_ID)
            (*i)->setMonitoringEnabled(monitoringEnabled);
    }
}

void ControlRepository::destroyAllControls()
{
    T_ControlListIterator i;
    for (i = m_controlList.begin(); i != m_controlList.end(); ++i)
        delete (*i);

    m_controlList.clear();

    m_protectionControl = NULL;
    m_emergencyInputControl = NULL;
    m_orderedShutdownControl = NULL;
}

void ControlRepository::initProtectionControl()
{
    if (m_protectionControl == NULL)
    {
//        PeripheralRepository::getInstance().initDryContactOutput();
        m_protectionControl = new ProtectionControl();
    }
}

void ControlRepository::initEmergencyInputControl()
{
    if (m_emergencyInputControl == NULL)
        m_emergencyInputControl = new EmergencyInputControl();
}

ProtectionControl* ControlRepository::getProtectionControl()
{
    if (m_protectionControl == NULL)
        initProtectionControl();

    return m_protectionControl;
}

EmergencyInputControl* ControlRepository::getEmergencyInputControl()
{
    if (m_emergencyInputControl == NULL)
        initEmergencyInputControl();

    return m_emergencyInputControl;
}

/**
 * if exceptStopOnDisconnection is false, the action will be performed on all controls,
 * regardless the value of the "stopOnDisconnection" variable.
 */
void ControlRepository::resetAllControlsToOn(bool exceptStopOnDisconnection)
{
    T_ControlListIterator i;
    for (i = m_controlList.begin(); i != m_controlList.end(); ++i)
    {
        if (!(exceptStopOnDisconnection && !(*i)->isStopOnDisconnection()))
        {
            (*i)->reset2On(0, 0);
        }
    }
}

/**
 * if exceptStopOnDisconnection is false, the action will be performed on all controls,
 * regardless the value of the "stopOnDisconnection" variable.
 */
void ControlRepository::initAllControls(bool exceptStopOnDisconnection)
{
    T_ControlListIterator i;
    for (i = m_controlList.begin(); i != m_controlList.end(); ++i)
    {
        if (!(exceptStopOnDisconnection && !(*i)->isStopOnDisconnection()))
            (*i)->initControl(0, 0);
    }
}

/**
 * if exceptStopOnDisconnection is false, the action will be performed on all controls,
 * regardless the value of the "stopOnDisconnection" variable.
 */
void ControlRepository::stopAllControls(bool exceptStopOnDisconnection)
{
    T_ControlListIterator i;
    for (i = m_controlList.begin(); i != m_controlList.end(); ++i)
    {
        if (!(exceptStopOnDisconnection && !(*i)->isStopOnDisconnection()))
            (*i)->move2Standby(0, 0, 0);
    }
}

void ControlRepository::stopOnEmr()
{
    T_ControlListIterator i;
    for (i = m_controlList.begin(); i != m_controlList.end(); ++i)
    {
        // TODO: Restore moving to emergency when we can handle errors correctly.
//        (*i)->move2Standby(0, 0);
        (*i)->stopOnEmr();
    }
}

void ControlRepository::recoverFromEmergency()
{
    T_ControlListIterator i;
    for (i = m_controlList.begin(); i != m_controlList.end(); ++i)
    {
        (*i)->recoverFromEmr();
    }
}

bool ControlRepository::isInEmergency()
{
    if (m_emergencyInputControl != NULL)
        return m_emergencyInputControl->isEmergencyActive();
    else
        return false;
}

void ControlRepository::sendUpdateNotificationForAllControls()
{
    T_ControlListIterator i;
    for (i = m_controlList.begin(); i != m_controlList.end(); ++i)
    {
        (*i)->sendNotification();
    }
//    for (int i = 0; i < m_controlArray.size(); ++i)
//    {
//        m_controlArray[i]->sendNotification();
//    }
}

bool ControlRepository::addShutdownOperation(int delay, uint16_t control, E_ShutdownOperation operation, float setpoint)
{
    if (m_orderedShutdownControl == NULL)
        m_orderedShutdownControl = new OrderedShutdownControl();

    return m_orderedShutdownControl->addOperation(delay, control, operation, setpoint);
}

void ControlRepository::executeShutdownOperation(bool exceptStopOnDisconnection)
{
    if (m_orderedShutdownControl == NULL)
    {
        // perform the default operations when disconnecting, if no operation was defined.
        addShutdownOperation(0, 0xffff, E_ShutdownOperation_ResetToOn, 0);
        addShutdownOperation(500, 0xffff, E_ShutdownOperation_Init, 0);
        addShutdownOperation(500, 0xffff, E_ShutdownOperation_ResetToOn, 0);
    }

    m_orderedShutdownControl->performDisconnectionSequence(exceptStopOnDisconnection);
}

void ControlRepository::setBoardInReady(bool state)
{
    T_ControlListIterator i;
    for (i = m_controlList.begin(); i != m_controlList.end(); ++i)
    {
        (*i)->setBoardInReady(state);
    }
}

void ControlRepository::startRecovery()
{
    T_ControlListIterator i;
    for (i = m_controlList.begin(); i != m_controlList.end(); ++i)
    {
        (*i)->startRecovery();
    }
}
