/*
 * LeakDetectionControl.cpp
 *
 *  Created on: 28 Oct 2014
 *      Author: maora
 */

#include "LeakDetectionControl.h"
#include <PscServer/PscMasterServer.h>
#include <Peripherals/PeripheralRepository.h>

LeakDetectionControl::LeakDetectionControl() :
        ControlBase()
{
    m_tankLevel = NULL;
    m_tubLevel = NULL;
    m_window = 0;
    m_deviation = 0;
    m_timeout = 0;
    m_currentStage = E_LeakDetectionCalibrationStage_Init;
    m_tankInitialLevel = 0;
    m_deltaHtx = 0;
    m_deltaTub = 0;
    m_filterCoeff = 0;
    m_tankLevelFiltered = 0;
    m_tubLevelFiltered = 0;
    m_updateInterval = 0;
}

LeakDetectionControl::~LeakDetectionControl()
{
    m_tankLevel->removeObserver(this);
    m_tubLevel->removeObserver(this);
}

void LeakDetectionControl::updateNotification(ElementBase* element)
{
    if (!element->isValid())
        return;

    if (element == m_tankLevel)
    {
//        if (m_updateInterval == 0)
    }

}

void LeakDetectionControl::execute()
{
    switch (m_controlState)
    {
    case E_ControlState_On:
        return;
    case E_ControlState_Move2Ready:
    {
//        bool levelStable = false;
        switch (m_currentStage)
        {
        case E_LeakDetectionCalibrationStage_TankInitialLevel:
            if (waitForStableLevel(m_tankLevelFiltered))
            {
                m_tankInitialLevel = m_tankLevelFiltered;
                if (m_lastSn != 0)
                {
                    PscMasterServer::getInstance().sendSeqEnded(MSG_ActivateLeakageDetectionControl, m_lastSn,
                            Psc_ControllerId, getPssId(), E_SeqEndedStatus_Success);
                }
                m_controlState = E_ControlState_Standby;
            }
            break;
        case E_LeakDetectionCalibrationStage_DeltaHTX:
            if (waitForStableLevel(m_tankLevelFiltered))
            {
                m_deltaHtx = m_tankLevelFiltered;
                if (m_lastSn != 0)
                {
                    PscMasterServer::getInstance().sendSeqEnded(MSG_ActivateLeakageDetectionControl, m_lastSn,
                            Psc_ControllerId, getPssId(), E_SeqEndedStatus_Success);
                }
                m_controlState = E_ControlState_Standby;
            }
            break;
        case E_LeakDetectionCalibrationStage_TubStabilization:
            if (waitForStableLevel(m_tubLevelFiltered))
            {
                if (m_lastSn != 0)
                {
                    PscMasterServer::getInstance().sendSeqEnded(MSG_ActivateLeakageDetectionControl, m_lastSn,
                            Psc_ControllerId, getPssId(), E_SeqEndedStatus_Success);
                }
                m_controlState = E_ControlState_Standby;
            }
            break;
        case E_LeakDetectionCalibrationStage_DeltaTub:
            if (waitForStableLevel(m_tankLevelFiltered))
            {
                m_deltaTub = m_tankLevelFiltered;
                if (m_lastSn != 0)
                {
                    PscMasterServer::getInstance().sendSeqEnded(MSG_ActivateLeakageDetectionControl, m_lastSn,
                            Psc_ControllerId, getPssId(), E_SeqEndedStatus_Success);
                }
                m_controlState = E_ControlState_Standby;
            }
            break;
        case E_LeakDetectionCalibrationStage_EndCalibration:
            break;
        }
    }
    break;
    case E_ControlState_Ready:
    {
        bool result;
        // check if leak is detected, and raise an error if so:
        result = waitForStableLevel(0);
        raiseError(0, E_PSSErrors_ControlExceedsLimits, !result);
        break;
    }
    }
}

bool LeakDetectionControl::sendNotification()
{
    // static instance for the master server.
    PscMessageStruct replyMessage;

    replyMessage.header.id.split.id = MSG_ControlStatusNotification;
    replyMessage.payload.pSSControlStatusNotificationMsg.pssId = getPssId();
    replyMessage.payload.pSSControlStatusNotificationMsg.dataType = E_ValueType_U32;
    replyMessage.payload.pSSControlStatusNotificationMsg.setPoint = 0;
    replyMessage.payload.pSSControlStatusNotificationMsg.state = m_controlState;
    replyMessage.payload.pSSControlStatusNotificationMsg.exceptions = getControlExceptions();
    replyMessage.payload.pSSControlStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSControlStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);

    return true;
}

void LeakDetectionControl::activateControl(unsigned short stage, int timeout, int window, float deviation, int sn)
{
    if (m_controlState == E_ControlState_On)
        return;

    m_window = window;
    calculateLpfCoeff();

    E_LeakDetectionCalibrationStage newStage = (E_LeakDetectionCalibrationStage) stage;
    m_lastSn = sn;

    switch (newStage)
    {
    case E_LeakDetectionCalibrationStage_TankInitialLevel:
        m_controlState = E_ControlState_Move2Ready;
        break;
    case E_LeakDetectionCalibrationStage_DeltaHTX:
        m_controlState = E_ControlState_Move2Ready;
        break;
    case E_LeakDetectionCalibrationStage_TubStabilization:
        m_controlState = E_ControlState_Move2Ready;
        break;
    case E_LeakDetectionCalibrationStage_DeltaTub:
        m_controlState = E_ControlState_Move2Ready;
        break;
    case E_LeakDetectionCalibrationStage_EndCalibration:
        m_controlState = E_ControlState_Ready;
        if (m_lastSn != 0)
        {
            PscMasterServer::getInstance().sendSeqEnded(MSG_ActivateLeakageDetectionControl, m_lastSn, Psc_ControllerId,
                    getPssId(), E_SeqEndedStatus_Success);
        }
        break;
    }
    sendNotification();
}

bool LeakDetectionControl::onInitControl()
{
    endInitControl();
    return true;
}

bool LeakDetectionControl::onMove2Standby()
{
    endMove2Standby();
    return true;
}

bool LeakDetectionControl::onReset2On()
{
    m_currentStage = E_LeakDetectionCalibrationStage_Init;
    endReset2On();
    return true;
}

bool LeakDetectionControl::onMove2Error()
{
    endMove2Error();
    return true;
}

bool LeakDetectionControl::onRecoverFromEmr()
{
    endRecoverFromEmr();
    return true;
}

bool LeakDetectionControl::waitForStableLevel(float level)
{
    // TODO: Perform calculations for determining if the level is stable.
    return true;
}

void LeakDetectionControl::setElementTankLevel(ElementBase* input)
{
    m_tankLevel = input;
    m_tankLevel->addObserver(this);
//    m_errorBitManager.setSecondaryPssId(m_tankLevel->getPssId());

    PeripheralBase* periph = PeripheralRepository::getInstance().getPeripheralContainingElementPssId(input->getPssId());

    assert(periph != NULL);
    m_updateInterval = periph->getUpdateInterval();
}

bool LeakDetectionControl::onStopOnEmr()
{
    endStopOnEmr();
    return true;
}

void LeakDetectionControl::calculateLpfCoeff()
{
    m_filterCoeff = (float)m_window/(float)m_updateInterval;
}
