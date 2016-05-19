/*
 * PsocDigitalOutputPeripheral.h
 *
 *  Created on: 12 αιπε 2014
 *      Author: maora
 */

#ifndef PSOCDIGITALOUTPUTPERIPHERAL_H_
#define PSOCDIGITALOUTPUTPERIPHERAL_H_

#include <Peripherals/OutputPeripheralBase.h>
#include <Elements/ElementRepository.h>
#include "PsocPeripheralErrorHandler.h"

#define M_NUM_OF_PSOC_PWM_OUTPUTS 6

class PsocHandler;

class PsocDigitalOutputPeripheral: public OutputPeripheralBase, public PsocPeripheralErrorHandler
{
    ValidationElementU8* m_outputElementArray[M_NUM_OF_PSOC_PWM_OUTPUTS];

    PsocHandler* m_psocHandler;

    uint16_t m_firstElementIndex;

public:
    PsocDigitalOutputPeripheral();
    virtual ~PsocDigitalOutputPeripheral();

    virtual E_PeripheralType getPeripheralType() {return E_PeripheralType_HwPWM; }
    virtual int getElementCount() {return M_NUM_OF_PSOC_PWM_OUTPUTS;}
    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);
    virtual void enableElementByIndex(int index, bool enable);

    virtual void updateOutputs();

    virtual void updateNotification(ElementBase* element);
    virtual bool requestValueChange(ElementBase* element){return true;}
    virtual void timeoutExpired(uint16_t timeoutType){}

    void setPsocHandler(PsocHandler* psocHandler);
    void setPsocOutputsEnabled(bool enabled);

    virtual void raiseError(E_PsocErrorBits error, bool state);
    virtual void raiseWarning(E_PSSWarnings warning, bool state) {}

    virtual void serialize(F_FILE* f);
    virtual int deserialize(F_FILE* f);

    template <class T> friend class Serializer;
};

#endif /* PSOCDIGITALOUTPUTPERIPHERAL_H_ */

