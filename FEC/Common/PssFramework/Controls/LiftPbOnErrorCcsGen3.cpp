/*
 * LiftPbOnErrorCcsGen3CcsGen3.cpp
 *
 *  Created on: 28 Jul 2015
 *      Author: maora
 */

#include "Controls/LiftPbOnErrorCcsGen3.h"
#include <Peripherals/PeripheralRepository.h>
#include <Peripherals/PSoC/PsocManager.h>
#include <PscServer/PscMessageHandler.h>

#define M_AIR_PRESSURE_INDEX 2
#define M_TUB_ENGAGE_INDEX 0
#define M_TUB_ENGAGE_COUNT 4
#define M_SQUEEGEE_ENGAGE_INDEX 4
#define M_SQUEEGEE_ENGAGE_COUNT 2
#define M_PB_OK_INDEX 1

LiftPbOnErrorCcsGen3::LiftPbOnErrorCcsGen3() :
        ControlBase()
{
    m_cableId = 0;
    setPssId(M_PSS_ID_ALL);
//    m_errorBitManager.setSecondaryPssId(M_PSS_ID_ALL);

    m_airPressureUp = NULL;
    m_airPressureDown = NULL;
    m_pbOk = NULL;
}

LiftPbOnErrorCcsGen3::~LiftPbOnErrorCcsGen3()
{
    if (m_airPressureUp != NULL) m_airPressureUp->removeObserver(this);
    if (m_airPressureDown != NULL) m_airPressureDown->removeObserver(this);
    if (m_airPressureOk != NULL) m_airPressureOk->removeObserver(this);
    if (m_blanketMoving != NULL) m_blanketMoving->removeObserver(this);
}

void LiftPbOnErrorCcsGen3::execute()
{
}

bool LiftPbOnErrorCcsGen3::sendNotification()
{
    return true;
}

void LiftPbOnErrorCcsGen3::updateNotification(ElementBase* element)
{
    // y = A.B + A.!C.!D.E.F + A.C.D.!E.!F
    // A: Air Pressure
    // B: Blanket Moving
    // C,D: Engage
    // E,F: Disengage
    float airPressureUp, airPressureDown;
    bool airPressureOk, blanketMoving;
    bool pbOk;

#define M_UPPER_PRESSURE_LIMIT 4.8
#define M_LOWER_PRESSURE_LIMIT 0.2

    airPressureUp = m_airPressureUp->getValueF();
    airPressureDown = m_airPressureDown->getValueF();

    airPressureOk = m_airPressureOk->getValueI32();
    blanketMoving = m_blanketMoving->getValueI32();

    pbOk = (airPressureOk && !blanketMoving) ||
            (airPressureOk && (airPressureDown < M_LOWER_PRESSURE_LIMIT) && (airPressureUp > M_UPPER_PRESSURE_LIMIT) ||
            (airPressureOk && (airPressureUp < M_LOWER_PRESSURE_LIMIT) && (airPressureDown > M_UPPER_PRESSURE_LIMIT)));

    if (!pbOk)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Print bars lifted");
    }
    raiseError(M_PSS_ID_ALL, E_PSSErrors_PrintBarLifted, !pbOk);
    m_pbOk->setValue(pbOk);
}

bool LiftPbOnErrorCcsGen3::onInitControl()
{
    return true;
}

bool LiftPbOnErrorCcsGen3::onMove2Standby()
{
    return true;
}

bool LiftPbOnErrorCcsGen3::onReset2On()
{
    return true;
}

bool LiftPbOnErrorCcsGen3::onMove2Error()
{
    return true;
}

bool LiftPbOnErrorCcsGen3::onStopOnEmr()
{
    return true;
}

bool LiftPbOnErrorCcsGen3::initInputElements(int cableId)
{
    if (!PeripheralRepository::getInstance().initDigitalInputs(0, 6))
        return false;

    DigitalInputsPeripheral* digitalInputs = PeripheralRepository::getInstance().getDigitalInputsPeripheral();

    if (digitalInputs == NULL)
        return false;

    assert(digitalInputs != NULL);

    PsocManager* psocManager = PscMessageHandler::getInstance()->getPsocManager();

    m_airPressureOk = digitalInputs->getElementByIndex(2);
    m_blanketMoving = digitalInputs->getElementByIndex(0);

//    uint8_t cable1, cable2, cable3, errors;
//    psocManager->getPsocCableID(psocIndex, cable1, cable2, cable3, errors);

    m_cableId = cableId;

    psocManager->initDigitalOutputPeripheralByCableId(m_cableId, 0, 6);
    psocManager->initDigitalInputPeripheralByCableId(m_cableId, 0, 6);

    PsocDigitalInputPeripheral* psocDigitalInputs = psocManager->getDigitalInPeripheralByCableId(m_cableId);
    PsocDigitalOutputPeripheral *psocDigitalOutputs = psocManager->getDigitalOutPeripheralByCableId(m_cableId);
    PsocAnalogInputsPeripheral* psocAnalogInputs = psocManager->getAnalogInPeripheralByCableId(m_cableId);

    if (psocDigitalInputs == NULL || psocDigitalOutputs == NULL || psocAnalogInputs == NULL)
        return false;

    m_airPressureUp = psocAnalogInputs->getElementByIndex(0);
    m_airPressureDown = psocAnalogInputs->getElementByIndex(1);

    m_pbOk = psocDigitalOutputs->getElementByIndex(1);

    m_airPressureUp->addObserver(this);
    m_airPressureDown->addObserver(this);
    m_airPressureOk->addObserver(this);
    m_blanketMoving->addObserver(this);

    return true;
}

bool LiftPbOnErrorCcsGen3::onRecoverFromEmr()
{
    return true;
}

