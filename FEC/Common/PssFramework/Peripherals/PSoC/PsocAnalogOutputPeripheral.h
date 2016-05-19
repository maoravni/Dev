/*
 * PsocAnalogOutputPeripheral.h
 *
 *  Created on: 12 αιπε 2014
 *      Author: maora
 */

#ifndef PsocAnalogOutputPeripheral_H_
#define PsocAnalogOutputPeripheral_H_

#include <Peripherals/OutputPeripheralBase.h>
#include <Elements/ElementRepository.h>
#include "PsocMessageStruct.h"
#include "PsocPeripheralErrorHandler.h"

#define M_NUM_OF_PSOC_ANALOG_OUTPUTS 2

class PsocHandler;

class PsocAnalogOutputPeripheral: public OutputPeripheralBase, public PsocPeripheralErrorHandler
{
    ValidationElementU8* m_elementArray[M_NUM_OF_PSOC_ANALOG_OUTPUTS];
    PsocHandler* m_psocHandler;

    uint16_t m_firstElementIndex;

public:
    PsocAnalogOutputPeripheral();
    virtual ~PsocAnalogOutputPeripheral();

    virtual E_PeripheralType getPeripheralType() {return E_PeripheralType_AO; }
    virtual int getElementCount() {return M_NUM_OF_PSOC_ANALOG_OUTPUTS;}
    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);
    virtual void enableElementByIndex(int index, bool enable);

    virtual void updateOutputs();

    virtual void updateNotification(ElementBase* element);
    virtual bool requestValueChange(ElementBase* element){return true;}
    virtual void timeoutExpired(uint16_t timeoutType){}

    void downloadConfigurationToPsoc(CBinarySemaphore* completeSemaphore);

    void setPsocHandler(PsocHandler* psocHandler);
    void setPsocOutputsEnabled(bool enabled);

    virtual void raiseError(E_PsocErrorBits error, bool state);
    virtual void raiseWarning(E_PSSWarnings warning, bool state) {}

    virtual void serialize(F_FILE* f);
    virtual int deserialize(F_FILE* f);

    template <class T> friend class Serializer;
};


#endif /* PsocAnalogOutputPeripheral_H_ */
