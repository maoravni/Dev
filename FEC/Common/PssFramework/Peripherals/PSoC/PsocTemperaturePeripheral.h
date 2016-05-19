/*
 * PsocTemperaturePeripheral.h
 *
 *  Created on: 8 αιπε 2014
 *      Author: maora
 */

#ifndef PSOCTEMPERATUREPERIPHERAL_H_
#define PSOCTEMPERATUREPERIPHERAL_H_

#include <Peripherals/AnalogInputPeripheralBase.h>
#include <Elements/ValidationElement.h>
#include "PsocPeripheralErrorHandler.h"

#define M_NUM_OF_TEMPERATURE_SENSORS 4

class PsocHandler;

class PsocTemperaturePeripheral: public AnalogInputPeripheralBase, public PsocPeripheralErrorHandler
{
    ValidationElementFloat* m_temperatureElementsArray[M_NUM_OF_TEMPERATURE_SENSORS];
    float m_aCoeff[M_NUM_OF_TEMPERATURE_SENSORS];
    float m_bCoeff[M_NUM_OF_TEMPERATURE_SENSORS];
    float m_hardLimit[M_NUM_OF_TEMPERATURE_SENSORS];
    uint8_t m_sensorType[M_NUM_OF_TEMPERATURE_SENSORS];
    float m_lpfCoeff;
    PsocHandler* m_psocHandler;

public:
    PsocTemperaturePeripheral();
    virtual ~PsocTemperaturePeripheral();

    virtual void readInputs();

    void setPsocHandler(PsocHandler* psocHandler);

    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);
    virtual void enableElementByIndex(int index, bool enable);
    virtual E_PeripheralType getPeripheralType() {return E_PeripheralType_InternalTemperature;}
    virtual int getElementCount() {return M_NUM_OF_TEMPERATURE_SENSORS;}

    virtual void setCalibrationCoeff(int index, float aCoeff, float bCoeff);
    virtual void setScalingCoeff(int index, float aCoeff, float bCoeff);
    virtual void setSensorType(int index, uint8_t type);
    virtual void setLpfWindow(int lpfWindow);

    void downloadConfigurationToPsoc(CBinarySemaphore* completeSemaphore);

    virtual void raiseError(E_PsocErrorBits error, bool state);
    virtual void raiseWarning(E_PSSWarnings warning, bool state) {}

    virtual void serialize(F_FILE* f);
    virtual int deserialize(F_FILE* f);

    template <class T> friend class Serializer;
private:
};

#endif /* PSOCTEMPERATUREPERIPHERAL_H_ */
