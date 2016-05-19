/*
 * VirtualPeripheral.cpp
 *
 *  Created on: 16 Sep 2015
 *      Author: maora
 */

#include <Peripherals/VirtualPeripheral.h>
#include <Elements/ElementRepository.h>
#include "Persistency/PeripheralSerializers.h"

VirtualPeripheral::VirtualPeripheral(E_PeripheralType peripheralType)
{
    m_peripheralType = peripheralType;
}

VirtualPeripheral::~VirtualPeripheral()
{
}

void VirtualPeripheral::execute()
{
}

E_PeripheralType VirtualPeripheral::getPeripheralType()
{
    return m_peripheralType;
}

int VirtualPeripheral::getElementCount()
{
    return m_elementList.size();
}

ElementBase* VirtualPeripheral::getElementByIndex(int index)
{
    if (index < m_elementList.size())
        return m_elementList[index];

    // if this is the next element, simply add it to the list.
    if (index == m_elementList.size())
    {
        switch (m_peripheralType)
        {
        case E_PeripheralType_VirtualFloatPeripheral:
            m_elementList.push_back(ElementRepository::getInstance().addValidationElementFloat());
            break;
        case E_PeripheralType_VirtualIntegerPeripheral:
            m_elementList.push_back(ElementRepository::getInstance().addValidationElementI32());
            break;
        }
        return m_elementList[index];
    }

    return NULL;
}

ElementBase* VirtualPeripheral::getElementByPssId(int pssId)
{
    T_ElementListIterator it;
    for (it = m_elementList.begin(); it != m_elementList.end(); ++it)
    {
        if ((*it)->getPssId() == pssId)
            return (*it);
    }
    return NULL;
}

void VirtualPeripheral::enableElementByIndex(int index, bool enable)
{
}

void VirtualPeripheral::serialize(F_FILE* f)
{
    Serializer<VirtualPeripheral> s;
    s.serialize(f, *this);
}

VirtualPeripheral::VirtualPeripheral(F_FILE* f)
{
    Serializer<VirtualPeripheral> s;
    s.deserialize(f, *this);
}
