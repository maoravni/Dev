/*
 * LiftPbOnErrorCcsGen2CcsGen2.cpp
 *
 *  Created on: 28 Jul 2015
 *      Author: maora
 */

#include "Controls/LiftPbOnErrorCcsGen2.h"
#include <Peripherals/PeripheralRepository.h>
#include <Peripherals/PSoC/PsocManager.h>
#include <PscServer/PscMessageHandler.h>
#include <Persistency/ControlSerializers.h>

#define M_AIR_PRESSURE_INDEX 2
#define M_TUB_ENGAGE_INDEX 0
#define M_TUB_ENGAGE_COUNT 4
#define M_SQUEEGEE_ENGAGE_INDEX 4
#define M_SQUEEGEE_ENGAGE_COUNT 2
#define M_PB_OK_INDEX 1

LiftPbOnErrorCcsGen2::LiftPbOnErrorCcsGen2() :
        ControlBase()
{
    m_cableId = 0;
    setPssId(M_PSS_ID_ALL);
//    m_errorBitManager.setSecondaryPssId(M_PSS_ID_ALL);

    m_tubEngage[0] = NULL;
    m_tubEngage[1] = NULL;
    m_tubDisengage[0] = NULL;
    m_tubDisengage[1] = NULL;
    m_pbOk = NULL;
    m_airPressureOk = NULL;
}

LiftPbOnErrorCcsGen2::~LiftPbOnErrorCcsGen2()
{
    if (m_tubEngage[0] != NULL) m_tubEngage[0]->removeObserver(this);
    if (m_tubEngage[1] != NULL) m_tubEngage[1]->removeObserver(this);
    if (m_tubDisengage[0] != NULL) m_tubDisengage[0]->removeObserver(this);
    if (m_tubDisengage[1] != NULL) m_tubDisengage[1]->removeObserver(this);
    if (m_airPressureOk != NULL) m_airPressureOk->removeObserver(this);
    if (m_blanketMoving != NULL) m_blanketMoving->removeObserver(this);
}

void LiftPbOnErrorCcsGen2::execute()
{
}

bool LiftPbOnErrorCcsGen2::sendNotification()
{
    return true;
}

void LiftPbOnErrorCcsGen2::updateNotification(ElementBase* element)
{
    // y = A.B + A.!C.!D.E.F + A.C.D.!E.!F
    // A: Air Pressure
    // B: Blanket Moving
    // C,D: Engage
    // E,F: Disengage
    bool tubEngage1, tubEngage0, tubDisengage0, tubDisengage1, airPressureOk, blanketMoving;
    bool pbOk;

    tubEngage0 = m_tubEngage[0]->getValueI32();
    tubEngage1 = m_tubEngage[1]->getValueI32();
    tubDisengage0 = m_tubDisengage[0]->getValueI32();
    tubDisengage1 = m_tubDisengage[1]->getValueI32();
    airPressureOk = m_airPressureOk->getValueI32();
    blanketMoving = m_blanketMoving->getValueI32();

    pbOk = (airPressureOk && !blanketMoving) ||
            (airPressureOk && !tubEngage0 && !tubEngage1 && tubDisengage0 && tubDisengage1) ||
            (airPressureOk && tubEngage0 && tubEngage1 && !tubDisengage0 && !tubDisengage1);

    if (!pbOk)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Print bars lifted");
    }
    raiseErrorSimple(M_PSS_ID_ALL, E_PSSErrors_PrintBarLifted, !pbOk);
    m_pbOk->setValue(pbOk);
}

bool LiftPbOnErrorCcsGen2::onInitControl()
{
    return true;
}

bool LiftPbOnErrorCcsGen2::onMove2Standby(uint32_t delay)
{
    return true;
}

bool LiftPbOnErrorCcsGen2::onReset2On()
{
    return true;
}

bool LiftPbOnErrorCcsGen2::onMove2Error()
{
    return true;
}

bool LiftPbOnErrorCcsGen2::onStopOnEmr()
{
    return true;
}

bool LiftPbOnErrorCcsGen2::initInputElements(int cableId)
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

    if (psocDigitalInputs == NULL || psocDigitalOutputs == NULL)
        return false;

    m_tubEngage[0] = psocDigitalInputs->getElementByIndex(1);
    m_tubEngage[1] = psocDigitalInputs->getElementByIndex(3);
    m_tubDisengage[0] = psocDigitalInputs->getElementByIndex(0);
    m_tubDisengage[1] = psocDigitalInputs->getElementByIndex(2);

    m_pbOk = psocDigitalOutputs->getElementByIndex(1);

    m_tubEngage[0]->addObserver(this);
    m_tubEngage[1]->addObserver(this);
    m_tubDisengage[0]->addObserver(this);
    m_tubDisengage[1]->addObserver(this);
    m_airPressureOk->addObserver(this);
    m_blanketMoving->addObserver(this);


    return true;
}

bool LiftPbOnErrorCcsGen2::onRecoverFromEmr()
{
    return true;
}

void LiftPbOnErrorCcsGen2::serialize(F_FILE* f)
{
    Serializer<LiftPbOnErrorCcsGen2> s;
    s.serialize(f, *this);
}
LiftPbOnErrorCcsGen2::LiftPbOnErrorCcsGen2(F_FILE* f)
{
    Serializer<LiftPbOnErrorCcsGen2> s;
    s.deserialize(f, *this);

    m_tubEngage[0]->addObserver(this);
    m_tubEngage[1]->addObserver(this);
    m_tubDisengage[0]->addObserver(this);
    m_tubDisengage[1]->addObserver(this);
    m_airPressureOk->addObserver(this);
    m_blanketMoving->addObserver(this);
}

