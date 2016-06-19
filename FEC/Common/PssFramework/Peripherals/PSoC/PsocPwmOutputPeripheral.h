/*
 * PsocPwmOutputPeripheral.h
 *
 *  Created on: 12 αιπε 2014
 *      Author: maora
 */

#ifndef PSOCPWMOUTPUTPERIPHERAL_H_
#define PSOCPWMOUTPUTPERIPHERAL_H_

#include <Peripherals/OutputPeripheralBase.h>
#include <Elements/ElementRepository.h>
#include "PsocMessageStruct.h"
#include "PsocPeripheralErrorHandler.h"

#define M_NUM_OF_PSOC_PWM_OUTPUTS 6

class PsocHandler;

struct PwmChannelRampParameters
{
    uint8_t riseStep;
    uint8_t fallStep;
    uint8_t riseTime;
    uint8_t fallTime;
};

class PsocPwmOutputPeripheral: public OutputPeripheralBase, public PsocPeripheralErrorHandler
{
    ValidationElementFloat* m_dutyCycleElementArray[M_NUM_OF_PSOC_PWM_OUTPUTS];
    E_DigitalIOType m_pwmChannelType[M_NUM_OF_PSOC_PWM_OUTPUTS];
    PwmChannelRampParameters m_pwmChannelRampParameters[M_NUM_OF_PSOC_PWM_OUTPUTS];
    PsocHandler* m_psocHandler;

    uint16_t m_firstElementIndex;

public:
    PsocPwmOutputPeripheral();
    PsocPwmOutputPeripheral(F_FILE* f);
    virtual ~PsocPwmOutputPeripheral();

    virtual E_PeripheralType getPeripheralType() {return E_PeripheralType_HwPWM; }
    virtual int getElementCount() {return M_NUM_OF_PSOC_PWM_OUTPUTS;}
    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);
    virtual void enableElementByIndex(int index, bool enable);

    virtual void updateOutputs();

    virtual void updateNotification(ElementBase* element);
    virtual bool requestValueChange(ElementBase* element){return true;}
    virtual void timeoutExpired(uint16_t timeoutType){}

    void configPwmChannel(uint8_t channelIndex, uint16_t cycleLength, uint8_t riseStep, uint8_t fallStep, uint16_t riseTime, uint16_t fallTime);
    void downloadConfigurationToPsoc(CBinarySemaphore* completeSemaphore);

    void setPsocHandler(PsocHandler* psocHandler);
    void setPsocOutputsEnabled(bool enabled);

    virtual void raiseError(E_PsocErrorBits error, bool state);
    virtual void raiseWarning(E_PSSWarnings warning, bool state) {}

    virtual void serialize(F_FILE* f);
    virtual int deserialize(F_FILE* f);

    template <class T> friend class Serializer;
};

#endif /* PSOCPWMOUTPUTPERIPHERAL_H_ */
