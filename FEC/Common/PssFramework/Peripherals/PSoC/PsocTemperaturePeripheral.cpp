/*
 * PsocTemperaturePeripheral.cpp
 *
 *  Created on: 8 αιπε 2014
 *      Author: maora
 */

#include "Peripherals/PSoC/PsocTemperaturePeripheral.h"
#include <Elements/ElementRepository.h>
#include "PsocHandler.h"
#include <Controls/ControlRepository.h>
#include "Persistency/PsocPeripheralSerializers.h"

void bp()
{
    puts("bp");
}

PsocTemperaturePeripheral::PsocTemperaturePeripheral()
{
    for (int i = 0; i < M_NUM_OF_TEMPERATURE_SENSORS; ++i)
    {
        m_temperatureElementsArray[i] = ElementRepository::getInstance().addValidationElementFloat();
        m_aCoeff[i] = 1;
        m_bCoeff[i] = 0;
        m_hardLimit[i] = 900;
        m_sensorType[i] = 0;
    }

    setLpfWindow(0);
}

PsocTemperaturePeripheral::~PsocTemperaturePeripheral()
{
}

#define M_LowPassFilter(newVal, prevVal, coeff) coeff * newVal + (1 - coeff) * prevVal

void PsocTemperaturePeripheral::readInputs()
{
    float temperature;
    if (m_psocHandler->getPsocCommState() == E_PsocCommState_Enabled)
    {
        uint8_t temperatureErrorBits = m_psocHandler->getLocalTemperatureErrorBits();
        bool valueValid;
        for (int i = 0; i < M_NUM_OF_TEMPERATURE_SENSORS; ++i)
        {
            valueValid = ((temperatureErrorBits & (0x2 << (i * 2))) == 0);
            m_temperatureElementsArray[i]->setValueValid(valueValid);

            temperature = M_LowPassFilter(m_psocHandler->getLocalTemperatureSensorValue(i),
                    m_temperatureElementsArray[i]->getValueF(), m_lpfCoeff);
            m_temperatureElementsArray[i]->setValue(temperature);
        }
    }
}

void PsocTemperaturePeripheral::setPsocHandler(PsocHandler* psocHandler)
{
    m_psocHandler = psocHandler;
}

ElementBase* PsocTemperaturePeripheral::getElementByIndex(int index)
{
    return m_temperatureElementsArray[index];
}

void PsocTemperaturePeripheral::enableElementByIndex(int index, bool enable)
{
}

void PsocTemperaturePeripheral::setCalibrationCoeff(int index, float aCoeff, float bCoeff)
{
    m_aCoeff[index] = aCoeff;
    m_bCoeff[index] = bCoeff;
}

void PsocTemperaturePeripheral::setScalingCoeff(int index, float aCoeff, float bCoeff)
{
}

ElementBase* PsocTemperaturePeripheral::getElementByPssId(int pssId)
{
    for (int i = 0; i < M_NUM_OF_TEMPERATURE_SENSORS; ++i)
    {
        if (m_temperatureElementsArray[i]->getPssId() == pssId)
            return m_temperatureElementsArray[i];
    }
    return NULL;
}

void PsocTemperaturePeripheral::setLpfWindow(int lpfWindow)
{
    m_lpfCoeff = 1 / ((float) lpfWindow + 1);
}

void PsocTemperaturePeripheral::setSensorType(int index, uint8_t type)
{
    m_sensorType[index] = type;
}

void PsocTemperaturePeripheral::downloadConfigurationToPsoc(CBinarySemaphore* completeSemaphore)
{
    float hardLimit;
    bool elementFound;
    for (int index = 0; index < M_NUM_OF_TEMPERATURE_SENSORS; ++index)
    {
        if (m_temperatureElementsArray[index]->getPssId() != 0)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "TempSensor PSSID {[PSSID:%d]} is at psoc %d channel %d",
                    m_temperatureElementsArray[index]->getPssId(), m_psocHandler->getPsocIndex(), index);
            E_PsocTemperatureSensorType sensType = (E_PsocTemperatureSensorType) m_sensorType[index];
            if (sensType != E_PsocTemperatureSensorType_LiquidDetector)
            {
                elementFound = ControlRepository::getInstance().getProtectionControl()->getElementUpperHardLimit(
                        m_temperatureElementsArray[index], hardLimit);
                if (!elementFound)
                    M_LOGGER_LOGF(M_LOGGER_LEVEL_WARNING, "Psoc Temp Sensor {[PSSID:%d]} have no hard limit defined",
                            m_temperatureElementsArray[index]->getPssId());
            }
            else
            {
                hardLimit = 400;
            }

            m_psocHandler->configTemperatureSensor(completeSemaphore, index,
                    (E_PsocTemperatureSensorType) m_sensorType[index], E_PsocTemperatureSensorFilterType_SinglePoleIIR,
                    hardLimit, m_aCoeff[index], m_bCoeff[index], m_temperatureElementsArray[index]->getMissingDevicePriority());
        }
    }
}

void PsocTemperaturePeripheral::raiseError(E_PsocErrorBits error, bool state)
{
    int i;
    switch (error)
    {
    case E_PsocErrorBits_HardTemperatureLimit:
        for (i = 0; i < M_NUM_OF_TEMPERATURE_SENSORS; ++i)
            m_temperatureElementsArray[i]->updateErrorBits(E_PSSErrors_DeviceExceedsHardLimits, state);
        break;
    case E_PsocErrorBits_SoftTemperatureLimit:
        for (i = 0; i < M_NUM_OF_TEMPERATURE_SENSORS; ++i)
            m_temperatureElementsArray[i]->updateErrorBits(E_PSSErrors_DeviceExceedsSoftLimits, state);
        break;
    case E_PsocErrorBits_Pt100CalResistorError:
        for (i = 0; i < M_NUM_OF_TEMPERATURE_SENSORS; ++i)
            m_temperatureElementsArray[i]->updateErrorBits(E_PSSErrors_SensorMalfunction, state);
        break;
    }
}

void PsocTemperaturePeripheral::serialize(F_FILE* f)
{
    Serializer<PsocTemperaturePeripheral> s;
    s.serialize(f, *this);
}

PsocTemperaturePeripheral::PsocTemperaturePeripheral(F_FILE* f)
{
    Serializer<PsocTemperaturePeripheral> s;
    s.deserialize(f, *this);
}

