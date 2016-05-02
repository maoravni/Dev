/*
 * PsocDigitalInputPeripheral.h
 *
 *  Created on: 19 במאי 2014
 *      Author: maora
 */

#ifndef PSOCDIGITALINPUTPERIPHERAL_H_
#define PSOCDIGITALINPUTPERIPHERAL_H_

#include <Peripherals/InputPeripheralBase.h>
#include <Elements/ElementRepository.h>
#include "PsocPeripheralErrorHandler.h"

#define M_NUM_OF_PSOC_DI_INPUTS 6

class PsocHandler;

class PsocDigitalInputPeripheral: public InputPeripheralBase, public PsocPeripheralErrorHandler
{
    ElementU8* m_inputElementsArray[M_NUM_OF_PSOC_DI_INPUTS];

    PsocHandler* m_psocHandler;

    uint16_t m_firstElementIndex;

public:
    PsocDigitalInputPeripheral();
    virtual ~PsocDigitalInputPeripheral();

    virtual void readInputs();

    void setPsocHandler(PsocHandler* psocHandler);

    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);

   virtual void enableElementByIndex(int index, bool enable);
    virtual E_PeripheralType getPeripheralType()
    {
        return E_PeripheralType_DI;
    }
    virtual int getElementCount()
    {
        return M_NUM_OF_PSOC_DI_INPUTS;
    }

    virtual void raiseError(E_PsocErrorBits error, bool state) {}
    virtual void raiseWarning(E_PSSWarnings warning, bool state) {}

    virtual int serialize(F_FILE* f);
    virtual int deserialize(F_FILE* f);

    template <class T> friend class Serializer;
};

#endif /* PSOCDIGITALINPUTPERIPHERAL_H_ */
