/*
 * ModBus8TCPeripheral.h
 *
 *  Created on: 30 Jun 2013
 *      Author: maora
 */

#ifndef MODBUS8TCPERIPHERAL_H_
#define MODBUS8TCPERIPHERAL_H_

#include "AnalogInputPeripheralBase.h"
#include "ModbusPeripheralBase.h"
#include <Elements/ValidationElement.h>
#include <Elements/ElementRepository.h>

#define M_8TC_NUM_OF_SENSORS 8
#define M_8TC_REG_START_ADDRESS 1
#define M_8TC_SAMPLE_INTERVAL 1500

class Modbus8TCPeripheral : public ModbusAnalogInputPeripheralBase
{
    float m_aCoeff[M_8TC_NUM_OF_SENSORS];
    float m_bCoeff[M_8TC_NUM_OF_SENSORS];
//    uint16_t m_startAddress;

    ValidationElementFloat* m_temperatureElementsArray[M_8TC_NUM_OF_SENSORS];
public:
    Modbus8TCPeripheral(uint8_t slaveId);
    Modbus8TCPeripheral(F_FILE* f);
    virtual ~Modbus8TCPeripheral();

    virtual E_PeripheralType getPeripheralType() {return E_PeripheralType_8TC;}
    virtual int getElementCount(){ return M_8TC_NUM_OF_SENSORS;}

    virtual void readInputs();
//    virtual void updateOutputs();
//    virtual void updateNotification(ElementBase* element);

    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);
    virtual void enableElementByIndex(int index, bool enable);
    virtual void setCalibrationCoeff(int index, float aCoeff, float bCoeff);
    virtual void setScalingCoeff(int index, float aCoeff, float bCoeff);
    virtual void setSensorType(int index, uint8_t type) {}

    virtual void  serialize(F_FILE* f);
    virtual int deserialize(F_FILE* f);

    template <class T> friend class Serializer;
};


#endif /* MODBUS8TCPERIPHERAL_H_ */
