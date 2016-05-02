/*
 * PsocDigitalInputPeripheral.cpp
 *
 *  Created on: 19 במאי 2014
 *      Author: maora
 */

#include "PsocDigitalInputPeripheral.h"
#include "PsocHandler.h"
#include "Persistency/PsocPeripheralSerializers.h"

PsocDigitalInputPeripheral::PsocDigitalInputPeripheral()
{
    m_psocHandler = NULL;

    for (int i = 0; i < M_NUM_OF_PSOC_DI_INPUTS; ++i)
    {
        m_inputElementsArray[i] = ElementRepository::getInstance().addElementU8();
        m_inputElementsArray[i]->setValue((int)2);

    }
    // TODO: Implement digital inputs using interrupts.
    setUpdateInterval(10);
}

PsocDigitalInputPeripheral::~PsocDigitalInputPeripheral()
{
}

void PsocDigitalInputPeripheral::readInputs()
{
    if (m_psocHandler->getPsocCommState() == E_PsocCommState_Enabled && m_boardInReady)
    {
        int i;
        uint16_t digitalInputs = m_psocHandler->getLocalDigitalInputsValue();
        uint8_t di;
        for (i = 0; i < M_NUM_OF_PSOC_DI_INPUTS; ++i)
        {
            di = digitalInputs & 1;
            if (di != (uint8_t)m_inputElementsArray[i]->getValueU32()) m_inputElementsArray[i]->setValue(di);
            digitalInputs = digitalInputs >> 1;
        }
    }
}

void PsocDigitalInputPeripheral::setPsocHandler(PsocHandler* psocHandler)
{
    m_psocHandler = psocHandler;
}

ElementBase* PsocDigitalInputPeripheral::getElementByIndex(int index)
{
    return m_inputElementsArray[index];
}

ElementBase* PsocDigitalInputPeripheral::getElementByPssId(int pssId)
{
    for (int i = 0; i < M_NUM_OF_PSOC_DI_INPUTS; ++i)
    {
        if (m_inputElementsArray[i]->getPssId() == pssId)
            return m_inputElementsArray[i];
    }
    return NULL;
}

void PsocDigitalInputPeripheral::enableElementByIndex(int index, bool enable)
{
}

int PsocDigitalInputPeripheral::serialize(F_FILE* f)
{
    Serializer<PsocDigitalInputPeripheral> s;
    return s.serialize(f, *this);
}
