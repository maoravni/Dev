/*
 * ElementRepository.h
 *
 *  Created on: 9 Jun 2013
 *      Author: maora
 */

#ifndef ELEMENTREPOSITORY_H_
#define ELEMENTREPOSITORY_H_

#include <Elements/ElementBase.h>
#include <Elements/Element.h>
#include <DynamicArray.h>

//typedef DynamicArray<ElementBase*> T_ElementVector;

class ElementRepository
{
private:
    static ElementRepository s_instance;

//    T_ElementVector m_elementVector;
    T_ElementList m_elementList;

    ElementRepository();

public:
    virtual ~ElementRepository();

    static ElementRepository& getInstance()
    {
        return s_instance;
    }

    ElementI8* addElementI8();
    ElementI16* addElementI16();
    ElementI32* addElementI32();
    ElementU8* addElementU8();
    ElementU16* addElementU16();
    ElementU32* addElementU32();
    ElementFloat* addElementFloat();

    ValidationElementI8* addValidationElementI8();
    ValidationElementI16* addValidationElementI16();
    ValidationElementI32* addValidationElementI32();
    ValidationElementU8* addValidationElementU8();
    ValidationElementU16* addValidationElementU16();
    ValidationElementU32* addValidationElementU32();
    ValidationElementFloat* addValidationElementFloat();

    void addElement(ElementBase* element);
    //void markElementAsDeleted(ElementBase* element);
    void markElementAsDeleted(int index);

    ElementBase* getElementByIndex(int index);
    ElementBase* getElementByPssId(int pssId);

    void setAllMonitoringEnabled(bool monitoringEnabled);
//    void sendUpdateNotificationForAllElements();

    void destroyAllElements();
private:

    template <class T> friend class Serializer;
};

#endif /* ELEMENTREPOSITORY_H_ */
