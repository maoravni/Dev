/*
 * PsocAnalogInputsPeripheral.h
 *
 *  Created on: 28 באפר 2014
 *      Author: maora
 */

#ifndef PSOCANALOGINPUTSPERIPHERAL_H_
#define PSOCANALOGINPUTSPERIPHERAL_H_

#include <Peripherals/AnalogInputPeripheralBase.h>
#include <Elements/ValidationElement.h>
#include "PsocPeripheralErrorHandler.h"

#define M_NUM_OF_ANALOG_SENSORS 6

#define MAX_UPDATE_INTERVAL 1000
class PsocHandler;

class PsocAnalogInputsPeripheral: public AnalogInputPeripheralBase, public PsocPeripheralErrorHandler
{
    ValidationElementFloat* m_analogElementsArray[M_NUM_OF_ANALOG_SENSORS];
    float m_aCoeff[M_NUM_OF_ANALOG_SENSORS];
    float m_bCoeff[M_NUM_OF_ANALOG_SENSORS];
    uint16_t m_peakCounts;
    uint16_t m_filterSettleCounts;
    float m_lpfCoeff;
    portTickType m_lastEnergyCalculationTickCount;

    PsocHandler* m_psocHandler;

public:
    PsocAnalogInputsPeripheral();
    virtual ~PsocAnalogInputsPeripheral();

    virtual void readInputs();

    void setPsocHandler(PsocHandler* psocHandler);

    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);
    virtual void enableElementByIndex(int index, bool enable);
    virtual E_PeripheralType getPeripheralType()
    {
        return E_PeripheralType_AI;
    }
    virtual int getElementCount()
    {
        return M_NUM_OF_ANALOG_SENSORS;
    }

    virtual void setCalibrationCoeff(int index, float aCoeff, float bCoeff);
    void setScalingCoeff(int index, float aCoeff, float bCoeff);
    virtual void setSensorType(int index, uint8_t type) {}

    void downloadConfigurationToPsoc(CBinarySemaphore* completeSemaphore);

    virtual void raiseError(E_PsocErrorBits error, bool state);
    virtual void raiseWarning(E_PSSWarnings warning, bool state) {}

    virtual void setUpdateInterval(uint16_t updateInterval);
    virtual void setFilterSettleCounts(uint16_t LowPassFilterWindow);

private:
};

#endif /* PSOCANALOGINPUTSPERIPHERAL_H_ */
