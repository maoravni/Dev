/*
 * DigitalInputsPeripheral.h
 *
 *  Created on: 28 αιεμ 2013
 *      Author: maora
 */

#ifndef DIGITALINPUTSPERIPHERAL_H_
#define DIGITALINPUTSPERIPHERAL_H_

#include "InputPeripheralBase.h"
#include <Elements/Element.h>
#include <Elements/ElementRepository.h>

#ifdef FEC2_BOARD
#define M_NUM_OF_DIGITAL_INPUTS 13
#else
#define M_NUM_OF_DIGITAL_INPUTS 12
#endif

class DigitalInputsPeripheral: public InputPeripheralBase
{
    ElementU8* m_elementsArray[M_NUM_OF_DIGITAL_INPUTS];
    uint8_t m_inputValues[M_NUM_OF_DIGITAL_INPUTS];
    E_BoardType m_boardType;

public:
    DigitalInputsPeripheral();
    virtual ~DigitalInputsPeripheral();

    virtual E_PeripheralType getPeripheralType() {return E_PeripheralType_DI;}
    virtual int getElementCount() {return M_NUM_OF_DIGITAL_INPUTS;}

    virtual void readInputs();

    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);
    virtual void enableElementByIndex(int index, bool enable);

    virtual int serialize(F_FILE* f);
    virtual int deserialize(F_FILE* f);

    template <class T> friend class Serializer;

};

#endif /* DIGITALINPUTSPERIPHERAL_H_ */
