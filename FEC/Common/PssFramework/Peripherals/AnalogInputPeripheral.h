/*
 * AnalogInputPeripheral.h
 *
 *  Created on: 10 Jun 2013
 *      Author: maora
 */

#ifndef ANALOGINPUTPERIPHERAL_H_
#define ANALOGINPUTPERIPHERAL_H_

#include "AnalogInputPeripheralBase.h"
#include <Elements/ElementRepository.h>
#include <DynamicArray.h>
//typedef DynamicArray<ValidationElementFloat*> T_AnalogInputElements;
//#include <vector>
//typedef std::vector<ValidationElementFloat*> T_AnalogInputElements;
#define M_NUM_OF_ANALOG_INTERNAL_SENSORS 4

class AnalogInputPeripheral: public AnalogInputPeripheralBase
{
    ValidationElementFloat* m_inputElementsArray[M_NUM_OF_ANALOG_INTERNAL_SENSORS];
//    float m_correctionA[M_NUM_OF_ANALOG_INTERNAL_SENSORS];
//    float m_correctionB[M_NUM_OF_ANALOG_INTERNAL_SENSORS];
    float m_scalingA[M_NUM_OF_ANALOG_INTERNAL_SENSORS];
    float m_scalingB[M_NUM_OF_ANALOG_INTERNAL_SENSORS];

    float m_sensorDisconnectedValue;

public:
    AnalogInputPeripheral();
    AnalogInputPeripheral(F_FILE* f);
    virtual ~AnalogInputPeripheral();

    virtual E_PeripheralType getPeripheralType() {return E_PeripheralType_AI;}
    virtual int getElementCount() {return M_NUM_OF_ANALOG_INTERNAL_SENSORS;}

    virtual void readInputs();

    float convert2Digital(float input);
    void convert2Digital(float input, int index);

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

#endif /* ANALOGINPUTPERIPHERAL_H_ */
