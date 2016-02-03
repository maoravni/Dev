/*
 * ModbusInverter.h
 *
 *  Created on: 9 בספט 2013
 *      Author: maora
 */

#ifndef MODBUSINVERTER_H_
#define MODBUSINVERTER_H_

#include "ModbusPeripheralBase.h"
#include "InputPeripheralBase.h"
#include "OutputPeripheralBase.h"
#include <Elements/ElementRepository.h>

enum E_ModbusInverterType
{
    E_ModbusInverterType_CommanderSK,
    E_ModbusInverterType_UnidriveM200,
    E_ModbusInverterType_SchneiderAltivar32
};

class ModbusInverterPeripheralBase : public ModbusInputOutputPeripheralBase
{
private:
protected:
    ElementBase* m_outputFrequency;
    ElementBase* m_outputCurrent;
    ElementBase* m_frequencySetpoint;
    ElementBase* m_outputEnable;

    E_ModbusInverterType m_inverterType;
    float m_currentMultiplier;
    float m_frequencyMultiplier;
    int m_setpointMultiplier;

    int m_numOfFailedReads;
    int m_numOfFailedEnables;

    bool m_setpointUpdated;

public:
    ModbusInverterPeripheralBase(uint8_t slaveId);
    virtual ~ModbusInverterPeripheralBase();

    virtual E_PeripheralType getPeripheralType() {return E_PeripheralType_Inverter;}
    virtual int getElementCount(){ return 3;}

    virtual void readInputs();
    virtual void updateOutputs();
    virtual void updateNotification(ElementBase* element);
    virtual bool requestValueChange(ElementBase* element){return true;}
    virtual void timeoutExpired(uint16_t timeoutType){}

    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);

    virtual void enableElementByIndex(int index, bool enable);

    void setFrequencySetpointElement(ElementBase* element)
    {
        m_frequencySetpoint = element;
        m_frequencySetpoint->addObserver(this);
    }

    void setOutputEnableElement(ElementBase* element)
    {
        m_outputEnable = element;
    }

    ElementBase* getFrequencySetpointElement() const
    {
        return m_frequencySetpoint;
    }

    ElementBase* getOutputCurrentElement() const
    {
        return m_outputCurrent;
    }

    ElementBase* getOutputFrequencyElement() const
    {
        return m_outputFrequency;
    }

    virtual void setBoardInReady(bool state);
    virtual void startRecovery();

    virtual void setupInverterMinSpeed(float value) = 0;
    virtual void setupInverterMaxSpeed(float value) = 0;
    virtual void setupInverterAccelRate(float value) = 0;
    virtual void setupInverterDecelRate(float value) = 0;
    virtual void setupInverterMotorFrequency(float value) = 0;
    virtual void setupInverterMotorCurrent(float value) = 0;
    virtual void setupInverterMotorNominalRPM(float value) = 0;
    virtual void setupInverter() = 0;
    virtual void setInverterType() = 0;
    virtual void resetInverter() = 0;
};

#endif /* MODBUSINVERTER_H_ */
