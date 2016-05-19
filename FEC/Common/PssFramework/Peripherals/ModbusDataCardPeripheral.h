/*
 * ModBusDataCardPeripheral.h
 *
 *  Created on: 30 Jun 2013
 *      Author: maora
 */

#ifndef MODBUSDataCardPERIPHERAL_H_
#define MODBUSDataCardPERIPHERAL_H_

#include "AnalogInputPeripheralBase.h"
#include "ModbusPeripheralBase.h"
#include <Elements/ValidationElement.h>
#include <Elements/ElementRepository.h>

#define M_DataCard_NUM_OF_SENSORS 8
#define M_DataCard_REG_START_ADDRESS 0
#define M_DataCard_SAMPLE_INTERVAL 2000

class ModbusDataCardPeripheral : public ModbusAnalogInputPeripheralBase
{
    float m_aCoeff[M_DataCard_NUM_OF_SENSORS];
    float m_bCoeff[M_DataCard_NUM_OF_SENSORS];
//    uint16_t m_startAddress;

    ValidationElementFloat* m_temperatureElementsArray[M_DataCard_NUM_OF_SENSORS];
public:
    ModbusDataCardPeripheral(uint8_t slaveId);
    ModbusDataCardPeripheral(F_FILE* f);
    virtual ~ModbusDataCardPeripheral();

    virtual E_PeripheralType getPeripheralType() {return E_PeripheralType_DataCard;}
    virtual int getElementCount(){ return M_DataCard_NUM_OF_SENSORS;}

    virtual void readInputs();
//    virtual void updateOutputs();
//    virtual void updateNotification(ElementBase* element);

    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);

    virtual void enableElementByIndex(int index, bool enable);
    virtual void setCalibrationCoeff(int index, float aCoeff, float bCoeff);
    virtual void setScalingCoeff(int index, float aCoeff, float bCoeff);
    virtual void setSensorType(int index, uint8_t type) {}

    virtual void serialize(F_FILE* f);
    virtual int deserialize(F_FILE* f);

    template <class T> friend class Serializer;
};


#endif /* MODBUSDataCardPERIPHERAL_H_ */
