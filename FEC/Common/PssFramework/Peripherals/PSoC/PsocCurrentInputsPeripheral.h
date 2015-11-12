/*
 * PsocCurrentInputsPeripheral.h
 *
 *  Created on: 28 באפר 2014
 *      Author: maora
 */

#ifndef PsocCurrentInputsPeripheral_H_
#define PsocCurrentInputsPeripheral_H_

#include <Peripherals/AnalogInputPeripheralBase.h>
#include <Elements/ValidationElement.h>

#define M_NUM_OF_ANALOG_SENSORS 4

class PsocHandler;

class PsocCurrentInputsPeripheral: public AnalogInputPeripheralBase
{
    ValidationElementFloat* m_analogElementsArray[M_NUM_OF_ANALOG_SENSORS];
    float m_aCoeff[M_NUM_OF_ANALOG_SENSORS];
    float m_bCoeff[M_NUM_OF_ANALOG_SENSORS];
    uint16_t m_peakCounts;
    float m_lpfCoeff;

    PsocHandler* m_psocHandler;

public:
    PsocCurrentInputsPeripheral();
    virtual ~PsocCurrentInputsPeripheral();

    virtual void readInputs();

    void setPsocHandler(PsocHandler* psocHandler);

    virtual ElementBase* getElementByIndex(int index);
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

    void downloadConfigurationToPsoc(CBinarySemaphore* completeSemaphore);

private:
    virtual void setUpdateInterval(uint16_t updateInterval);
};

#endif /* PsocCurrentInputsPeripheral_H_ */
