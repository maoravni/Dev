/*
 * ElementRepository.cpp
 *
 *  Created on: 9 Jun 2013
 *      Author: maora
 */

#include "ElementRepository.h"
#include "Element.h"

ElementRepository ElementRepository::s_instance;

ElementRepository::ElementRepository()
{
}

ElementRepository::~ElementRepository()
{
}

ElementI8* ElementRepository::addElementI8()
{
    ElementI8 *element = new ElementI8;
    return element;
}

ElementI16* ElementRepository::addElementI16()
{
    ElementI16 *element = new ElementI16;
    return element;
}

ElementI32* ElementRepository::addElementI32()
{
    ElementI32 *element = new ElementI32;
    return element;
}

ElementU8* ElementRepository::addElementU8()
{
    ElementU8 *element = new ElementU8;
    return element;
}

ElementU16* ElementRepository::addElementU16()
{
    ElementU16 *element = new ElementU16;
    return element;
}

ElementU32* ElementRepository::addElementU32()
{
    ElementU32 *element = new ElementU32;
    return element;
}

ElementFloat* ElementRepository::addElementFloat()
{
    ElementFloat *element = new ElementFloat;
    return element;
}

ValidationElementI8* ElementRepository::addValidationElementI8()
{
    ValidationElementI8 *element = new ValidationElementI8;
    return element;
}

ValidationElementI16* ElementRepository::addValidationElementI16()
{
    ValidationElementI16 *element = new ValidationElementI16;
    return element;
}

ValidationElementI32* ElementRepository::addValidationElementI32()
{
    ValidationElementI32 *element = new ValidationElementI32;
    return element;
}

ValidationElementU8* ElementRepository::addValidationElementU8()
{
    ValidationElementU8 *element = new ValidationElementU8;
    return element;
}

ValidationElementU16* ElementRepository::addValidationElementU16()
{
    ValidationElementU16 *element = new ValidationElementU16;
    return element;
}

ValidationElementU32* ElementRepository::addValidationElementU32()
{
    ValidationElementU32 *element = new ValidationElementU32;
    return element;
}

ValidationElementFloat* ElementRepository::addValidationElementFloat()
{
    ValidationElementFloat *element = new ValidationElementFloat;
    return element;
}

void ElementRepository::addElement(ElementBase* element)
{
    element->setElementIndex(m_elementList.size());
    m_elementList.push_back(element);
}

ElementBase* ElementRepository::getElementByIndex(int index)
{
    if (index >= m_elementList.size())
        return NULL;

    T_ElementListIterator i;
    for (i = m_elementList.begin(); i != m_elementList.end(); ++i)
    {
        if ((*i)->getElementIndex() == index)
            return (*i);
    }
    return NULL;
}

ElementBase* ElementRepository::getElementByPssId(int pssId)
{
    T_ElementListIterator i;
    for (i = m_elementList.begin() ;i != m_elementList.end(); ++i)
    {
        if ((*i)->getPssId() == pssId)
            return (*i);
    }
    return NULL;
}

void ElementRepository::setAllMonitoringEnabled(bool monitoringEnabled)
{
    T_ElementListIterator i;
    for (i = m_elementList.begin() ;i != m_elementList.end(); ++i)
    {
        if ((*i)->getPssId() != M_UNASSIGNED_PSS_ID)
            (*i)->setMonitoringEnabled(monitoringEnabled);
    }
}

//void ElementRepository::sendUpdateNotificationForAllElements()
//{
//    T_ElementListIterator i;
//    for (i = m_elementList.begin() ;i != m_elementList.end(); ++i)
//    {
//        if ((*i)->getPssId() != M_UNASSIGNED_PSS_ID)
//            (*i)->sendDeviceStatus();
//    }
//}

void ElementRepository::markElementAsDeleted(int index)
{
    if (index >= m_elementList.size())
        return ;

    int i = 0;
    T_ElementListIterator iterator;
    for (iterator = m_elementList.begin(); i != index && iterator != m_elementList.end(); ++iterator, ++i)
    {
        if (i == index)
        {
            m_elementList.erase(iterator);
            return;
        }
    }
    return ;
}

void ElementRepository::destroyAllElements()
{
    T_ElementListIterator i;
    for (i = m_elementList.begin() ;i != m_elementList.end(); ++i)
        if ((*i) != NULL)
            delete (*i);

    m_elementList.clear();
}
