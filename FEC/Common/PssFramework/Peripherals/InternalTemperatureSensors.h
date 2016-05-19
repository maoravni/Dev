/*
 * InternalTemperatureSensors.h
 *
 *  Created on: 10 Jun 2013
 *      Author: maora
 */

#ifndef INTERNALTEMPERATURESENSORS_H_
#define INTERNALTEMPERATURESENSORS_H_

#include "InputPeripheralBase.h"
#include "AnalogInputPeripheralBase.h"
#include <Elements/ElementRepository.h>
//#include <DynamicArray.h>
//typedef DynamicArray<ValidationElementFloat*> T_TemperatureSensorElements;
//#include <vector>
//typedef std::vector<ValidationElementFloat*> T_TemperatureSensorElements;

#define M_NUM_OF_INTERNAL_SENSORS 12

class InternalTemperatureSensors: public AnalogInputPeripheralBase
{
    ValidationElementFloat* m_temperatureElementsArray[M_NUM_OF_INTERNAL_SENSORS];
    float m_aCoeff[M_NUM_OF_INTERNAL_SENSORS];
    float m_bCoeff[M_NUM_OF_INTERNAL_SENSORS];

    float m_sensorDisconnectedValue;

public:
    InternalTemperatureSensors();
    InternalTemperatureSensors(F_FILE* f);
    virtual ~InternalTemperatureSensors();

    virtual E_PeripheralType getPeripheralType() {return E_PeripheralType_InternalTemperature;}
    virtual int getElementCount() {return M_NUM_OF_INTERNAL_SENSORS;}

    virtual void readInputs();

    float convertDigital2Temperature(float input, float aCoeff, float bCoeff);
    void convertDigital2Temperature(float input, int index);

    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);

    virtual void enableElementByIndex(int index, bool enable);

    float getSensorDisconnectedValue() const
    {
        return m_sensorDisconnectedValue;
    }

    void setSensorDisconnectedValue(float sensorDisconnectedValue)
    {
        m_sensorDisconnectedValue = sensorDisconnectedValue;
    }

    void setLpfWindow(unsigned long lpfWindow);

    virtual void setCalibrationCoeff(int index, float aCoeff, float bCoeff);
    virtual void setScalingCoeff(int index, float aCoeff, float bCoeff);
    virtual void setSensorType(int index, uint8_t type) {}

    virtual void serialize(F_FILE* f);
    virtual int deserialize(F_FILE* f);

    template <class T> friend class Serializer;

};

#endif /* INTERNALTEMPERATURESENSORS_H_ */
