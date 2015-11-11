/*
 * DeviceRepository.cpp
 *
 *  Created on: 10 Jun 2013
 *      Author: maora
 */

#include "PeripheralRepository.h"
#include <Tasks/UpdateSchedulerTask.h>
#include <logger.h>

PeripheralRepository PeripheralRepository::s_instance;

PeripheralRepository::PeripheralRepository()
{
    m_internalTemperatureSensors = NULL;
    m_digitalOutputs = NULL;
    m_swPwmOutput = NULL;
    m_dryContactOutput = NULL;
    m_digitalInputs = NULL;
    m_analogCurrentOutputs = NULL;
    m_analogInputs = NULL;
}

PeripheralRepository::~PeripheralRepository()
{
}

void PeripheralRepository::addPeripheral(PeripheralBase* device)
{
    M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Adding device with ID {[PSSID:%d]}", device->getPssId());

    // assign the device index to the new device:
    device->setPeripheralRepIndex(m_periphVector.size());

    // add the device to the repository
    m_periphVector.push_back(device);

    // add the device to the update scheduler:
    //UpdateSchedulerTask::getInstance().addDevice(device);

}

PeripheralBase* PeripheralRepository::getPeripheralByPssId(int pssId)
{
//    for (int i = 0; i < E_PeripheralType_Size; ++i)
//    {
    for (int j = 0; j < m_periphVector.size(); ++j)
    {
        if (pssId == m_periphVector[j]->getPssId())
            return m_periphVector[j];
    }
//    }
    return NULL;
}

PeripheralBase* PeripheralRepository::getPeripheralContainingElementPssId(int pssId)
{
//    for (int i = 0; i < E_PeripheralType_Size; ++i)
//    {
    ElementBase* element;
    for (int j = 0; j < m_periphVector.size(); ++j)
    {
        element = m_periphVector[j]->getElementByPssId(pssId);
        if (element != NULL)
            return m_periphVector[j];
    }
//    }
    return NULL;
}

PeripheralBase* PeripheralRepository::getPeripheralByIndex(int index)
{
    if (index >= m_periphVector.size())
        return NULL;

    assert(index == m_periphVector[index]->getPeripheralRepIndex());

    return m_periphVector[index];
}

void PeripheralRepository::initInternalPeripherals()
{
    initInternalTemperatureSensors(0, 200, 2000, 12);
    initDigitalOutputs(0, 12);
    initSwPwmOutput(0, 12);
    initDryContactOutput(0);
    initDigitalInputs(0, 12);
    initAnalogCurrentOutputs(0, 2);
//    if (m_internalTemperatureSensors == NULL)
//    {
//        m_internalTemperatureSensors = new InternalTemperatureSensors();
//        addPeripheral(m_internalTemperatureSensors);
//        m_internalTemperatureSensors->setUpdateInterval(200);
//    }
//    if (m_digitalOutputs == NULL) {
//        m_digitalOutputs = new DigitalOutputsPeripheral();
//        addPeripheral(m_digitalOutputs);
//    }
//    if (m_digitalInputs == NULL) {
//        m_digitalInputs = new DigitalInputsPeripheral();
//        addPeripheral(m_digitalInputs);
//    }
//    if (m_swPwmOutput == NULL)
//    {
//        m_swPwmOutput = new SwPwmOutputPeripheral();
//        addPeripheral(m_swPwmOutput);
//        m_swPwmOutput->createTask();
//    }
//    if (m_dryContactOutput == NULL) {
//        m_dryContactOutput = new DryContactDigitalOutput();
//        addPeripheral(m_dryContactOutput);
//    }
//    if (m_analogCurrentOutputs == NULL)
//    {
//        m_analogCurrentOutputs = new AnalogOutCurrentPeripheral();
//        addPeripheral(m_analogCurrentOutputs);
//    }
}

DigitalOutputsPeripheral* PeripheralRepository::getDigitalOutputsPeripheral()
{
    return m_digitalOutputs;
}

InternalTemperatureSensors* PeripheralRepository::getInternalTemperatureSensorsPeripheral()
{
    return m_internalTemperatureSensors;
}

SwPwmOutputPeripheral* PeripheralRepository::getSwPwmOutputPeripheral()
{
    return m_swPwmOutput;
}

bool PeripheralRepository::initDigitalOutputs(int pssId, int numberOfDevices)
{
    if (m_digitalOutputs != NULL)
    {
        if (pssId == 0)
        {
            return true;
        }
        else if (m_digitalOutputs->getPssId() == 0)
        {
            m_digitalOutputs->setPssId(pssId);
            return true;
        }
        else
            return false;
    }

    m_digitalOutputs = new DigitalOutputsPeripheral();
    addPeripheral(m_digitalOutputs);
    m_digitalOutputs->setPssId(pssId);

    // TODO: Do we need to delete the just created peripheral?
    if (m_digitalOutputs->getElementCount() < numberOfDevices)
        return false;

    return true;
}

bool PeripheralRepository::initSwPwmOutput(int pssId, int numberOfDevices)
{
    if (m_swPwmOutput != NULL)
    {
        if (pssId == 0)
        {
            return true;
        }
        else if (m_swPwmOutput->getPssId() == 0)
        {
            m_swPwmOutput->setPssId(pssId);
            return true;
        }
        else
            return false;
    }

    m_swPwmOutput = new SwPwmOutputPeripheral();
    addPeripheral(m_swPwmOutput);
    m_swPwmOutput->createTask();
    m_swPwmOutput->setPssId(pssId);

    // TODO: Do we need to delete the just created peripheral?
    if (m_swPwmOutput->getElementCount() < numberOfDevices)
        return false;

    return true;
}

bool PeripheralRepository::initDryContactOutput(int pssId)
{
    if (m_dryContactOutput != NULL)
    {
        if (pssId == 0)
        {
            return true;
        }
        else if (m_dryContactOutput->getPssId() == 0)
        {
            m_dryContactOutput->setPssId(pssId);
            return true;
        }
        else
            return false;
    }

    m_dryContactOutput = new DryContactDigitalOutput();
    addPeripheral(m_dryContactOutput);
    m_dryContactOutput->setPssId(pssId);

    return true;
}

bool PeripheralRepository::initDigitalInputs(int pssId, int numberOfDevices)
{
    if (m_digitalInputs != NULL)
    {
        if (pssId == 0)
        {
            return true;
        }
        else if (m_digitalInputs->getPssId() == 0)
        {
            m_digitalInputs->setPssId(pssId);
            return true;
        }
        else
            return false;
    }

    m_digitalInputs = new DigitalInputsPeripheral();
    addPeripheral(m_digitalInputs);
    m_digitalInputs->setPssId(pssId);

    // TODO: Do we need to delete the just created peripheral?
    if (m_digitalInputs->getElementCount() < numberOfDevices)
        return false;

    return true;
}

bool PeripheralRepository::initAnalogInputs(int pssId, int numberOfDevices)
{
    if (m_analogInputs != NULL)
    {
        if (pssId == 0)
        {
            return true;
        }
        else if (m_analogInputs->getPssId() == 0)
        {
            m_analogInputs->setPssId(pssId);
            return true;
        }
        else
            return false;
    }

    m_analogInputs = new AnalogInputPeripheral();
    addPeripheral(m_analogInputs);
    m_analogInputs->setPssId(pssId);

    // TODO: Do we need to delete the just created peripheral?
    if (m_analogInputs->getElementCount() < numberOfDevices)
        return false;

    return true;
}

bool PeripheralRepository::initAnalogCurrentOutputs(int pssId, int numberOfDevices)
{
    if (m_analogCurrentOutputs != NULL)
    {
        if (pssId == 0)
        {
            return true;
        }
        else if (m_analogCurrentOutputs->getPssId() == 0)
        {
            m_analogCurrentOutputs->setPssId(pssId);
            return true;
        }
        else
            return false;
    }

    m_analogCurrentOutputs = new AnalogOutCurrentPeripheral();
    addPeripheral(m_analogCurrentOutputs);
    m_analogCurrentOutputs->setPssId(pssId);

    // TODO: Do we need to delete the just created peripheral?
    if (m_analogCurrentOutputs->getElementCount() < numberOfDevices)
        return false;

    return true;
}

bool PeripheralRepository::initInternalTemperatureSensors(int pssId, int sampleInterval, int lpfWindow,
        int numberOfDevices)
{
    if (m_internalTemperatureSensors != NULL)
    {
        if (pssId == 0)
        {
            return true;
        }
        else if (m_internalTemperatureSensors->getPssId() == 0)
        {
            m_internalTemperatureSensors->setPssId(pssId);
            return true;
        }
        else
            return false;
    }

    m_internalTemperatureSensors = new InternalTemperatureSensors();
    addPeripheral(m_internalTemperatureSensors);
    if (sampleInterval == 0)
        sampleInterval = 100;
    m_internalTemperatureSensors->setUpdateInterval(sampleInterval);
    m_internalTemperatureSensors->setPssId(pssId);

    // CHECK: Do we need to delete the just created peripheral?
    if (m_internalTemperatureSensors->getElementCount() < numberOfDevices)
        return false;

    return true;
}

void PeripheralRepository::setBoardInReady(bool state)
{
    int i;
    for (i = 0; i < m_periphVector.size(); ++i)
    {
        m_periphVector[i]->setBoardInReady(state);
    }
}

void PeripheralRepository::destroyAllPeripherals()
{
//    for (int i = 0; i < E_PeripheralType_Size; ++i)
//    {
    for (int j = 0; j < m_periphVector.size(); ++j)
    {
        delete m_periphVector[j];
    }
    m_periphVector.clean();
//    }

    m_internalTemperatureSensors = NULL;
    m_digitalOutputs = NULL;
    m_swPwmOutput = NULL;
    m_dryContactOutput = NULL;
    m_digitalInputs = NULL;
    m_analogCurrentOutputs = NULL;
    m_analogInputs = NULL;
}

int PeripheralRepository::getPeripheralCount()
{
//    int result = 0;
//    for (int i = 0; i < E_PeripheralType_Size; ++i)
//    {
//        result += m_periphVector[i].size();
//    }
//    return result;
    return m_periphVector.size();
}

void PeripheralRepository::startRecovery()
{
    int i;
    for (i = 0; i < m_periphVector.size(); ++i)
    {
        m_periphVector[i]->startRecovery();
    }
}
