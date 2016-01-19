/*
 * LiftPbOnError.cpp
 *
 *  Created on: 3 Jul 2014
 *      Author: maora
 */

#include "Controls/LiftPbOnError.h"
#include <Peripherals/PeripheralRepository.h>
#include <Peripherals/PSoC/PsocManager.h>
#include <PscServer/PscMessageHandler.h>

#define M_AIR_PRESSURE_INDEX 2
#define M_TUB_ENGAGE_INDEX 0
#define M_TUB_ENGAGE_COUNT 4
#define M_SQUEEGEE_ENGAGE_INDEX 4
#define M_SQUEEGEE_ENGAGE_COUNT 2
#define M_PB_OK_INDEX 1

LiftPbOnError::LiftPbOnError() :
        ControlBase()
{
    m_cableId = 0;
    setPssId(M_PSS_ID_ALL);
//    m_errorBitManager.setSecondaryPssId(M_PSS_ID_ALL);

    m_airPressure = NULL;
    m_tubEngage[0] = NULL;
    m_tubEngage[1] = NULL;
    m_tubEngage[2] = NULL;
    m_tubEngage[3] = NULL;
    m_squeegeeEngage[0] = NULL;
    m_squeegeeEngage[1] = NULL;
    m_pbOk = NULL;
}

LiftPbOnError::~LiftPbOnError()
{
    if (m_airPressure != NULL) m_airPressure->removeObserver(this);
    if (m_tubEngage[0] != NULL) m_tubEngage[0]->removeObserver(this);
    if (m_tubEngage[1] != NULL) m_tubEngage[1]->removeObserver(this);
    if (m_tubEngage[2] != NULL) m_tubEngage[2]->removeObserver(this);
    if (m_tubEngage[3] != NULL) m_tubEngage[3]->removeObserver(this);
    if (m_squeegeeEngage[0] != NULL) m_squeegeeEngage[0]->removeObserver(this);
    if (m_squeegeeEngage[1] != NULL) m_squeegeeEngage[1]->removeObserver(this);
}

void LiftPbOnError::execute()
{
}

bool LiftPbOnError::sendNotification()
{
    return true;
}

void LiftPbOnError::updateNotification(ElementBase* element)
{
    bool tubEngage1, tubEngage2, tubEngage3, tubEngage0, tubEngage;
    bool squeegeeEngage1, squeegeeEngage0, squeegeeEngage;
    bool airPressure;
    bool pbOk;

    tubEngage0 = m_tubEngage[0]->getValueI32();
    tubEngage1 = m_tubEngage[1]->getValueI32();
    tubEngage2 = m_tubEngage[2]->getValueI32();
    tubEngage3 = m_tubEngage[3]->getValueI32();
    tubEngage = tubEngage0 || tubEngage1 || tubEngage2 || tubEngage3;

    squeegeeEngage0 = m_squeegeeEngage[0]->getValueI32();
    squeegeeEngage1 = m_squeegeeEngage[1]->getValueI32();
    squeegeeEngage = squeegeeEngage0 && squeegeeEngage1;

    airPressure = m_airPressure->getValueI32();

    pbOk = (airPressure && squeegeeEngage) || !tubEngage;

    if (!pbOk)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Print bars lifted");
    }
    raiseError(M_PSS_ID_ALL, E_PSSErrors_PrintBarLifted, !pbOk);
    m_pbOk->setValue(pbOk);
}

bool LiftPbOnError::onInitControl()
{
    return true;
}

bool LiftPbOnError::onMove2Standby(uint32_t delay)
{
    return true;
}

bool LiftPbOnError::onReset2On()
{
    return true;
}

bool LiftPbOnError::onMove2Error()
{
    return true;
}

bool LiftPbOnError::onStopOnEmr()
{
    return true;
}

bool LiftPbOnError::initInputElements(int cableId)
{
    if (!PeripheralRepository::getInstance().initDigitalInputs(0, 6))
        return false;

    DigitalInputsPeripheral* digitalInputs = PeripheralRepository::getInstance().getDigitalInputsPeripheral();

    if (digitalInputs == NULL)
        return false;

    assert(digitalInputs != NULL);

    m_airPressure = digitalInputs->getElementByIndex(2);

    PsocManager* psocManager = PscMessageHandler::getInstance()->getPsocManager();

//    uint8_t cable1, cable2, cable3, errors;
//    psocManager->getPsocCableID(psocIndex, cable1, cable2, cable3, errors);

    m_cableId = cableId;

    psocManager->initDigitalOutputPeripheralByCableId(m_cableId, 0, 6);
    psocManager->initDigitalInputPeripheralByCableId(m_cableId, 0, 6);

    PsocDigitalInputPeripheral* psocDigitalInputs = psocManager->getDigitalInPeripheralByCableId(m_cableId);
    PsocDigitalOutputPeripheral *psocDigitalOutputs = psocManager->getDigitalOutPeripheralByCableId(m_cableId);

    if (psocDigitalInputs == NULL || psocDigitalOutputs == NULL)
        return false;

    m_tubEngage[0] = psocDigitalInputs->getElementByIndex(0);
    m_tubEngage[1] = psocDigitalInputs->getElementByIndex(1);
    m_tubEngage[2] = psocDigitalInputs->getElementByIndex(2);
    m_tubEngage[3] = psocDigitalInputs->getElementByIndex(3);

    m_squeegeeEngage[0] = psocDigitalInputs->getElementByIndex(4);
    m_squeegeeEngage[1] = psocDigitalInputs->getElementByIndex(5);

    m_pbOk = psocDigitalOutputs->getElementByIndex(1);

    m_airPressure->addObserver(this);
    m_tubEngage[0]->addObserver(this);
    m_tubEngage[1]->addObserver(this);
    m_tubEngage[2]->addObserver(this);
    m_tubEngage[3]->addObserver(this);
    m_squeegeeEngage[0]->addObserver(this);
    m_squeegeeEngage[1]->addObserver(this);

    return true;
}

bool LiftPbOnError::onRecoverFromEmr()
{
    return true;
}
