/*
 * SwPwmOutputDevice.h
 *
 *  Created on: 13 Jun 2013
 *      Author: maora
 */

#ifndef SWPWMOUTPUTDEVICE_H_
#define SWPWMOUTPUTDEVICE_H_

#include "OutputPeripheralBase.h"
#include <AManagedTask.h>
#include "DigitalOutputsPeripheral.h"
#include <Elements/ValidationElement.h>

enum E_SoftwarePwmMode
{
    E_SoftwarePwmMode_Random,
    E_SoftwarePwmMode_Distributed,
    E_SoftwarePwmMode_Normal
};

struct T_PwmChannelState
{
    bool enabled;
    E_SoftwarePwmMode mode;
    uint32_t totalCycleLength; // total ticks in the PWM cycle
    uint32_t dutyCycleLength;  // ticks active in the cycle
    uint8_t groupId; // group ID of the channel, so we can distribute load across channels in the same group.
    uint32_t currentCounter;
};

class SwPwmOutputPeripheral: public OutputPeripheralBase, public AManagedTask
{
    ValidationElementFloat* m_dutyCycleElementArray[M_NUM_OF_DIGITAL_OUT];
    T_PwmChannelState m_channelStateArray[M_NUM_OF_DIGITAL_OUT];
    uint16_t m_firstElementIndex;
    bool m_channelsEnabled;

public:
    SwPwmOutputPeripheral();
    virtual ~SwPwmOutputPeripheral();

    virtual E_PeripheralType getPeripheralType() {return E_PeripheralType_SwPwm;}
    virtual int getElementCount() {return M_NUM_OF_DIGITAL_OUT;}

    /**
     * Task control loop.
     */
    virtual void run();

    void configPwmChannel(uint8_t channelIndex, uint16_t cycleLength, uint8_t groupId);
//    void enablePwmChannel(uint8_t channelIndex, bool enable);

    virtual void updateOutputs();
    virtual void updateNotification(ElementBase* element);
    virtual bool requestValueChange(ElementBase* element){return true;}
    virtual void timeoutExpired(uint16_t timeoutType){}

    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);
    virtual void enableElementByIndex(int index, bool enable);

    bool createTask();

    virtual int serialize(F_FILE* f);
    virtual int deserialize(F_FILE* f);

private:
    void reassignCounterOffset();
    void setDutyCycle(int i);

    template <class T> friend class Serializer;

};

#endif /* SWPWMOUTPUTDEVICE_H_ */
